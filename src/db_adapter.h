#ifndef DB_ADAPTER_H
#define DB_ADAPTER_H

#include "harvester.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct db_conn db_conn;
typedef struct adapter_ops adapter_ops;


typedef enum db_type {
    DB_POSTGRES,
    DB_MYSQL,
    DB_MONGODB,
    DB_LIMIT,
} db_type;

static const char* db_map[DB_LIMIT] = {
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
 *
 */
typedef struct {
    uint32_t id;

    char name[128];
    char host[128];
    uint16_t port;
    uint32_t adapter;
    char dbname[128];
    char user[64];
    char password[128];
    char uri[512];

    retry_policy policy;
} db_conf; 


/** 
 * db_conn defines a single database connection. 
 * It contains the connection ID, host, port, adapterID,
 * dbname, user, password(encrypted), uri, harvester instance,
 * ref to a vtable and adapter-private ctx. */
struct db_conn {
    db_conf config;

    /* harvester state */
    hvst hv;     
    /* vtable */
    const adapter_ops *ops; 
    /* adapter-private ctx (PGconn*, mongoc_client_t*, etc.) */
    void *priv;

    retry_policy policy;
};


/**
 * Adapter vtable. */
struct adapter_ops {
    /* Async connection */
    int (*connect)(db_conn* conn);
    //void     (*disconnect)(db_conn *c);

    /* Get underlying fd (for epoll/kqueue integration(later)) */
    //int      (*get_fd)(db_conn *c);

    /* Trigger async poll (e.g., send query/stat request) */
    //int      (*prepare_poll)(db_conn *c);

    /* Consume ready results, returns poll_result (must be freed by core) */
    //poll_result* (*consume)(db_conn *c);

    /* Utility */
    const char* (*get_error)(db_conn *conn);
    /* Free poll_result memory */
    //void (*free_result)(poll_result *r);
};


/* Public API */
bool db_config_set_uri(db_conf *config);
//db_conn* db_conn_create(const db_conf *config);
void db_conn_destroy(db_conn *conn);

/* Adapter registry */
void adapter_register(db_type type, const adapter_ops *ops);
const adapter_ops* adapter_get_ops(db_type type);

/* Harvester Settings */
void hv_set_disconnected(db_conn* conn);
void hv_set_connected(db_conn* conn);
void hv_set_ready(db_conn* conn);
void hv_set_querying(db_conn* conn);
void hv_set_err(db_conn* conn);
void hv_poll_cnt_init(db_conn* conn);


#ifdef __cplusplus
}
#endif


#endif // DB_ADAPTER_H
