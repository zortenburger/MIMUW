/*
 * Author: Zuzanna Ortenburger
 *
 * Functions sending lookup messages and controlling active radios.
 */

#ifndef MIMUW_SIK_RADIO_LOOKUPER_H
#define MIMUW_SIK_RADIO_LOOKUPER_H

#include "err.h"
#include "common.h"
#include "radio.h"
#include "data.h"
#include "sikradio-listener.h"
#include "sikradio-ui.h"
#include "sikradio-switch.h"

struct Radio *choose_radio(struct RadioList *radio_list, struct Receiver_data *data) {
    struct Radio *pom = NULL;
    if (data->NAME_INPUT != NULL) {
        pom = findRadio(radio_list, data->NAME_INPUT, NULL, 0);
        if (pom != NULL) {
            return pom;
        } else {
            return NULL;
        }
    }
    if (radio_list->head != NULL) {
        return radio_list->head;
    } else {
        return NULL;
    }
}


void change_current(struct Receiver_data *data) {
    pthread_mutex_lock(&data->radio_mutex);
    if (data->active_radios->current == NULL && data->active_radios->turned_on) {
        data->active_radios->turned_on = false;
        stop_current(data);
    }
    if (data->active_radios->current == NULL) {
        data->active_radios->current = choose_radio(data->active_radios, data);
        if (data->active_radios->current != NULL) {
            start_current(data);
        }
    }
    refresh_screen(data);
    pthread_mutex_unlock(&data->radio_mutex);
}


struct lookup_data {
    int socket_fd;
    struct Receiver_data *receiver_data;
};

void *send_lookup(void *arg) {
    struct lookup_data *data = (struct lookup_data *) arg;
    struct sockaddr_in discover_address = get_send_address(data->receiver_data->DISCOVER_ADDR,
                                                           data->receiver_data->CTRL_PORT);
    while (true) {
        send_message_to(data->socket_fd, &discover_address, (uint8_t *) data->receiver_data->lookup_msg, look_len);
        if (remove_old_radios(data->receiver_data->active_radios, (uint64_t) time(NULL))) {
            change_current(data->receiver_data);
        }
        sleep(LOOKUP_TIME);
    }
}


bool handle_response(char *message, struct Receiver_data *data, uint64_t series, struct sockaddr_in *client_addr) {
    // Rozdzielenie pierwszej części
    char *token = strtok(message, " ");
    if(strcmp(token, REPLY) != 0) {
        return false;
    }

    // Rozdzielenie drugiej części
    token = strtok(NULL, " ");
    char *ipAddress = token;
    // Rozdzielenie trzeciej części
    token = strtok(NULL, " ");
    char *port_c = token;

    // Rozdzielenie czwartej części
    token = strtok(NULL, "");
    char *name = token;


    if(!is_valid_ip(ipAddress)) {
        return false;
    }
    if(!is_valid_port(port_c)) {
        return false;
    }
    if(!is_valid_name(name)) {
        return false;
    }
    uint16_t port = read_port(port_c);


    if (addRadio(data->active_radios, name, ipAddress, port, series, client_addr)) {
        change_current(data);
    }
    return true;
}

void receive_reply(int socket_fd, struct Receiver_data *data) {
    char *buffer = calloc(MAX_UDP_SIZE, 1);
    while (true) {
        memset(buffer, 0, MAX_UDP_SIZE);
        struct sockaddr_in client_address;
        receive_message(socket_fd, &client_address, (uint8_t *) buffer, MAX_UDP_SIZE);
        handle_response(buffer, data, (uint64_t) time(NULL), &client_address);
    }
}

void *lookup(void *arg) {
    struct Receiver_data *data = (struct Receiver_data *) arg;
    int socket_fd = open_udp_socket();
    /* uaktywnienie rozgłaszania (ang. broadcast) */
    int optval = 1;
    CHECK_ERRNO(setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, (void *) &optval, sizeof optval));
    /* ustawienie TTL dla datagramów rozsyłanych do grupy */
    optval = TTL_VALUE;
    CHECK_ERRNO(setsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &optval, sizeof optval));

    struct lookup_data *lookup_data = malloc(sizeof(struct lookup_data));
    lookup_data->socket_fd = socket_fd;
    lookup_data->receiver_data = data;
    pthread_t send_thread;
    CHECK(pthread_create(&send_thread, NULL, send_lookup, (void *) lookup_data));
    receive_reply(socket_fd, data);
    free(lookup_data);
    return NULL;
}

#endif //MIMUW_SIK_RADIO_LOOKUPER_H
