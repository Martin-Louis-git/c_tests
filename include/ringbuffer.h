#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct RingBuffer {
    int* data;
    size_t capacity;
    size_t count;
    size_t head;
    size_t tail;
} RingBuffer;

bool rb_init(RingBuffer* rb, int* storage, size_t capacity);
bool rb_push(RingBuffer* rb, int value);
bool rb_pop(RingBuffer* rb, int* out);
size_t rb_size(const RingBuffer* rb);