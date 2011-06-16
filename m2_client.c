//
// Mongrel 2 client library.
//

#include "m2_client.h"

static void *zmq_context;

/* Init Mongrel 2. */
void m2_init(void)
{
	zmq_context = zmq_init(1);
}

/* Finalize Mongrel 2. */
void m2_finalize(void)
{
	zmq_term(zmq_context);
}

/* Connect to a server. */
s_context *m2_init_context(void)
{
	s_context *ctx = malloc(sizeof(s_context));

	if (ctx == NULL)
	{
		perror("When creating the mongrel 2 context");
		return NULL;
	}

	ctx->subscriber = zmq_socket(zmq_context, ZMQ_SUB);
	if (ctx->subscriber == NULL)
	{
		perror("When creating subscriber socket");
		free(ctx);
		return(NULL);
	}

	if (zmq_connect(ctx->subscriber, REQUEST_ADDR) != 0)
	{
		perror("When connecting subscriber socket");
		zmq_close(ctx->subscriber);
		free(ctx);
		return NULL;
	}

	if (zmq_setsockopt(ctx->subscriber, ZMQ_SUBSCRIBE, "", 0) != 0)
	{
		perror("When setting subscriber options");
		zmq_close(ctx->subscriber);
		free(ctx);
		return NULL;
	}

	ctx->responder = zmq_socket(zmq_context, ZMQ_PUB);
	if (ctx->responder == NULL)
	{
		perror("When creating responder socket");
		zmq_close(ctx->subscriber);
		free(ctx);
		return(NULL);
	}

	if (zmq_connect(ctx->responder, RESPONSE_ADDR) != 0)
	{
		perror("When connectiong responder socket");
		zmq_close(ctx->subscriber);
		zmq_close(ctx->responder);
		free(ctx);
		return(NULL);
	}
	return ctx;
}

/* Disconect from the server. */
int m2_finalize_context(s_context *ctx)
{
	zmq_close(ctx->subscriber);
	zmq_close(ctx->responder);
	free(ctx);
	return 0;
}

/* Parse a request. */
s_request *m2_parse_request(char *raw_request)
{
	char **ptr = &raw_request;
	long size;
	char *ssize;

	s_request *request = (s_request*)malloc(sizeof(s_request));

	request->uuid = strsep(ptr, " ");
	request->client_id = strsep(ptr, " ");
	request->path = strsep(ptr, " ");

	ssize = strsep(ptr, ":");
	request->headers = *ptr;
	size = strtol(ssize, NULL, 10);
	*(*ptr + size) = '\0';

	*ptr = (*ptr + size + 1);
	ssize = strsep(ptr, ":");
	size = strtol(ssize, NULL, 10);
	*(*ptr + size) = '\0';
	request->body = *ptr;

	return request;
}

/* Receive a request and return the parsed one.
   If there is no request waiting it blocks. */
s_request *m2_receive_request(s_context *ctx)
{
	char *raw_request = s_recv(ctx->subscriber);
	s_request *request = m2_parse_request(raw_request);

	return request;
}

/* Same as m2_receive_request but return NULL if there
   is no request. */
s_request *m2_receive_request_nonblock(s_context *ctx)
{
	char *raw_request;

	raw_request = s_recv_nonblock(ctx->subscriber);
	if (raw_request == NULL)
	{
		return(NULL);
	}

	s_request *request = m2_parse_request(raw_request);
	return request;
}

/* Free request resources. */
void m2_free_request(s_request *request)
{
	free(request->uuid);
	free(request);
}

/* Format response body. */
char *m2_format_response_body(char *status, char *body)
{
	char *format = "HTTP/1.1 %s\r\nContent-Type: text/plain\r\nContent-Length: %zu\r\nServer: m2_pure_data\r\n\r\n%s";
	char *response;

	asprintf(&response, format, status, strlen(body), body);
	return response;
}

/* Format response. */
char *m2_format_response(char *uuid, char *client_id, char *status, char *body)
{
	char *raw_body = m2_format_response_body(status, body);

	if (raw_body == NULL)
		return NULL;
	char *response;
	char *format = "%s %zu:%s, %s";
	asprintf(&response, format, uuid, strlen(client_id), client_id, raw_body);
	free(raw_body);
	return response;
}

/* Send response.
   For example:
     m2_send_response(m2_context, request, "200 OK", "The body of the response");
   return 0 if success. */
int m2_send_response(s_context *ctx, s_request *request, char *status, char *body)
{
	char *raw_response = m2_format_response(request->uuid, request->client_id, status, body);

	if (raw_response == NULL)
		return -1;
	int rc = s_send(ctx->responder, raw_response);
	free(raw_response);
	return rc;
}
