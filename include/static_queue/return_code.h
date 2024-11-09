#pragma once

/**
 * @brief Return codes for the static queue library.
 */
typedef enum {
    STATIC_QUEUE_RET_UNINITIALIZED = -1,
    STATIC_QUEUE_RET_SUCCESS = 0,
    STATIC_QUEUE_RET_INVALID_ARGS,
    STATIC_QUEUE_RET_FULL,
    STATIC_QUEUE_RET_EMPTY,

    STATIC_QUEUE_RET_MAX,
} static_queue_ret_t;

_Static_assert(sizeof(static_queue_ret_t) == sizeof(int),
               "static_queue_ret_t must be the same size as int");
