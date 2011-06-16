#include "m2pd.h"

static t_class *m2pd_class;

void m2pd_setup(void)
{
	m2_init();

	m2pd_class = class_new(gensym("m2pd"),
	                       (t_newmethod)m2pd_new,
	                       (t_method)m2pd_destroy,
	                       sizeof(t_m2pd),
	                       CLASS_DEFAULT,
	                       0);

	class_addbang(m2pd_class, m2pd_bang);
}

static void m2pd_bang(t_m2pd *instance)
{
	s_request *request;

	request = m2_receive_request_nonblock(instance->m2_context);
	if (request == NULL)
	{
		return;
	}
	m2_send_response(instance->m2_context, request, "200 OK", ":-P");

	instance->last_body = gensym(request->body);
	outlet_symbol(instance->x_obj.ob_outlet, instance->last_body);
	m2_free_request(request);
}

static void *m2pd_new(void)
{
	s_context *m2_context;

	m2_context = m2_init_context();
	if (m2_context == NULL)
	{
		return NULL;
	}

	t_m2pd *instance = (t_m2pd*)pd_new(m2pd_class);
	instance->m2_context = m2_context;

	outlet_new(&instance->x_obj, &s_symbol);
	return instance;
}

static void m2pd_destroy(t_m2pd *object)
{
	m2_finalize_context(object->m2_context);
}
