#pragma once

#include <stddef.h>
#include <stdint.h>

#include "static_queue/return_code.h"

/**
 * @brief A ring buffer structure. This is a FIFO data structure that uses a
 * fixed-size buffer.
 */
typedef struct ring_buffer_s {
    uint8_t* buffer;

    size_t length;
    size_t capacity;

    // Read from buffer index
    size_t start;
    // Write to buffer index
    size_t end;
} ring_buffer_t;

/**
 * @brief Initializes a ring buffer. This function must be called before using
 * the ring buffer.
 *
 * @param ring_buffer The ring buffer to initialize.
 * @param data The whole buffer to use for the ring buffer.
 * @param data_size The size of the buffer. `buffer` memory should be valid for
 * at least `data_size` bytes.
 * @return static_queue_ret_t `STATIC_QUEUE_RET_SUCCESS` if the ring buffer was
 * successfully initialized.
 *                            `STATIC_QUEUE_RET_INVALID_ARGS` if received
 * invalid arguments.
 */
static_queue_ret_t ring_buffer_init(ring_buffer_t* ring_buffer,
                                    uint8_t* data,
                                    size_t data_size);
/**
 * @brief Push data into the ring buffer. This function will copy data_size
 * bytes from data into the ring buffer.
 *
 * @param ring_buffer The ring buffer to push data into.
 * @param data The data to push into the ring buffer.
 * @param data_size The size of the data to push.
 * @return static_queue_ret_t `STATIC_QUEUE_RET_SUCCESS` if the data was
 * successfully pushed into the ring buffer.
 *                            `STATIC_QUEUE_RET_INVALID_ARGS` if received
 * invalid arguments. `STATIC_QUEUE_RET_FULL` if the ring buffer is full.
 */
static_queue_ret_t ring_buffer_push(ring_buffer_t* ring_buffer,
                                    const void* data,
                                    size_t data_size);

/**
 * @brief Pop data from the ring buffer. This function will copy data_size bytes
 * from the ring buffer into data.
 *
 * @param ring_buffer The ring buffer to pop data from.
 * @param data The data to pop from the ring buffer.
 * @param data_size The size of the data to pop.
 * @return static_queue_ret_t `STATIC_QUEUE_RET_SUCCESS` if the data was
 * successfully popped from the ring buffer.
 *                            `STATIC_QUEUE_RET_INVALID_ARGS` if received
 * invalid arguments.
 *                            `STATIC_QUEUE_RET_EMPTY` if the ring buffer is
 * empty.
 */
static_queue_ret_t ring_buffer_pop(ring_buffer_t* ring_buffer,
                                   void* data,
                                   size_t data_size);
