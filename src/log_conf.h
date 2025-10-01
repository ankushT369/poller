#ifndef LOG_CONF_H
#define LOG_CONF_H

#include <zlog.h>

#define SYSTEM "system"

typedef struct {
    zlog_category_t* postgres;
    zlog_category_t* mysql;
    zlog_category_t* mongodb;
    zlog_category_t* system;
} log_t;


int log_init(const char* confpath, log_t* lg);


#endif // LOG_CONF_H 
