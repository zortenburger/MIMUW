/*
 * Author: Zuzanna Ortenburger
 *
 * Structures with data of sender and receiver with proper functions
 */
#ifndef MIMUW_SIK_RADIO_DATA_H
#define MIMUW_SIK_RADIO_DATA_H

#include "err.h"
#include "common.h"
#include "vector.h"
#include "radio.h"
#include "fifo.h"
#include <poll.h>

//SENDER

struct rexmit_data {
    struct Queue *fifo;
    int socket_fd;
    struct Vector *act;
    struct Vector *toAdd;
    pthread_mutex_t vector_mutex;
    pthread_mutex_t socket_mutex;
};

struct sender_data {
    char *MCAST_ADDR;
    uint16_t DATA_PORT;
    uint16_t CTRL_PORT;
    uint16_t PSIZE;
    uint64_t FSIZE;
    uint64_t RTIME;
    char *NAME;
    char *reply_msg;
    uint16_t reply_msg_len;
    uint64_t session_id;
    bool working;
    struct rexmit_data *rexmit_data;
};

struct sender_data *init_sender_data() {
    struct sender_data *data = calloc(1, sizeof(struct sender_data));
    data->MCAST_ADDR = NULL;
    data->DATA_PORT = DEFAULT_DATA_PORT;
    data->CTRL_PORT = DEFAULT_CTRL_PORT;
    data->PSIZE = DEFAULT_PSIZE;
    data->FSIZE = DEFAULT_FSIZE;
    data->RTIME = DEFAULT_RTIME;
    data->NAME = DEFAULT_NAME;
    data->reply_msg = NULL;
    data->reply_msg_len = 0;
    data->session_id = htobe64((uint64_t)
                                       time(NULL));
    data->working = true;
    data->rexmit_data = NULL;
    return data;
}

void set_reply_msg(struct sender_data *data) {
    data->reply_msg_len = reply_len + strlen(data->MCAST_ADDR) + 8 + strlen(data->NAME);
    data->reply_msg = calloc(data->reply_msg_len + 1, sizeof(char));
    sprintf(data->reply_msg, "%s %s %d %s", REPLY, data->MCAST_ADDR, data->DATA_PORT, data->NAME);
    data->reply_msg_len = strlen(data->reply_msg);
}


void init_connection(struct sender_data *data) {
    data->rexmit_data = calloc(1, sizeof(struct rexmit_data));
    struct rexmit_data *rexmit_data = data->rexmit_data;
    rexmit_data->fifo = create_fifo(data->FSIZE, data->PSIZE);

    rexmit_data->act = init_vector();
    rexmit_data->socket_fd = open_udp_socket();

    int optval = 1;
    CHECK_ERRNO(setsockopt(rexmit_data->socket_fd, SOL_SOCKET, SO_BROADCAST, (void *) &optval, sizeof optval));
    optval = TTL_VALUE;
    CHECK_ERRNO(setsockopt(rexmit_data->socket_fd, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &optval, sizeof optval));

    struct sockaddr_in send_address = get_send_address(data->MCAST_ADDR, data->DATA_PORT);
    connect_socket(rexmit_data->socket_fd, &send_address);
    CHECK(pthread_mutex_init(&rexmit_data->vector_mutex, NULL));
    CHECK(pthread_mutex_init(&rexmit_data->socket_mutex, NULL));
}

void destroy_rexmit(struct rexmit_data *data) {
    delete_fifo(data->fifo);
    destroy_vector(data->act);
    CHECK(close(data->socket_fd));
    CHECK(pthread_mutex_destroy(&data->vector_mutex));
    CHECK(pthread_mutex_destroy(&data->socket_mutex));
    free(data);
}

void turn_off(struct sender_data *data) {
    destroy_rexmit(data->rexmit_data);
    free(data->reply_msg);
}

//RECEIVER
struct Receiver_data {
    char *DISCOVER_ADDR;
    uint16_t CTRL_PORT;
    uint16_t UI_PORT;
    uint64_t RTIME;
    uint64_t BSIZE;
    char *NAME_INPUT;
    char *lookup_msg;
    pthread_mutex_t radio_mutex;
    struct RadioList *active_radios;
    pthread_t listening_thread;
    int socket_fd;
    struct pollfd *poll_descriptors;
};

struct Receiver_data *init_receiver_data() {
    struct Receiver_data *data = calloc(1, sizeof(struct Receiver_data));
    data->DISCOVER_ADDR = DEFAULT_DISCOVER_ADDR;
    data->CTRL_PORT = DEFAULT_CTRL_PORT;
    data->UI_PORT = DEFAULT_UI_PORT;
    data->RTIME = DEFAULT_RTIME;
    data->BSIZE = DEFAULT_BSIZE;
    data->NAME_INPUT = NULL;
    data->lookup_msg = LOOKUP;
    CHECK(pthread_mutex_init(&data->radio_mutex, NULL));
    data->active_radios = initializeRadioList();
    data->listening_thread = 0;
    data->poll_descriptors = malloc(sizeof(struct pollfd) * CONNECTIONS);

    /* Inicjujemy tablicę z gniazdkami klientów, poll_descriptors[0] to gniazdko centrali */
    for (int i = 0; i < CONNECTIONS; ++i) {
        data->poll_descriptors[i].fd = -1;
        data->poll_descriptors[i].events = POLLIN;
        data->poll_descriptors[i].revents = 0;
    }
    return data;
}

void turn_off_receiver(struct Receiver_data *data) {
    CHECK(pthread_mutex_destroy(&data->radio_mutex));
    destroyRadioList(data->active_radios);
    free(data->poll_descriptors);
    free(data->NAME_INPUT);
    free(data);
}


struct Listen_data {
    struct Buffer *buffer;
    pthread_t playing_thread;
    pthread_t rexmit_thread;
    uint64_t RTIME;
    struct Missing *missing;
    int socket_fd;
    struct sockaddr_in *addr;
};


#endif //MIMUW_SIK_RADIO_DATA_H
