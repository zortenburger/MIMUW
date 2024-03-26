/*
 * Author: Zuzanna Ortenburger
 *
 * Implementation of a vector.
 */

#ifndef MIMUW_SIK_RADIO_VECTOR_H
#define MIMUW_SIK_RADIO_VECTOR_H

#include "err.h"
#include "common.h"

struct Vector {
    uint64_t *data;
    size_t size;
    size_t capacity;
};

struct Vector *init_vector() {
    struct Vector *vector = malloc(sizeof(struct Vector));
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    return vector;
}

void destroy_vector(struct Vector *vector) {
    free(vector->data);
    free(vector);
}

void add_to_vector(struct Vector *vector, uint64_t element) {
    if (vector->size == vector->capacity) {
        if (vector->capacity == 0) {
            vector->capacity = 1;
        } else {
            vector->capacity *= 2;
        }
        vector->data = (uint64_t *) realloc(vector->data, vector->capacity * sizeof(uint64_t));
    }

    vector->data[vector->size] = element;
    vector->size++;
}

int get_size(struct Vector *vector) {
    return vector->size;
}

int compare(const void *a, const void *b) {
    int num1 = *(int *) a;
    int num2 = *(int *) b;

    if (num1 < num2) {
        return -1;
    } else if (num1 > num2) {
        return 1;
    } else {
        return 0;
    }
}

void sort_vector(struct Vector *vector) {
    qsort(vector->data, vector->size, sizeof(int), compare);
}

#endif //MIMUW_SIK_RADIO_VECTOR_H
