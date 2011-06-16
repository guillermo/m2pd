#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "zhelpers.h"
#include <string.h>

#define REQUEST_ADDR "tcp://localhost:9999"
#define RESPONSE_ADDR "tcp://localhost:9998"

typedef struct {
	void *subscriber;
	void *responder;
} s_context;

typedef struct {
	char *uuid;
	char *client_id;
	char *path;
	char *headers;
	char *body;
} s_request;

typedef struct {
	char *uuid;
	char *client_id;
	char *body;
} s_response;

#define HTTP_200 = "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nContent-Length:15\r\nServer: PureData\r\n\r\nOK"

#define DEBUG(name, value) printf("%-10s: %s\n", name, value)

// Public
extern void m2_init(void);
extern void m2_finalize(void);
extern s_context  *m2_init_context(void);
extern int         m2_finalize_context(s_context*);
extern s_request  *m2_receive_request(s_context*);
extern s_request  *m2_receive_request_nonblock(s_context*);
extern void        m2_free_request(s_request*);
extern int m2_send_response(s_context*, s_request*, char *status, char *body);

#endif
