/*
 * Author: Zuzanna Ortenburger
 *
 * Functions associated with control port in sender. Resending music, receiving lookup messages, sending replies.
 */
#ifndef MIMUW_SIK_RADIO_CONTROLER_H
#define MIMUW_SIK_RADIO_CONTROLER_H

#include <ctype.h>
#include "err.h"
#include "common.h"
#include "fifo.h"
#include "data.h"
#include "vector.h"


void resend(uint8_t *buffer, struct rexmit_data *data) {
    for (size_t i = 0; i < data->toAdd->size; i++) {
        uint64_t first_byte_num = data->toAdd->data[i];
        uint8_t *data_to_send = find_fifo(data->fifo, first_byte_num);
        if (data_to_send == NULL) continue;
        memcpy(buffer + 2 * sizeof(uint64_t), data_to_send, data->fifo->PSIZE);
        uint64_t first_byte_num_n = htobe64(first_byte_num);
        memcpy(buffer + sizeof(uint64_t), &first_byte_num_n, sizeof(uint64_t));
        pthread_mutex_lock(&data->socket_mutex);
            send_message(data->socket_fd, buffer, data->fifo->PSIZE + 2 * sizeof(uint64_t), NO_FLAGS);
        pthread_mutex_unlock(&data->socket_mutex);
    }
}

void copy_vector(struct rexmit_data *data) {
    CHECK(pthread_mutex_lock(&data->vector_mutex));
    data->toAdd = data->act;
    data->act = init_vector();
    CHECK(pthread_mutex_unlock(&data->vector_mutex));
}

void *retry(void *arg) {
    struct sender_data *data = (struct sender_data *) arg;
    uint8_t *buffer = calloc(data->PSIZE * sizeof(uint8_t) + 2 * sizeof(uint64_t), 1);
    memcpy(buffer, &data->session_id, sizeof(uint64_t));

    while (data->working) {
        usleep(data->RTIME);
        copy_vector(data->rexmit_data);
        resend(buffer, data->rexmit_data);
        destroy_vector(data->rexmit_data->toAdd);
    }
    return NULL;
}

void add_packet_numbers(char *packetNumbers, struct Vector *vec) {
    char *token = strtok(packetNumbers, ",");
    while (token != NULL) {
        uint64_t number = strtoull(token, NULL, 10);
        add_to_vector(vec, number);
        token = strtok(NULL, ",");
    }
}

bool checkREXMIT(const char *string) {
    char *token;
    char *copy = strdup(string);  // Tworzenie kopii łańcucha
    int element_count = 0;

    token = strtok(copy, ",");
    while (token != NULL) {
        element_count++;
        for (size_t i = 0; i < strlen(token); i++) {
            if (!isdigit(token[i])) {
                free(copy);
                return false;
            }
        }
        token = strtok(NULL, ",");
    }

    free(copy);
    return element_count >= 1;
}

void
process_packet(int socket_fd, char *packet, struct sockaddr_in *client_addr, struct sender_data *data) {
    if (strncmp(packet, LOOKUP, look_len) == 0) {
        send_message_to(socket_fd, client_addr, (uint8_t *) data->reply_msg, data->reply_msg_len);
    } else if (strncmp(packet, REXMIT, rexmit_len) == 0 && checkREXMIT(packet + rexmit_len)) {
        CHECK(pthread_mutex_lock(&data->rexmit_data->vector_mutex));
        add_packet_numbers(packet + rexmit_len, data->rexmit_data->act);
        CHECK(pthread_mutex_unlock(&data->rexmit_data->vector_mutex));
    }
}

void cleanup_thread(void *arg) {
    pthread_t thread = (pthread_t) arg;
    pthread_join(thread, NULL);
}

void *control(void *arg) {
    struct sender_data *data = (struct sender_data *) arg;
    struct sockaddr_in client_address;
    char *reading_buffer = malloc(MAX_UDP_SIZE);
    int ctrl_socket_fd = open_udp_socket();

    struct ip_mreq ip_mreq;
    ip_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (inet_aton(data->MCAST_ADDR, &ip_mreq.imr_multiaddr) == 0) {
        fatal("inet_aton - invalid multicast address\n");
    }

    set_port_reuse(ctrl_socket_fd);
    bind_socket(ctrl_socket_fd, data->CTRL_PORT);

    pthread_t retry_thread;
    CHECK_ERRNO(pthread_create(&retry_thread, 0, retry, (void *) data));

    pthread_cleanup_push(cleanup_thread, (void *) retry_thread) ;
            while (data->working) {
                memset(reading_buffer, 0, MAX_UDP_SIZE);
                receive_message(ctrl_socket_fd, &client_address, (uint8_t *) reading_buffer, MAX_UDP_SIZE);
                process_packet(ctrl_socket_fd, reading_buffer, &client_address, data);
            }
    pthread_cleanup_pop(0);
    free(reading_buffer);
    return NULL;
}

#endif //MIMUW_SIK_RADIO_CONTROLER_H
