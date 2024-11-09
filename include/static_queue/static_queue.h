#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "static_queue/return_code.h"
#include "static_queue/ring_buffer.h"

typedef struct static_queue_s {
    ring_buffer_t ring_buffer;
    size_t node_size;
} static_queue_t;

static_queue_ret_t static_queue_init(static_queue_t* queue,
                                     uint8_t* buffer,
                                     size_t buffer_size,
                                     size_t node_size,
                                     size_t capacity);

static_queue_ret_t static_queue_push(static_queue_t* queue, const void* data);

static_queue_ret_t static_queue_pop(static_queue_t* queue, void* data);
