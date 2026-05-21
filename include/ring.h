#pragma once

#include<pthread.h>
#include <stdbool.h>

#define BUFFER_SIZE 100

typedef struct {

    size_t write_index;
    size_t read_index;
    size_t capacity;
    pthread_mutex_t lock;
    pthread_cond_t data_cond;
    int buffer[BUFFER_SIZE];

} ring_t;

ring_t* ring_init(size_t capacity);

int ring_read(ring_t* buf);

void ring_write(ring_t* buf, int val);

void ring_peek(ring_t* buf);

void ring_destroy(ring_t* buf);