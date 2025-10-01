#ifndef DB_ADAPTER_H
#define DB_ADAPTER_H

#include "harvester.h"
#include "pg_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct db_conn db_conn;
typedef struct adapter_ops adapter_ops;


typedef enum db_type {
    POSTGRES,
    MYSQL,
    MONGODB,
    LIMIT,
} db_type;

static const char* db_map[LIMIT] = {
    "postgres",
    "mysql",
    "mongodb",
};

/** 
 * LATER THIS COMMENT NEED TO CHANGE
 * 
 * poll_result defines a pool of database connections.
 * It contains the total number of connections and an array of 
 * pointers to individual db_conn instances. */

//typedef struct {
//    stmt_st **stmts;
//    size_t nstmts;
//} poll_result;


typedef struct {
    int max_attempts;
    int interval_seconds;
} retry_policy;


/** 
 * db_conn defines a single database connection. 
 * It contains the connection ID, host, port, adapterID,
 * dbname, user, password(encrypted), uri, harvester instance,
 * ref to a vtable and adapter-private ctx. */
struct db_conn {
    uint32_t id;

    char name[128];
    char host[128];
    uint16_t port;
    uint32_t adapter;
    char dbname[128];
    char user[64];
    char password[128];
    char uri[512];

    /* harvester state */
    hvst hv;     
    /* vtable */
    const adapter_ops *ops; 
    /* adapter-private ctx (PGconn*, mongoc_client_t*, etc.) */
    void *priv;

    struct stmt_st {
        union {
            void* generic;
            /* Test with postgres for now */
            pg_stat* pg_stmt;
            //mysql_stat* mysql_stmt;
            //mongo_stat* mongo_stmt;
        } db_spec;
    } st;

    retry_policy policy;
};


/**
 * Adapter vtable. */
struct adapter_ops {
    /* Global init/shutdown */
    int  (*init)(void *global_ctx);
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


bool set_uri(db_conn* conn, size_t conn_size);

#ifdef __cplusplus
}
#endif


#endif // DB_ADAPTER_H
