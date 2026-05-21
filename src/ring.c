#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "ring.h"

ring_t* ring_init(size_t capacity) {

    ring_t* ring = (ring_t*)malloc(sizeof(ring_t));
    ring->write_index = 0; // the slot we will write to next (when write is called, CURRENTLY EMPTY)
    ring->read_index = 0; // the slot we will consume (when read is called)
    ring->capacity = capacity;
    

    pthread_mutex_init(&ring->lock, NULL);
    pthread_cond_init(&ring->data_cond, NULL);

    for (size_t i = 0; i < capacity; i++) {
        ring->buffer[i] = -1;
    }

    return ring;

}

int ring_read(ring_t* buf) {

    pthread_mutex_lock(&buf->lock);

    while (buf->read_index == buf->write_index) { // no data to read (the consuming slot = the empty slot)
        pthread_cond_wait(&buf->data_cond, &buf->lock);
    }

    int data = buf->buffer[buf->read_index];
    buf->read_index = (buf->read_index + 1) % buf->capacity;


    pthread_mutex_unlock(&buf->lock);

    return data;

}

void ring_write(ring_t* buf, int val) {

    pthread_mutex_lock(&buf->lock);


    buf->buffer[buf->write_index] = val; // buffer[empty slot] = data;

    size_t next_write_index = (buf->write_index + 1) % buf->capacity; // empty slot ++

    if (next_write_index == buf->read_index) { // if empty slot = consuming slot (the next slot we will write to is the next slot we will read from, we need to advance the reader)
        buf->read_index = (buf->read_index + 1) % buf->capacity;
    }


    buf->write_index = next_write_index;

    pthread_cond_signal(&buf->data_cond);

    pthread_mutex_unlock(&buf->lock);


}

void ring_peek(ring_t* buf) {

    pthread_mutex_lock(&buf->lock);

    for (size_t i = 0; i < buf->capacity; i++) {

        printf("%d\t", buf->buffer[i]);

    }


    printf("\n");

    // for (size_t i = 0; i < buf->capacity; i++) {

    //     if (i == buf->write_index) {
    //         printf("%c\t", 'H');
    //     } else {
    //         printf("\t");
    //     }


    // }

    // printf("\n");

    // for (size_t i = 0; i < buf->capacity; i++) {


    //     if (i == buf->read_index) {
    //         printf("%c", 'T');
    //     }

    //     printf("\t");

    // }

    printf("%s: %zu, %s %zu\n", "Writer @", buf->write_index, "Reader @", buf->read_index);

    printf("\n\n\n\n");

    pthread_mutex_unlock(&buf->lock);

}

void ring_destroy(ring_t* buf) {

    free(buf);

}