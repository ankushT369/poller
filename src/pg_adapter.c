#include <libpq-fe.h>
#include <string.h>

#include "db_adapter.h"
#include "pg_adapter.h"

#include <stdlib.h>


/* THIS IS A TESTING IMPLEMENTATION */
static int pg_connect(db_conn *conn) {
    if (!conn) return -1;
    
    pg_ctx_t *ctx = calloc(1, sizeof(pg_ctx_t));
    if (!ctx) return -1;
    
    PGconn *pg_conn = PQconnectdb(conn->config.uri);
    ctx->pg_conn = pg_conn;
    conn->priv = ctx;

    if (PQstatus(pg_conn) != CONNECTION_OK) {
        strncpy(ctx->last_error, PQerrorMessage(pg_conn), sizeof(ctx->last_error)-1);
        PQfinish(pg_conn);
        //free(ctx);
        return -1;
    }
    
    hv_set_connected(conn);
    
    return 0;
}

static const char* pg_get_error(db_conn *conn) {
    if (!conn || !conn->priv)
        return "No context or unknown error";
    pg_ctx_t *ctx = (pg_ctx_t*)conn->priv;
    return ctx->last_error;
}

const adapter_ops* get_pg_adapter_ops(void) {
    return &pg_adapter_ops;
}

/* Define the actual pg_adapter_ops variable */
const adapter_ops pg_adapter_ops = {
    .connect = pg_connect,
    .get_error = pg_get_error,
    // Add other function pointers as needed
};

