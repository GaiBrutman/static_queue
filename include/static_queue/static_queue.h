#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "static_queue/return_code.h"
#include "static_queue/ring_buffer.h"

/**
 * @brief A static FIFO queue implementation. This queue is backed by a ring
 * buffer.
 */
typedef struct static_queue_s {
    ring_buffer_t ring_buffer;
    size_t node_size;
} static_queue_t;

/**
 * @brief Initialize a static queue. This function must be called before using
 * the queue.
 *
 * @param queue The queue to initialize.
 * @param buffer The buffer to use for the queue. This buffer should be at least
 * `capacity` * `node_size` bytes.
 * @param buffer_size The size of the buffer.
 * @param node_size The size of each node in the queue. Must not change after
 * initialization.
 * @param capacity The capacity of the queue. Pushing more than `capacity` nodes
 * will result in an error.
 * @return static_queue_ret_t `STATIC_QUEUE_RET_SUCCESS` if the queue was
 * successfully initialized. `STATIC_QUEUE_RET_INVALID_ARGS` if received invalid
 * arguments.
 *
 * @note See `ring_buffer_init` for more possible return codes.
 */
static_queue_ret_t static_queue_init(static_queue_t* queue,
                                     uint8_t* buffer,
                                     size_t buffer_size,
                                     size_t node_size,
                                     size_t capacity);

/**
 * @brief Push a node into the queue.
 *
 * @param queue The queue to push the node into.
 * @param data The data to push into the queue. This data must be of size
 * `node_size`.
 * @return static_queue_ret_t `STATIC_QUEUE_RET_SUCCESS` if the node was
 * successfully pushed into the queue. `STATIC_QUEUE_RET_INVALID_ARGS` if
 * received invalid arguments. `STATIC_QUEUE_RET_FULL` if the queue is full.
 *
 * @note See `ring_buffer_push` for more possible return codes.
 */
static_queue_ret_t static_queue_push(static_queue_t* queue, const void* data);

/**
 * @brief Pop a node from the queue.
 *
 * @param queue The queue to pop the node from.
 * @param data The data to pop from the queue. This data must be of size
 * `node_size`.
 * @return static_queue_ret_t `STATIC_QUEUE_RET_SUCCESS` if the node was
 * successfully popped from the queue. `STATIC_QUEUE_RET_INVALID_ARGS` if
 * received invalid arguments. `STATIC_QUEUE_RET_EMPTY` if the queue is empty.
 *
 * @note See `ring_buffer_pop` for more possible return codes.
 */
static_queue_ret_t static_queue_pop(static_queue_t* queue, void* data);
