#ifndef DB_ADAPTER_H
#define DB_ADAPTER_H

#include "harvester.h"
#include "pg.h"


typedef struct statement_stat stmt_st;
typedef struct db_conn db_conn;
typedef struct adapter_ops adapter_ops;


/** 
 * conn_pool defines a pool of database connections. 
 * It contains the total number of connections and an array of 
 * pointers to individual db_conn instances. */
typedef struct {
    stmt_st **stmts;
    size_t nstmts;
} poll_result;


/** 
 * db_conn defines a single database connection. 
 * It contains the connection ID, host, port, ref 
 * to an associated statement handle, ref to a harvester state
 * ref to a vtable and adapter-private ctx. */
struct db_conn {
    uint32_t id;
    const char* host;
    const char* user;
    const char* dbname;
    int32_t port;

    /* harvester state */
    hvst *hv;                  
    /* vtable */
    const adapter_ops *ops;    
    /* adapter-private ctx (PGconn*, mongoc_client_t*, etc.) */
    void *priv;                
};


/**
 * Adapter vtable. */
struct adapter_ops {
    const char *name;

    /* Global init/shutdown */
    int  (*init)(void *global_ctx);
    void (*shutdown)(void);

    /* Async connection */
    db_conn* (*connect)(const char *uri);
    void     (*disconnect)(db_conn *c);

    /* Get underlying fd (for epoll/kqueue integration(later)) */
    int      (*get_fd)(db_conn *c);

    /* Trigger async poll (e.g., send query/stat request) */
    int      (*prepare_poll)(db_conn *c);

    /* Consume ready results, returns poll_result (must be freed by core) */
    poll_result* (*consume)(db_conn *c);

    /* Free poll_result memory */
    void (*free_result)(poll_result *r);
};

#endif // DB_ADAPTER_H
