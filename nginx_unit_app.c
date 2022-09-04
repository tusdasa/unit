#include <nxt_unit.h>
#include <nxt_unit_request.h>
#include <nxt_unit_response.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>


#define CONTENT_TYPE  "Content-Type"
#define APPLICATION_JSON    "application/json"
#define RESPONSE_CONTENT "{\"code\":200,\"msg\":\"ok\"}"

static int ready_handler(nxt_unit_ctx_t *ctx);
static void* worker(void *main_ctx);
static void my_app_request_handler(nxt_unit_request_info_t *req);
//static inline char* copy(char *p, const void *src, uint32_t len);

static int        thread_count;
static pthread_t  *threads;

int main(int argc, char **argv) {
	int i, err;
	nxt_unit_ctx_t *ctx;
	nxt_unit_init_t init;

	if (argc == 3 && strcmp(argv[1], "-t") == 0) {
		thread_count = atoi(argv[2]);
	}

	memset(&init, 0, sizeof(nxt_unit_init_t));

	init.callbacks.request_handler = my_app_request_handler;
	init.callbacks.ready_handler = ready_handler;

	ctx = nxt_unit_init(&init);
	if (ctx == NULL) {
		return 1;
	}

	err = nxt_unit_run(ctx);

	nxt_unit_debug(ctx, "main worker finished with %d code", err);

	if (thread_count > 1) {
		for (i = 0; i < thread_count - 1; i++) {
			err = pthread_join(threads[i], NULL);

			if (err == 0) {
				nxt_unit_debug(ctx, "join thread #%d", i);

			} else {
				nxt_unit_alert(ctx, "pthread_join(#%d) failed: %s (%d)", i,
						strerror(err), err);
			}
		}

		nxt_unit_free(ctx, threads);
	}

	nxt_unit_done(ctx);

	nxt_unit_debug(NULL, "main worker done");
	return 0;
}

static void my_app_request_handler(nxt_unit_request_info_t *req) {


	int                 rc;
	nxt_unit_request_t  *r;

	rc = nxt_unit_response_init(req, 200 /* Status code. */,
	                                1 /* Number of response headers. */,
									sizeof(CONTENT_TYPE)-1
	                                + sizeof(APPLICATION_JSON)-1
	                                + sizeof(RESPONSE_CONTENT)-1);
	if(rc!=NXT_UNIT_OK){
		goto fail;
	}

	rc = nxt_unit_response_add_field(req,
			CONTENT_TYPE,
			sizeof(CONTENT_TYPE)-1,
			APPLICATION_JSON,
			sizeof(APPLICATION_JSON)-1);

	if (rc != NXT_UNIT_OK) {
		goto fail;
	}

	rc = nxt_unit_response_add_content(req, RESPONSE_CONTENT,
			sizeof(RESPONSE_CONTENT)-1);
	if (rc != NXT_UNIT_OK) {
		goto fail;
	}

	rc = nxt_unit_response_send(req);
	if (rc != NXT_UNIT_OK) {
		goto fail;
	}

//	r = req->request;
//
//	if(strcmp(nxt_unit_sptr_get(&r->method),"GET")==0){
//		nxt_unit_debug(NULL, "method");
//	}



fail:
	nxt_unit_request_done(req, rc);
}

static int ready_handler(nxt_unit_ctx_t *ctx) {
	int i, err;

	nxt_unit_debug(ctx, "ready");

	if (thread_count <= 1) {
		return NXT_UNIT_OK;
	}

	threads = nxt_unit_malloc(ctx, sizeof(pthread_t) * (thread_count - 1));
	if (threads == NULL) {
		return NXT_UNIT_ERROR;
	}

	for (i = 0; i < thread_count - 1; i++) {
		err = pthread_create(&threads[i], NULL, worker, ctx);
		if (err != 0) {
			return NXT_UNIT_ERROR;
		}
	}

	return NXT_UNIT_OK;
}

static void*
worker(void *main_ctx) {
	int rc;
	nxt_unit_ctx_t *ctx;

	ctx = nxt_unit_ctx_alloc(main_ctx, NULL);
	if (ctx == NULL) {
		return NULL;
	}

	nxt_unit_debug(ctx, "start worker");

	rc = nxt_unit_run(ctx);

	nxt_unit_debug(ctx, "worker finished with %d code", rc);

	nxt_unit_done(ctx);

	return (void*) (intptr_t) rc;
}

/*
static inline char*
copy(char *p, const void *src, uint32_t len) {
	memcpy(p, src, len);

	return p + len;
}
*/
