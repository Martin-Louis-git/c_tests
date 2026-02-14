#include "ringbuffer.h"

#include <stdarg.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_rb_init_null_rb(void** state){
    (void) state;
    int storage[10];
    assert_false(rb_init(NULL, storage, 10));
}

static void test_rb_init_null_storage(void** state){
    (void) state;
    RingBuffer rb;
    int* storage = NULL;
    assert_false(rb_init(&rb, storage, 10));
}

static void test_rb_init_null_capacity(void** state){
    (void) state;
    RingBuffer rb;
    int storage[10];
    assert_false(rb_init(&rb, storage, 0));
}

static void test_rb_init_valid(void** state){
    (void) state;
    RingBuffer rb;
    int storage[10];
    assert_true(rb_init(&rb, storage, 10));
    assert_ptr_equal(rb.data, storage);
    assert_int_equal(rb.capacity, 10);
    assert_int_equal(rb.count, 0);
    assert_int_equal(rb.head, 0);
    assert_int_equal(rb.tail, 0);
}

static void test_rb_push_null_rb(void** state){
    (void) state;
    assert_false(rb_push(NULL, 42));
}

static void test_rb_push_null_data(void** state){
    (void) state;
    RingBuffer rb = { .data = NULL, .capacity = 10, .count = 0, .head = 0, .tail = 0 };
    assert_false(rb_push(&rb, 42));
}

static void test_rb_push_full(void** state){
    (void) state;
    RingBuffer rb;
    int storage[2];
    assert_true(rb_init(&rb, storage, 2));
    assert_true(rb_push(&rb, 1));
    assert_true(rb_push(&rb, 2));
    assert_false(rb_push(&rb, 3));
}

static void test_rb_push_valid(void** state){
    (void) state;
    RingBuffer rb;
    int storage[2];
    assert_true(rb_init(&rb, storage, 2));
    assert_true(rb_push(&rb, 1));
    assert_true(rb_push(&rb, 2));
    assert_int_equal(storage[0], 1);
    assert_int_equal(storage[1], 2);
}

static void test_rb_push_wrap_around(void** state){
    (void) state;
    RingBuffer rb;
    int storage[2];
    assert_true(rb_init(&rb, storage, 2));
    assert_true(rb_push(&rb, 1));
    assert_true(rb_push(&rb, 2));
    int out;
    assert_true(rb_pop(&rb, &out));
    assert_int_equal(out, 1);
    assert_true(rb_push(&rb, 3));
    assert_int_equal(storage[0], 3);
}

static void test_rb_pop_null_rb(void** state){
    (void) state;
    int out;
    assert_false(rb_pop(NULL, &out));
}

static void test_rb_pop_null_data(void** state){
    (void) state;
    RingBuffer rb = { .data = NULL, .capacity = 10, .count = 0, .head = 0, .tail = 0 };
    int out;
    assert_false(rb_pop(&rb, &out));
}

static void test_rb_pop_empty(void** state){
    (void) state;
    RingBuffer rb;
    int storage[10];
    assert_true(rb_init(&rb, storage, 10));
    int out;
    assert_false(rb_pop(&rb, &out));
}

static void test_rb_pop_null_out(void** state){
    (void) state;
    RingBuffer rb;
    int storage[10];
    assert_true(rb_init(&rb, storage, 10));
    assert_true(rb_push(&rb, 42));
    assert_false(rb_pop(&rb, NULL));
}

static void test_rb_pop_valid(void** state){
    (void) state;
    RingBuffer rb;
    int storage[10];
    assert_true(rb_init(&rb, storage, 10));
    assert_true(rb_push(&rb, 42));
    int out;
    assert_true(rb_pop(&rb, &out));
    assert_int_equal(out, 42);
}

static void test_rb_pop_wrap_around(void** state){
    (void) state;
    RingBuffer rb;
    int storage[2];
    assert_true(rb_init(&rb, storage, 2));
    assert_true(rb_push(&rb, 1));
    assert_true(rb_push(&rb, 2));
    int out;
    assert_true(rb_pop(&rb, &out));
    assert_int_equal(out, 1);
    assert_true(rb_pop(&rb, &out));
    assert_int_equal(out, 2);
    assert_true(rb_push(&rb, 3));
    assert_true(rb_push(&rb, 4));
    assert_true(rb_pop(&rb, &out));
    assert_int_equal(out, 3);
    assert_true(rb_push(&rb, 5));
    assert_true(rb_pop(&rb, &out));
    assert_int_equal(out, 4);
}

static void test_rb_size_null_rb(void** state){
    (void) state;
    assert_int_equal(rb_size(NULL), 0);
}

static void test_rb_size_valid(void** state){
    (void) state;
    RingBuffer rb;
    int storage[10];
    assert_true(rb_init(&rb, storage, 10));
    assert_int_equal(rb_size(&rb), 0);
    assert_true(rb_push(&rb, 1));
    assert_int_equal(rb_size(&rb), 1);
    assert_true(rb_push(&rb, 2));
    assert_int_equal(rb_size(&rb), 2);
    int out;
    assert_true(rb_pop(&rb, &out));
    assert_int_equal(out, 1);
    assert_int_equal(rb_size(&rb), 1);
}

static void test_rb_size_wrap_around(void** state){
    (void) state;
    RingBuffer rb;
    int storage[2];
    assert_true(rb_init(&rb, storage, 2));
    assert_int_equal(rb_size(&rb), 0);
    assert_true(rb_push(&rb, 1));
    assert_true(rb_push(&rb, 2));
    assert_int_equal(rb_size(&rb), 2);
    int out;
    assert_true(rb_pop(&rb, &out));
    assert_int_equal(out, 1);
    assert_int_equal(rb_size(&rb), 1);
    assert_true(rb_push(&rb, 3));
    assert_int_equal(rb_size(&rb), 2);
}

int main(void){
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_rb_init_null_rb),
        cmocka_unit_test(test_rb_init_null_storage),
        cmocka_unit_test(test_rb_init_null_capacity),
        cmocka_unit_test(test_rb_init_valid),
        cmocka_unit_test(test_rb_push_null_rb),
        cmocka_unit_test(test_rb_push_null_data),
        cmocka_unit_test(test_rb_push_full),
        cmocka_unit_test(test_rb_push_valid),
        cmocka_unit_test(test_rb_push_wrap_around),
        cmocka_unit_test(test_rb_pop_null_rb),
        cmocka_unit_test(test_rb_pop_null_data),
        cmocka_unit_test(test_rb_pop_empty),
        cmocka_unit_test(test_rb_pop_null_out),
        cmocka_unit_test(test_rb_pop_valid),
        cmocka_unit_test(test_rb_pop_wrap_around),
        cmocka_unit_test(test_rb_size_null_rb),
        cmocka_unit_test(test_rb_size_valid),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}