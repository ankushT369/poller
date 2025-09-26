#ifndef HARVESTER_H
#define HARVESTER_H

#include <stdint.h>


typedef enum {
    IN_QUERY,
    IN_WAIT,
    IN_CONNECT,
    IN_DISCONNECT,
} state;

typedef struct {
    state st;
    uint64_t poll_cnt;
} hvst;

#endif // HARVESTER_H
