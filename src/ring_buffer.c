#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "static_queue/return_code.h"
#include "static_queue/ring_buffer.h"

static_queue_ret_t ring_buffer_init(ring_buffer_t* ring_buffer,
                                    uint8_t* data,
                                    size_t data_size,
                                    size_t capacity) {
    if (NULL == ring_buffer || NULL == data || 0 == data_size ||
        0 == capacity) {
        return STATIC_QUEUE_RET_INVALID_ARGS;
    }

    if (data_size < capacity) {
        return STATIC_QUEUE_RET_INVALID_ARGS;
    }

    ring_buffer->capacity = capacity;
    ring_buffer->buffer = data;
    ring_buffer->length = 0;
    ring_buffer->start = 0;
    ring_buffer->end = 0;

    return STATIC_QUEUE_RET_SUCCESS;
}

static_queue_ret_t ring_buffer_push(ring_buffer_t* ring_buffer,
                                    const void* data,
                                    size_t data_size) {
    if (NULL == ring_buffer || NULL == data || 0 == data_size) {
        return STATIC_QUEUE_RET_INVALID_ARGS;
    }

    if (data_size > ring_buffer->capacity) {
        return STATIC_QUEUE_RET_INVALID_ARGS;
    }

    // Check if the buffer is full (length + data_size > capacity)
    if (ring_buffer->length > ring_buffer->capacity - data_size) {
        return STATIC_QUEUE_RET_FULL;
    }

    memcpy(ring_buffer->buffer + ring_buffer->end, data, data_size);

    ring_buffer->end = (ring_buffer->end + data_size) % ring_buffer->capacity;
    ring_buffer->length += data_size;
    return STATIC_QUEUE_RET_SUCCESS;
}

static_queue_ret_t ring_buffer_pop(ring_buffer_t* ring_buffer,
                                   void* data,
                                   size_t data_size) {
    if (NULL == ring_buffer || NULL == data || 0 == data_size) {
        return STATIC_QUEUE_RET_INVALID_ARGS;
    }

    if (data_size > ring_buffer->capacity) {
        return STATIC_QUEUE_RET_INVALID_ARGS;
    }

    // Check if not enough data in the buffer
    if (ring_buffer->length < data_size) {
        return STATIC_QUEUE_RET_EMPTY;
    }

    memcpy(data, ring_buffer->buffer + ring_buffer->start, data_size);

    ring_buffer->start =
        (ring_buffer->start + data_size) % ring_buffer->capacity;
    ring_buffer->length -= data_size;
    return STATIC_QUEUE_RET_SUCCESS;
}
