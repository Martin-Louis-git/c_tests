#include "ringbuffer.h"

bool rb_init(RingBuffer* rb, int* storage, size_t capacity) {
    if(rb == NULL || storage == NULL || capacity == 0) {
        return false;
    }
    rb->data = storage;
    rb->capacity = capacity;
    rb->count = 0;
    rb->head = 0;
    rb->tail = 0;
    return true;
}

bool rb_push(RingBuffer* rb, int value){
    if(!rb || !rb->data || rb->count == rb->capacity) {
        return false;
    }
    rb->data[rb->head] = value;
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count++;
    return true;
}

bool rb_pop(RingBuffer* rb, int* out){
    if(!rb || !rb->data || rb->count == 0 || out == NULL) {
        return false;
    }
    *out = rb->data[rb->tail];
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count--;
    return true;
}

size_t rb_size(const RingBuffer* rb){
    if(rb == NULL) {
        return 0;
    }
    return rb->count;
}