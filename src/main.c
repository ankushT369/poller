/* THIS IS A TEST FILE */
#include <stdio.h>
#include "db_adapter.h"

#define MAX_DB 1000

#ifdef __cplusplus
extern "C" {
#endif

size_t count_dbs_config_yaml(const char* filename);
int parse_db_config_yaml(const char* filename, db_conn* db_array, int max_dbs);

#ifdef __cplusplus
}
#endif

int main() {
    size_t nums_dbs = count_dbs_config_yaml("db.yaml");
    db_conn conn[nums_dbs];
    size_t count = parse_db_config_yaml("db.yaml", conn, MAX_DB);

    for (int i = 0; i < count; i++) {
        /*
                debug print statements 
                printf("==== DB %d ====\n", i+1);
                printf("Name: %s\n", conn[i].name);
                printf("Adapter: %s\n", conn[i].adapter_name);
                printf("Host: %s\n", conn[i].host);
                printf("Port: %d\n", conn[i].port);
                printf("DB Name: %s\n", conn[i].dbname);
                printf("User: %s\n", conn[i].user);
                printf("Password: %s\n", conn[i].password[0] ? "<hidden>" : "<not set>");
                printf("Retry: %d attempts, interval %ds\n",
                       conn[i].policy.max_attempts,
                       conn[i].policy.interval_seconds);
        */
    }

    return 0;
}
