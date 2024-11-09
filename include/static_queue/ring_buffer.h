#pragma once

#include <stddef.h>
#include <stdint.h>

#include "static_queue/return_code.h"

typedef struct ring_buffer_s {
    uint8_t* buffer;

    size_t length;
    size_t capacity;

    // Read from buffer index
    size_t start;
    // Write to buffer index
    size_t end;
} ring_buffer_t;

static_queue_ret_t ring_buffer_init(ring_buffer_t* ring_buffer,
                                    uint8_t* data,
                                    size_t data_size,
                                    size_t capacity);

static_queue_ret_t ring_buffer_push(ring_buffer_t* ring_buffer,
                                    const void* data,
                                    size_t data_size);

static_queue_ret_t ring_buffer_pop(ring_buffer_t* ring_buffer,
                                   void* data,
                                   size_t data_size);
