#include <unity.h>

#include "static_queue/return_code.h"
#include "static_queue/ring_buffer.h"

#define CAPACITY (5)

void test_ring_buffer_init_sanity(void) {
    ring_buffer_t ring_buffer;
    uint8_t buffer[CAPACITY];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      ring_buffer_init(&ring_buffer, buffer, CAPACITY));
    TEST_ASSERT_EQUAL_size_t(0, ring_buffer.length);
    TEST_ASSERT_EQUAL_size_t(CAPACITY, ring_buffer.capacity);
}

void test_ring_buffer_push_sanity(void) {
    ring_buffer_t ring_buffer;
    uint8_t buffer[CAPACITY];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      ring_buffer_init(&ring_buffer, buffer, CAPACITY));

    uint8_t data[CAPACITY] = {1, 2, 3, 4, 5};

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      ring_buffer_push(&ring_buffer, data, sizeof(data)));
    TEST_ASSERT_EQUAL_size_t(ring_buffer.capacity, ring_buffer.length);
}

void test_ring_buffer_push_until_full(void) {
    ring_buffer_t ring_buffer;
    uint8_t buffer[CAPACITY];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      ring_buffer_init(&ring_buffer, buffer, CAPACITY));

    uint8_t data = 1;
    for (size_t i = 0; i < CAPACITY; i++) {
        TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                          ring_buffer_push(&ring_buffer, &data, sizeof(data)));
        TEST_ASSERT_EQUAL_size_t(i + 1, ring_buffer.length);
    }

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_FULL,
                      ring_buffer_push(&ring_buffer, &data, sizeof(data)));
    TEST_ASSERT_EQUAL_size_t(ring_buffer.capacity, ring_buffer.length);
}

void test_ring_buffer_pop_sanity(void) {
    ring_buffer_t ring_buffer;
    uint8_t buffer[CAPACITY];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      ring_buffer_init(&ring_buffer, buffer, CAPACITY));

    uint8_t push_data[CAPACITY] = {1, 2, 3, 4, 5};
    uint8_t pop_data[CAPACITY] = {0};
    TEST_ASSERT_EQUAL_size_t(sizeof(push_data), sizeof(pop_data));

    TEST_ASSERT_EQUAL(
        STATIC_QUEUE_RET_SUCCESS,
        ring_buffer_push(&ring_buffer, push_data, sizeof(push_data)));
    TEST_ASSERT_EQUAL_size_t(ring_buffer.capacity, ring_buffer.length);

    TEST_ASSERT_EQUAL(
        STATIC_QUEUE_RET_SUCCESS,
        ring_buffer_pop(&ring_buffer, pop_data, sizeof(pop_data)));

    TEST_ASSERT_EQUAL_size_t(0, ring_buffer.length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(push_data, pop_data, sizeof(push_data));
}

void test_ring_buffer_pop_until_empty(void) {
    ring_buffer_t ring_buffer;
    uint8_t buffer[CAPACITY];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      ring_buffer_init(&ring_buffer, buffer, CAPACITY));

    uint8_t push_data[CAPACITY] = {1, 2, 3, 4, 5};

    TEST_ASSERT_EQUAL(
        STATIC_QUEUE_RET_SUCCESS,
        ring_buffer_push(&ring_buffer, push_data, sizeof(push_data)));
    TEST_ASSERT_EQUAL_size_t(ring_buffer.capacity, ring_buffer.length);

    uint8_t current_data = 0;
    for (size_t i = 0; i < CAPACITY; i++) {
        TEST_ASSERT_EQUAL(
            STATIC_QUEUE_RET_SUCCESS,
            ring_buffer_pop(&ring_buffer, &current_data, sizeof(current_data)));
        TEST_ASSERT_EQUAL(push_data[i], current_data);
        TEST_ASSERT_EQUAL_size_t(CAPACITY - i - 1, ring_buffer.length);
    }

    TEST_ASSERT_EQUAL(
        STATIC_QUEUE_RET_EMPTY,
        ring_buffer_pop(&ring_buffer, &current_data, sizeof(current_data)));
    TEST_ASSERT_EQUAL_size_t(0, ring_buffer.length);
}

void test_ring_buffer_data_overlap(void) {
    ring_buffer_t ring_buffer;
    uint8_t buffer[CAPACITY];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      ring_buffer_init(&ring_buffer, buffer, CAPACITY));

    uint8_t push_data[3] = {1, 2, 3};

    // Push some data
    for (size_t i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                          ring_buffer_push(&ring_buffer, &push_data[i],
                                           sizeof(push_data[i])));
        TEST_ASSERT_EQUAL_size_t(i + 1, ring_buffer.length);
    }

    // Pop some data without emptying the buffer
    uint8_t pop_data[2] = {0};
    TEST_ASSERT_EQUAL(
        STATIC_QUEUE_RET_SUCCESS,
        ring_buffer_pop(&ring_buffer, pop_data, sizeof(pop_data)));
    TEST_ASSERT_EQUAL_size_t(1, ring_buffer.length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(push_data, pop_data, sizeof(pop_data));

    // Push some more data
    uint8_t unique_data = 6;
    TEST_ASSERT_EQUAL(
        STATIC_QUEUE_RET_SUCCESS,
        ring_buffer_push(&ring_buffer, &unique_data, sizeof(unique_data)));
    TEST_ASSERT_EQUAL_size_t(2, ring_buffer.length);

    uint8_t expected_data_left[2] = {3, 6};

    // Pop the rest of the data
    TEST_ASSERT_EQUAL(
        STATIC_QUEUE_RET_SUCCESS,
        ring_buffer_pop(&ring_buffer, pop_data, sizeof(pop_data)));
    TEST_ASSERT_EQUAL_size_t(0, ring_buffer.length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data_left, pop_data,
                                  sizeof(pop_data));
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UnityBegin(__FILE__);

    // Add tests here:
    RUN_TEST(test_ring_buffer_init_sanity);
    RUN_TEST(test_ring_buffer_push_sanity);
    RUN_TEST(test_ring_buffer_push_until_full);
    RUN_TEST(test_ring_buffer_pop_sanity);
    RUN_TEST(test_ring_buffer_pop_until_empty);
    RUN_TEST(test_ring_buffer_data_overlap);

    return (UnityEnd());
}
