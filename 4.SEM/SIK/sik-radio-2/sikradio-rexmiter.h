//
// Created by zuzia on 29.05.23.
//

#ifndef RADIO_DUZE_SIKRADIO_REXMITER_H
#define RADIO_DUZE_SIKRADIO_REXMITER_H

#include "err.h"
#include "common.h"
#include "missing.h"
#include "data.h"
#include "buffer.h"


char *get_rexmit_message(const uint64_t *data, const size_t size, const size_t oldest) {
    // Obliczamy długość całego rezultującego stringa
    size_t total_length = rexmit_len + size * 21; // 20 znaków na liczbę + 1 na spację

    // Alokujemy pamięć dla wynikowego stringa
    char *result = (char *) malloc((total_length + 1) * sizeof(char)); // +1 na znak końca stringa

    // Sklejamy stringa
    strcpy(result, REXMIT);
    for (size_t i = 0; i < size; i++) {
        char num_str[21]; // Bufor na konwersję liczby do stringa
        if (data[i] < oldest) continue;
        if (i == size - 1)
            snprintf(num_str, sizeof(num_str), "%lu", data[i]);
        else
            snprintf(num_str, sizeof(num_str), "%lu,", data[i]);
        strcat(result, num_str);
    }
    return result;
}

void *rexmit(void *arg) {
    struct Listen_data *data = (struct Listen_data *) arg;

    while (true) {
        CHECK(pthread_mutex_lock(&data->buffer->mutex));
        if (!data->buffer->playing) {
            CHECK(pthread_mutex_unlock(&data->buffer->mutex));
            break;
        }
        CHECK(pthread_mutex_unlock(&data->buffer->mutex));
        struct Node *node = deMissing(data->missing);
        usleep((node->time_mod - time(NULL) * 1000 + data->RTIME) % data->RTIME);
        if (node->placeholder == 1) {
            enMissing(data->missing, NULL, 0, node->time_mod, 1);
            continue;
        }
        uint64_t pop = 0;
        if (data->buffer->write_pos > data->buffer->size) {
            pop = data->buffer->write_pos - data->buffer->size;
        }
        uint64_t oldest_byte = MAX(data->buffer->byte0, pop);
        char *message = get_rexmit_message(node->data, node->size, oldest_byte);
        if(strcmp(message, REXMIT) == 0) {
            continue;
        }
        send_message_to(data->socket_fd, data->addr,(uint8_t *) message, strlen(message));
        enMissing(data->missing, node->data, node->size, node->time_mod, 0);
        free(message);
        free(node->data);
        free(node);
    }
    return NULL;
}

#endif //RADIO_DUZE_SIKRADIO_REXMITER_H
