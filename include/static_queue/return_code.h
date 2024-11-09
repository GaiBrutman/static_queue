#pragma once

typedef enum {
    STATIC_QUEUE_RET_SUCCESS = 0,
    STATIC_QUEUE_RET_INVALID_ARGS,
    STATIC_QUEUE_RET_FULL,
    STATIC_QUEUE_RET_EMPTY,

    STATIC_QUEUE_RET_MAX,
} static_queue_ret_t;
