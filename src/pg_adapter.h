#ifndef PG_H
#define PG_H

#include "types.h"
#include "db_adapter.h"

/**
 * postgres adapter operator all the functions will be 
 * accessed by this instance. */ 
extern const adapter_ops pg_adapter_ops;

/** 
 * LATER THIS COMMENT NEEDS TO CHANGE
 *
 * statement_stat defines the statistics of a database statement. 
 * It contains the user ID and database ID, a unique query ID, the query text, 
 * and detailed metrics for planning, execution, block usage (shared, local, temp), 
 * as well as I/O timings for reads and writes. */
typedef struct {
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
} pg_stat;


/* PostgreSQL connection context */
typedef struct {
    void *pg_conn;  /* PGconn* */
    void *pg_result; /* PGresult* for current query */
    char last_error[256];
} pg_ctx_t;

const adapter_ops* get_pg_adapter_ops(void);

#endif // PG_H

