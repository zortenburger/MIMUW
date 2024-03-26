/*
 * Author: Zuzanna Ortenburger
 *
 * Implementation of a circular fifo structure working with multiple threads.
 */

#ifndef MIMUW_SIK_RADIO_FIFO_H
#define MIMUW_SIK_RADIO_FIFO_H

#include "err.h"
#include "common.h"

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

struct Packet {
    uint8_t *data;
    uint64_t first_byte_num;
};

struct Queue {
    struct Packet **Packets;
    uint64_t start;
    uint64_t end;
    uint64_t count;
    uint64_t FSIZE;
    uint16_t PSIZE;
    uint64_t max_count;
    pthread_mutex_t lock;
};

struct Queue *create_fifo(uint64_t FSIZE, uint16_t PSIZE) {
    struct Queue *q = malloc(sizeof(struct Queue));
    q->FSIZE = FSIZE;
    q->PSIZE = PSIZE;
    q->max_count = FSIZE / PSIZE;
    q->Packets = malloc(q->FSIZE * sizeof(struct Packet *));
    q->start = 0;
    q->end = 0;
    q->count = 0;
    CHECK(pthread_mutex_init(&q->lock, NULL));
    return q;
}

void delete_fifo(struct Queue *q) {
    for (uint64_t i = 0; i < q->count; i++) {
        free(q->Packets[(q->start + i) % q->max_count]->data);
        free(q->Packets[(q->start + i) % q->max_count]);
    }
    free(q->Packets);
    CHECK(pthread_mutex_destroy(&q->lock));
    free(q);
}

void push_fifo(struct Queue *q, uint8_t *data, uint64_t first_byte_num) {
    CHECK(pthread_mutex_lock(&q->lock));

    if (q->count + 1 == q->max_count) {
        free(q->Packets[q->start]->data);
        free(q->Packets[q->start]);
        q->start = (q->start + 1) % q->max_count;
        q->count--;
    }

    struct Packet *p = malloc(sizeof(struct Packet));
    p->data = malloc(q->PSIZE);
    memcpy(p->data, data, q->PSIZE);
    p->first_byte_num = first_byte_num;
    q->Packets[q->end] = p;
    q->end = (q->end + 1) % q->max_count;
    q->count++;

    CHECK(pthread_mutex_unlock(&q->lock));
}

uint8_t *find_fifo(struct Queue *q, uint64_t first_byte_num) {
    CHECK(pthread_mutex_lock(&q->lock));
    if(first_byte_num >= q->Packets[q->start]->first_byte_num && first_byte_num <= q->Packets[(q->end+q->max_count-1)%q->max_count]->first_byte_num ){
    } else {
        pthread_mutex_unlock(&q->lock);
        return NULL;
    }

    uint64_t col = (first_byte_num - q->Packets[q->start]->first_byte_num) / q->PSIZE;
    uint64_t index = (q->start +  col) % q->max_count;
    // Check if the index is within the bounds of the current queue contents
        CHECK(pthread_mutex_unlock(&q->lock));
        return q->Packets[index]->data;
}

#endif //MIMUW_SIK_RADIO_FIFO_H
