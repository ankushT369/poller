/* THIS IS A TEST FILE */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zlog.h>

#include "log_conf.h"
#include "pg_adapter.h"
#include "db_adapter.h"

#define MAX_DB 1000
#define PATH "zlog.conf"


#ifdef __cplusplus
extern "C" {
#endif

size_t count_db_config_yaml(const char* filename);
size_t parse_db_config_yaml(const char* filename, db_conf* db_array);

#ifdef __cplusplus
}
#endif

int main() {
    log_t lg = {0};
    
    /* Initialize logging FIRST - critical for debugging everything else */
    if (!log_init(PATH, &lg)) {
        fprintf(stderr, "CRITICAL: Failed to initialize logging system\n");
        return EXIT_FAILURE;
    }
    zlog_info(lg.system, "Logging system initialized successfully");

    /* Register adapters */
    adapter_register(DB_POSTGRES, get_pg_adapter_ops());

    /* Validate configuration file exists before parsing */
    if (access("db.yaml", F_OK) != 0) {
        zlog_error(lg.system, "Database configuration file 'db.yaml' not found");
        zlog_fini();
        return EXIT_FAILURE;
    }

    /* Count database configurations */
    size_t nums_dbs = count_db_config_yaml("db.yaml");
    if (nums_dbs == 0 || nums_dbs > MAX_DB) {
        zlog_error(lg.system, "Invalid number of databases: %zu", nums_dbs);
        zlog_error(lg.system, "Too many databases (%zu), maximum supported: %d", 
                   nums_dbs, MAX_DB);
        zlog_fini();
        return EXIT_FAILURE;
    }
    zlog_info(lg.system, "Found %zu database configuration(s)", nums_dbs);

    db_conf config[nums_dbs];
    db_conn conn[nums_dbs];
    memset(config, 0, sizeof(config));  // Initialize to avoid garbage data
    memset(conn, 0, sizeof(conn));    // Initialize to avoid garbage data
    
    size_t conn_cnt = parse_db_config_yaml("db.yaml", config);
    if (conn_cnt == 0) {
        zlog_error(lg.system, "Failed to parse any database configurations");
        zlog_fini();
        return EXIT_FAILURE;
    }
    zlog_info(lg.system, "Successfully parsed %zu database connection(s)", conn_cnt);

    /* Validate parsing consistency */
    if (conn_cnt != nums_dbs) {
        zlog_warn(lg.system, "Configuration count mismatch: expected %zu, parsed %zu", 
                  nums_dbs, conn_cnt);
        // Continue with what we have, but log the discrepancy
    }

    for (size_t i = 0; i < conn_cnt; i++) {
        /* Set URIs with validation */
        if (!db_config_set_uri(&config[i])) {
            zlog_error(lg.system, "Failed to set database URIs");
            zlog_fini();
            return EXIT_FAILURE;
        }

        conn[i].config = config[i];

        /* Set adapter configurations */
        const adapter_ops* ops = adapter_get_ops(conn[i].config.adapter);
        if (!ops) {
            zlog_error(lg.system, "Failed to set database adapter operations");
            zlog_fini();
            return EXIT_FAILURE;
        }

        conn[i].ops = ops;
        hv_set_disconnected(&conn[i]);
        hv_poll_cnt_init(&conn[i]);

        if (conn[i].ops->connect(&conn[i]) != 0) {
            if (conn[i].config.adapter == DB_POSTGRES) {
                zlog_error(lg.postgres, "Failed to connect to %s: %s", config[i].name, conn[i].ops->get_error(&conn[i]));
                continue;
            }

            if (conn[i].config.adapter == DB_MYSQL) {
                zlog_error(lg.mysql, "Failed to connect to %s: %s", config[i].name, conn[i].ops->get_error(&conn[i]));
                continue;
            }

            if (conn[i].config.adapter == DB_MONGODB) {
                zlog_error(lg.mongodb, "Failed to connect to %s: %s", config[i].name, conn[i].ops->get_error(&conn[i]));
                continue;
            }
        }

        if (conn[i].config.adapter == DB_POSTGRES) {
            zlog_info(lg.postgres, "PostgreSQL connection established: %s", conn[i].config.name);
        }

        if (conn[i].config.adapter == DB_MYSQL) {
            zlog_info(lg.mysql, "mysql connection established: %s", conn[i].config.name);
        }

        if (conn[i].config.adapter == DB_MONGODB) {
            zlog_info(lg.mongodb, "mongodb connection established: %s", conn[i].config.name);
        }
    }
    
    /* Proper cleanup before exit */
    zlog_fini();
    return EXIT_SUCCESS;
}
