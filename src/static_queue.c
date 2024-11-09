#include <string.h>

#include "static_queue/return_code.h"
#include "static_queue/ring_buffer.h"
#include "static_queue/static_queue.h"

static_queue_ret_t static_queue_init(static_queue_t* queue,
                                     uint8_t* buffer,
                                     size_t buffer_size,
                                     size_t node_size,
                                     size_t capacity) {
    static_queue_ret_t ret = STATIC_QUEUE_RET_SUCCESS;

    if (NULL == queue || NULL == buffer || 0 == node_size || 0 == capacity) {
        return STATIC_QUEUE_RET_INVALID_ARGS;
    }

    if (buffer_size < node_size * capacity) {
        return STATIC_QUEUE_RET_INVALID_ARGS;
    }

    // Use only needed buffer to fill the capacity
    ret = ring_buffer_init(&queue->ring_buffer, buffer, node_size * capacity);
    if (STATIC_QUEUE_RET_SUCCESS != ret) {
        goto out;
    }

    queue->node_size = node_size;

    ret = STATIC_QUEUE_RET_SUCCESS;
out:
    return ret;
}

static_queue_ret_t static_queue_push(static_queue_t* queue, const void* node) {
    if (NULL == queue || NULL == node) {
        return STATIC_QUEUE_RET_INVALID_ARGS;
    }

    return ring_buffer_push(&queue->ring_buffer, node, queue->node_size);
}

static_queue_ret_t static_queue_pop(static_queue_t* queue, void* node) {
    if (NULL == queue || NULL == node) {
        return STATIC_QUEUE_RET_INVALID_ARGS;
    }

    return ring_buffer_pop(&queue->ring_buffer, node, queue->node_size);
}
