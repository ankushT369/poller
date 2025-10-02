#include <stdio.h>

#include "db_adapter.h"
#include "pg_adapter.h"

/* Adapter registry */
static const adapter_ops* adapter_registry[DB_LIMIT] = { NULL };


bool db_config_set_uri(db_conf *config) {
    if (!config || strlen(config->host) == 0 || strlen(config->dbname) == 0) {
        return false;
    }

    const char *adapter_str = (config->adapter < DB_LIMIT) ? 
                             db_map[config->adapter] : "unknown";

    snprintf(
        config->uri, sizeof(config->uri),
        "%s://%s:%s@%s:%" PRIu16 "/%s",
        adapter_str,
        config->user[0] ? config->user : "user",
        config->password[0] ? config->password : "password",
        config->host,
        config->port,
        config->dbname
    );

    return true;
}


void adapter_register(db_type type, const adapter_ops *ops) {
    if (type < DB_LIMIT)
        adapter_registry[type] = ops;
}


 const adapter_ops* adapter_get_ops(db_type type) {
    if (type < DB_LIMIT) {
        return adapter_registry[type];
    }
    return NULL;
}

void hv_set_disconnected(db_conn* conn) {
    conn->hv.st = CONN_DISCONNECTED;
}

void hv_set_connected(db_conn* conn) {
    conn->hv.st = CONN_CONNECTING;
}

void hv_set_ready(db_conn* conn) {
    conn->hv.st = CONN_READY;
}

void hv_set_querying(db_conn* conn) {
    conn->hv.st = CONN_QUERYING;
}

void hv_set_err(db_conn* conn) {
    conn->hv.st = CONN_ERROR;
}
