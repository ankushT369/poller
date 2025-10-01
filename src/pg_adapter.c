#include "db_adapter.h"
#include "pg_adapter.h"

static int pg_init(void* global_ctx) {
    return 21; 
}



adapter_ops pg_ops = {
    /* Global init/shutdown */
    //int  (*init)(void *global_ctx);
    .init = pg_init,
    //void (*shutdown)(void);

    /* Async connection */
    //db_conn* (*connect)(const char *uri);
    //void     (*disconnect)(db_conn *c);

    /* Get underlying fd (for epoll/kqueue integration(later)) */
    //int      (*get_fd)(db_conn *c);

    /* Trigger async poll (e.g., send query/stat request) */
    //int      (*prepare_poll)(db_conn *c);

    /* Consume ready results, returns poll_result (must be freed by core) */
    //poll_result* (*consume)(db_conn *c);

    /* Free poll_result memory */
    //void (*free_result)(poll_result *r);
};
