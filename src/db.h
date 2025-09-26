#ifndef DB_H
#define DB_H

#include <stddef.h>

#include "harvester.h"

#define MAX_CONN 1000


typedef struct statement_stat stmt_st;
typedef struct db_conn db_conn;

/** 
 * conn_pool defines a pool of database connections. 
 * It contains the total number of connections and an array of 
 * pointers to individual db_conn instances. */
typedef struct {
    size_t total_conn;
    db_conn* conn[MAX_CONN];
} conn_pool;


/** 
 * db_conn defines a single database connection. 
 * It contains the connection ID, host, port, ref 
 * to an associated statement handle and a ref to a harvester obj. */
struct db_conn {
    uint32_t id;
    const char* host;
    int32_t port;

    stmt_st* st;
    hvst* hv;
};


/** 
 * statement_stat defines the statistics of a database statement. 
 * It contains the user ID and database ID, a unique query ID, the query text, 
 * and detailed metrics for planning, execution, block usage (shared, local, temp), 
 * as well as I/O timings for reads and writes. */
struct statement_stat {
    uint32_t userid;       // OID of the user who ran the statement
    uint32_t dbid;         // OID of the database where the statement ran
    int64_t queryid;       // Unique hash derived from the parse tree
    char *query;           // Representative SQL statement text

    /* Planning statistics */
    int64_t plans;
    double total_plan_time;
    double min_plan_time;
    double max_plan_time;
    double mean_plan_time;
    double stddev_plan_time;

    /* Execution statistics */
    int64_t calls;
    double total_time;
    double min_time;
    double max_time;
    double mean_time;
    double stddev_time;
    int64_t rows;

    /* Shared block usage */
    int64_t shared_blks_hit;
    int64_t shared_blks_read;
    int64_t shared_blks_dirtied;
    int64_t shared_blks_written;

    /* Local block usage */
    int64_t local_blks_hit;
    int64_t local_blks_read;
    int64_t local_blks_dirtied;
    int64_t local_blks_written;

    /* Temporary block usage */
    int64_t temp_blks_read;
    int64_t temp_blks_written;

    /* Block I/O timings */
    double blk_read_time;
    double blk_write_time;
};


#endif // DB_H
