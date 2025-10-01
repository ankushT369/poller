#include <stdio.h>

#include "log_conf.h"
#include "db_adapter.h"


int log_init(const char* confpath, log_t* lg) {
    if(zlog_init(confpath)) {
        fprintf(stderr, "Failed to initialize zlog\n");
        return 0;
    }


    lg->postgres = zlog_get_category(db_map[0]);
    if(!lg->postgres) {
        fprintf(stderr, "postgres get catagory failed\n");
        zlog_fini();
        return 0;
    }

    lg->mysql = zlog_get_category(db_map[1]);
    if (!lg->mysql) {
        fprintf(stderr, "mysql get catagory failed\n");
        zlog_fini();
        return 0;
    }

    lg->mongodb = zlog_get_category(db_map[2]);
    if (!lg->mongodb) {
        fprintf(stderr, "mongodb get catagory failed\n");
        zlog_fini();
        return 0;
    }

    lg->system = zlog_get_category(SYSTEM);
    if (!lg->system) {
        fprintf(stderr, "system get catagory failed\n");
        zlog_fini();
        return 0;
    }

    return 1;
}
