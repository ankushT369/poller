#include <stdio.h>

#include "db_adapter.h"


bool set_uri(db_conn *conn, size_t conn_size) {
    if (!conn)
        return false;

    for (size_t i = 0; i < conn_size; i++) {
        db_conn *c = &conn[i];

        /* validate adapter index */
        if (c->adapter == LIMIT)
            return false;

        const char *adapter = (c->adapter < LIMIT) ? db_map[c->adapter] : "unknown";

        /* skip if host/dbname missing */
        if (strlen(c->host) == 0 || strlen(c->dbname) == 0) {
            c->uri[0] = '\0';
            continue;
        }

        /* build the URI directly into the fixed buffer */
        snprintf(
            c->uri, sizeof(c->uri),
            "%s://%s:%s@%s:%" PRId32 "/%s",
            adapter,
            c->user[0] ? c->user : "user",
            c->password[0] ? c->password : "password",
            c->host,
            c->port,
            c->dbname
        );
    }

    return true;
}
