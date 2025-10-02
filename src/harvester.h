#ifndef HARVESTER_H
#define HARVESTER_H

#include "types.h"

/**
 * state enum defines the current state of a connection. */
typedef enum {
    CONN_DISCONNECTED,
    CONN_CONNECTING,
    CONN_READY,
    CONN_QUERYING,
    CONN_ERROR,
} state;


/**
 */
typedef struct {
    state st;
    uint64_t poll_cnt;
} hvst;



#endif // HARVESTER_H
