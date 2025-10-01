/* THIS IS A TEST FILE */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "log_conf.h"
#include "db_adapter.h"

#define MAX_DB 1000
#define PATH "zlog.conf"

extern adapter_ops pg_ops;

#ifdef __cplusplus
extern "C" {
#endif

size_t count_db_config_yaml(const char* filename);
size_t parse_db_config_yaml(const char* filename, db_conn* db_array, int max_dbs);

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

    /* Validate configuration file exists before parsing */
    if (access("db.yaml", F_OK) != 0) {
        zlog_error(lg.system, "Database configuration file 'db.yaml' not found");
        zlog_fini();
        return EXIT_FAILURE;
    }

    /* Count database configurations with error handling */
    size_t nums_dbs = count_db_config_yaml("db.yaml");
    if (nums_dbs == 0) {
        zlog_error(lg.system, "No database configurations found in db.yaml");
        zlog_fini();
        return EXIT_FAILURE;
    }
    zlog_info(lg.system, "Found %zu database configuration(s)", nums_dbs);

    /* Parse configurations with bounds checking */
    if (nums_dbs > MAX_DB) {
        zlog_error(lg.system, "Too many databases (%zu), maximum supported: %d", 
                   nums_dbs, MAX_DB);
        zlog_fini();
        return EXIT_FAILURE;
    }

    db_conn conn[nums_dbs];
    memset(conn, 0, sizeof(conn));  // Initialize to avoid garbage data
    
    size_t conn_cnt = parse_db_config_yaml("db.yaml", conn, MAX_DB);
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

    /* Set URIs with validation */
    if (!set_uri(conn, conn_cnt)) {
        zlog_error(lg.system, "Failed to set database URIs");
        zlog_fini();
        return EXIT_FAILURE;
    }
    zlog_debug(lg.system, "Database URIs configured successfully");

    /* Initialize database operations with proper error context */
    if (pg_ops.init(NULL) != 0) {
        zlog_error(lg.system, "Failed to initialize database operations layer");
        zlog_fini();
        return EXIT_FAILURE;
    }
    zlog_info(lg.system, "Database operations initialized successfully");

    zlog_info(lg.system, "Application startup completed successfully");
    
    /* Proper cleanup before exit */
    zlog_fini();
    return EXIT_SUCCESS;
}
