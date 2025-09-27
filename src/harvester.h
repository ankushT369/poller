#ifndef HARVESTER_H
#define HARVESTER_H

#include "types.h"

/**
 * state enum defines the current state of a connection. */
typedef enum {
    IN_QUERY,
    IN_WAIT,
    IN_DISCONNECT,
} state;

/**
 */
typedef struct {
    state st;
    uint64_t poll_cnt;
} hvst;

#endif // HARVESTER_H
