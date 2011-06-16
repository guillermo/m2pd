#ifndef M2PD_H
#define M2PD_H

#include "m2_client.h"
#include <m_pd.h>

#define POST(msg)   post("[debug] %s:%d > %s", __FILE__, __LINE__, msg)
#define PRINT(msg)   printf("[debug] %s:%d %s > %s\n", __FILE__, __LINE__, __func__, msg)
#define FLUSH fflush(stdout)
#define D(msg) POST(msg); \
        PRINT(msg); \
        FLUSH



typedef struct _m2pd {
	t_object x_obj;
	s_context *m2_context;
	t_symbol *last_body;
} t_m2pd;

static void m2pd_bang(t_m2pd *x);
void m2pd_setup(void);
static void *m2pd_new(void);
static void m2pd_destroy(t_m2pd *object);

#endif
