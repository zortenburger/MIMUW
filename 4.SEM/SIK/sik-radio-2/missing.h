/*
 * Author: Zuzanna Ortenburger
 *
 * Implementation of a fifo storing missing packets working with multiple threads.
 */

#ifndef MIMUW_SIK_RADIO_MISSING_H
#define MIMUW_SIK_RADIO_MISSING_H

#include "err.h"
#include "common.h"


struct Node {
    uint64_t *data;
    uint64_t size;
    uint64_t time_mod;
    struct Node *next;
    int placeholder;
};

struct Missing {
    struct Node *front;
    struct Node *rear;
    uint64_t size;
    pthread_mutex_t mutex;
};

int is_empty(struct Missing *missing) {
    return (missing->front == NULL);
}

struct Node *deMissing(struct Missing *missing) {
    CHECK(pthread_mutex_lock(&missing->mutex));
    if (is_empty(missing)) {
        CHECK(pthread_mutex_unlock(&missing->mutex));
        return NULL;
    }
    missing->size--;
    struct Node *node = missing->front;
    missing->front = missing->front->next;
    if (missing->front == NULL) {
        missing->rear = NULL;
    }
    CHECK(pthread_mutex_unlock(&missing->mutex));
    return node;
}

struct Node *new_node(uint64_t *data, size_t size, uint64_t time_mod, int placeholder) {
    struct Node *newNode = malloc(sizeof(struct Node));
    newNode->size = size;
    newNode->data = malloc(size * sizeof(uint64_t));
    memcpy(newNode->data, data, size * sizeof(uint64_t));
    newNode->time_mod = time_mod;
    newNode->next = NULL;
    newNode->placeholder = placeholder;
    return newNode;
}


void enMissing(struct Missing *missing, uint64_t *data, size_t size, uint64_t time_mod, int placeholder) {
    CHECK(pthread_mutex_lock(&missing->mutex));
    struct Node *newNode = new_node(data, size, time_mod, placeholder);
    if (is_empty(missing)) {
        missing->front = newNode;
        missing->rear = newNode;
    } else {
        missing->rear->next = newNode;
        missing->rear = newNode;
    }
    missing->size++;
    CHECK(pthread_mutex_unlock(&missing->mutex));
}

struct Missing *init_missing() {
    struct Missing *missing = malloc(sizeof(struct Missing));
    missing->front = NULL;
    missing->rear = NULL;
    missing->size = 0;
    CHECK(pthread_mutex_init(&missing->mutex, NULL));
    enMissing(missing, NULL, 0, 0, 1);
    return missing;
}

void free_missing(struct Missing *missing) {
    struct Node *node = missing->front;
    while (node != NULL) {
        struct Node *next = node->next;
        free(node->data);
        free(node);
        node = next;
    }
    CHECK(pthread_mutex_destroy(&missing->mutex));
    free(missing);
}


#endif //MIMUW_SIK_RADIO_MISSING_H
