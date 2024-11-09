#include <string.h>
#include <unity.h>

#include "static_queue/return_code.h"
#include "static_queue/static_queue.h"

struct node {
    int data;
};

#define NODE_SIZE (sizeof(struct node))
#define CAPACITY (5)

void test_static_queue_init_sanity(void) {
    static_queue_t queue;
    uint8_t buffer[CAPACITY * NODE_SIZE];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_init(&queue, (uint8_t*)buffer,
                                        sizeof(buffer), NODE_SIZE, CAPACITY));
    TEST_ASSERT_EQUAL_size_t(0, queue.ring_buffer.length);
    TEST_ASSERT_EQUAL_size_t(CAPACITY * NODE_SIZE, queue.ring_buffer.capacity);
}

void test_static_queue_init_invalid_params(void) {
    // Fill the queue with some data to make sure it's not modified
    static_queue_t queue = {
        .node_size = 1,
        .ring_buffer =
            {
                .buffer = (void*)0xdeadbeef,
                .length = 2,
                .capacity = 3,
                .start = 4,
                .end = 5,
            },
    };
    // Fill the buffer with some data to make sure it's not modified
    uint8_t buffer[CAPACITY * NODE_SIZE] = {1, 2, 3, 4, 5};
    static_queue_t expected_queue = {0};
    uint8_t expected_buffer[CAPACITY * NODE_SIZE] = {0};

    memcpy(&expected_queue, &queue, sizeof(queue));
    memcpy(&expected_buffer, &buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_INVALID_ARGS,
                      static_queue_init(NULL, (uint8_t*)buffer, sizeof(buffer),
                                        NODE_SIZE, CAPACITY));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_queue, &queue, sizeof(queue));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_buffer, &buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(
        STATIC_QUEUE_RET_INVALID_ARGS,
        static_queue_init(&queue, NULL, sizeof(buffer), NODE_SIZE, CAPACITY));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_queue, &queue, sizeof(queue));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_buffer, &buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(
        STATIC_QUEUE_RET_INVALID_ARGS,
        static_queue_init(&queue, (uint8_t*)buffer, 0, NODE_SIZE, CAPACITY));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_queue, &queue, sizeof(queue));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_buffer, &buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_INVALID_ARGS,
                      static_queue_init(&queue, (uint8_t*)buffer,
                                        sizeof(buffer), 0, CAPACITY));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_queue, &queue, sizeof(queue));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_buffer, &buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_INVALID_ARGS,
                      static_queue_init(&queue, (uint8_t*)buffer,
                                        sizeof(buffer), NODE_SIZE, 0));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_queue, &queue, sizeof(queue));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_buffer, &buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(
        STATIC_QUEUE_RET_INVALID_ARGS,
        static_queue_init(&queue, (uint8_t*)buffer, sizeof(buffer) - 1,
                          NODE_SIZE, CAPACITY));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_queue, &queue, sizeof(queue));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_buffer, &buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(
        STATIC_QUEUE_RET_INVALID_ARGS,
        static_queue_init(&queue, (uint8_t*)buffer, sizeof(buffer), NODE_SIZE,
                          CAPACITY + 1));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_queue, &queue, sizeof(queue));
    TEST_ASSERT_EQUAL_INT8_ARRAY(&expected_buffer, &buffer, sizeof(buffer));
}

void test_static_queue_push_sanity(void) {
    static_queue_t queue;
    uint8_t buffer[CAPACITY * NODE_SIZE];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_init(&queue, (uint8_t*)buffer,
                                        sizeof(buffer), NODE_SIZE, CAPACITY));

    struct node data[CAPACITY] = {{1}, {2}, {3}, {4}, {5}};

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_push(&queue, &data[0]));
    TEST_ASSERT_EQUAL_size_t(NODE_SIZE, queue.ring_buffer.length);
}

void test_static_queue_push_invalid_params(void) {
    static_queue_t queue;
    uint8_t buffer[CAPACITY * NODE_SIZE];
    struct node node = {1};

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_init(&queue, (uint8_t*)buffer,
                                        sizeof(buffer), NODE_SIZE, CAPACITY));

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_INVALID_ARGS,
                      static_queue_push(NULL, NULL));
    TEST_ASSERT_EQUAL_size_t(0, queue.ring_buffer.length);
    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_INVALID_ARGS,
                      static_queue_push(&queue, NULL));
    TEST_ASSERT_EQUAL_size_t(0, queue.ring_buffer.length);
    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_INVALID_ARGS,
                      static_queue_push(NULL, &node));
    TEST_ASSERT_EQUAL_size_t(0, queue.ring_buffer.length);
}

void test_static_queue_push_until_full(void) {
    static_queue_t queue;
    uint8_t buffer[CAPACITY * NODE_SIZE];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_init(&queue, (uint8_t*)buffer,
                                        sizeof(buffer), NODE_SIZE, CAPACITY));

    struct node node = {0};
    for (size_t i = 0; i < CAPACITY; i++) {
        node.data = i;
        TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                          static_queue_push(&queue, &node));
        TEST_ASSERT_EQUAL_size_t((i + 1) * NODE_SIZE, queue.ring_buffer.length);
    }

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_FULL, static_queue_push(&queue, &node));
    TEST_ASSERT_EQUAL_size_t(queue.ring_buffer.capacity,
                             queue.ring_buffer.length);
}

void test_static_queue_pop_sanity(void) {
    static_queue_t queue;
    uint8_t buffer[CAPACITY * NODE_SIZE];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_init(&queue, (uint8_t*)buffer,
                                        sizeof(buffer), NODE_SIZE, CAPACITY));

    struct node push_node = {1};
    struct node pop_node = {0};

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_push(&queue, &push_node));
    TEST_ASSERT_EQUAL_size_t(NODE_SIZE, queue.ring_buffer.length);

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_pop(&queue, &pop_node));
    TEST_ASSERT_EQUAL_size_t(0, queue.ring_buffer.length);
    TEST_ASSERT_EQUAL_INT(push_node.data, pop_node.data);
}

void test_static_queue_pop_invalid_params(void) {
    static_queue_t queue;
    uint8_t buffer[CAPACITY * NODE_SIZE];
    struct node node = {1};
    struct node out_node = {2};

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_init(&queue, (uint8_t*)buffer,
                                        sizeof(buffer), NODE_SIZE, CAPACITY));
    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_push(&queue, &node));

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_INVALID_ARGS,
                      static_queue_pop(NULL, NULL));
    TEST_ASSERT_EQUAL_size_t(NODE_SIZE, queue.ring_buffer.length);
    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_INVALID_ARGS,
                      static_queue_push(&queue, NULL));
    TEST_ASSERT_EQUAL_size_t(NODE_SIZE, queue.ring_buffer.length);
    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_INVALID_ARGS,
                      static_queue_push(NULL, &out_node));
    TEST_ASSERT_EQUAL_size_t(NODE_SIZE, queue.ring_buffer.length);
    TEST_ASSERT_EQUAL_INT(2, out_node.data);
}

void test_static_queue_pop_until_empty(void) {
    static_queue_t queue;
    uint8_t buffer[CAPACITY * NODE_SIZE];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_init(&queue, (uint8_t*)buffer,
                                        sizeof(buffer), NODE_SIZE, CAPACITY));

    struct node node = {0};
    for (size_t i = 0; i < CAPACITY; i++) {
        node.data = i;
        TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                          static_queue_push(&queue, &node));
        TEST_ASSERT_EQUAL_size_t((i + 1) * NODE_SIZE, queue.ring_buffer.length);
    }

    for (size_t i = 0; i < CAPACITY; i++) {
        TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                          static_queue_pop(&queue, &node));
        TEST_ASSERT_EQUAL(node.data, i);
        TEST_ASSERT_EQUAL_size_t((CAPACITY - i - 1) * NODE_SIZE,
                                 queue.ring_buffer.length);
    }

    TEST_ASSERT_EQUAL_size_t(0, queue.ring_buffer.length);
    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_EMPTY, static_queue_pop(&queue, &node));
    TEST_ASSERT_EQUAL_size_t(0, queue.ring_buffer.length);
}

void test_static_queue_data_overlap(void) {
    static_queue_t queue;
    uint8_t buffer[CAPACITY * NODE_SIZE];

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_init(&queue, (uint8_t*)buffer,
                                        sizeof(buffer), NODE_SIZE, CAPACITY));

    struct node push_data[3] = {{1}, {2}, {3}};

    for (size_t i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                          static_queue_push(&queue, &push_data[i]));
        TEST_ASSERT_EQUAL_size_t((i + 1) * NODE_SIZE, queue.ring_buffer.length);
    }

    struct node pop_data[2] = {{0}};
    for (size_t i = 0; i < 2; i++) {
        TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                          static_queue_pop(&queue, &pop_data[i]));
    }
    TEST_ASSERT_EQUAL_size_t(NODE_SIZE, queue.ring_buffer.length);
    TEST_ASSERT_EQUAL_INT_ARRAY(push_data, pop_data,
                                sizeof(pop_data) / sizeof(pop_data[0]));

    struct node unique_data = {6};
    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                      static_queue_push(&queue, &unique_data));
    TEST_ASSERT_EQUAL_size_t(2 * NODE_SIZE, queue.ring_buffer.length);

    struct node expected_data_left[2] = {{3}, {6}};

    for (size_t i = 0; i < 2; i++) {
        TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_SUCCESS,
                          static_queue_pop(&queue, &pop_data[i]));
    }
    TEST_ASSERT_EQUAL_size_t(0, queue.ring_buffer.length);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected_data_left, pop_data,
                                sizeof(pop_data) / sizeof(pop_data[0]));

    TEST_ASSERT_EQUAL(STATIC_QUEUE_RET_EMPTY,
                      static_queue_pop(&queue, &pop_data[0]));
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UnityBegin(__FILE__);

    RUN_TEST(test_static_queue_init_sanity);
    RUN_TEST(test_static_queue_init_invalid_params);
    RUN_TEST(test_static_queue_push_sanity);
    RUN_TEST(test_static_queue_push_invalid_params);
    RUN_TEST(test_static_queue_push_until_full);
    RUN_TEST(test_static_queue_pop_sanity);
    RUN_TEST(test_static_queue_pop_invalid_params);
    RUN_TEST(test_static_queue_pop_until_empty);
    RUN_TEST(test_static_queue_data_overlap);

    return (UnityEnd());
}
