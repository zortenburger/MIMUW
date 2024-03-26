/*
 * Author: Zuzanna Ortenburger
 *
 * Functions for listening to music.
 */

#ifndef MIMUW_SIK_RADIO_LISTENER_H
#define MIMUW_SIK_RADIO_LISTENER_H

#include "err.h"
#include "common.h"
#include "missing.h"
#include "buffer.h"
#include "data.h"
#include "sikradio-player.h"
#include "sikradio-rexmiter.h"

void stop_playing(struct Listen_data *data) {
    buffer_end(data->buffer);
    CHECK_ERRNO(pthread_join(data->playing_thread, NULL));
    CHECK_ERRNO(pthread_join(data->rexmit_thread, NULL));
}

void cleanup_function(void *arg) {
    struct Listen_data *data = (struct Listen_data *) arg;
    stop_playing(data);
    buffer_destroy(data->buffer);
    free_missing(data->missing);
    free(data);
}

void *listen_music(void *arg) {
    struct Receiver_data *data = arg;
    data->socket_fd = open_udp_socket();

    pthread_mutex_lock(&data->radio_mutex);
    struct ip_mreq ip_mreq;
    /* podłączenie do grupy rozsyłania (ang. multicast) */
    ip_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (inet_aton(data->active_radios->current->address, &ip_mreq.imr_multiaddr) == 0) {
        fatal("inet_aton - invalid multicast address\n");
    }
    CHECK_ERRNO(setsockopt(data->socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &ip_mreq, sizeof ip_mreq));
    /* ustawienie adresu i portu lokalnego */
    bind_socket(data->socket_fd, data->active_radios->current->port);


    int read_length = 0;
    uint64_t session_id = 0;
    uint64_t first_byte_num = 0;
    uint8_t *reading_buffer = malloc(data->BSIZE);

    struct Listen_data *my_data = malloc(sizeof(struct Listen_data));
    my_data->playing_thread = 0;
    my_data->rexmit_thread = 0;
    my_data->RTIME = data->RTIME;
    my_data->buffer = buffer_init(data->BSIZE, 0, 0, 1, data->RTIME);
    my_data->missing = init_missing();
    my_data->socket_fd = open_udp_socket();
    my_data->addr = data->active_radios->current->addr;
    pthread_mutex_unlock(&data->radio_mutex);

    pthread_cleanup_push(cleanup_function, my_data) ;

            while (true) {
                memset(reading_buffer, 0, data->BSIZE);
                read_length = receive_message(data->socket_fd, NULL, reading_buffer,
                                              data->BSIZE + 2 * sizeof(uint64_t));
                if (read_length == -1) {
                    break;
                }

                memcpy(&session_id, reading_buffer, sizeof(uint64_t));
                session_id = be64toh(session_id);

                memcpy(&first_byte_num, reading_buffer + sizeof(uint64_t), sizeof(uint64_t));
                first_byte_num = be64toh(first_byte_num);
                if (session_id < my_data->buffer->session_id) { // If we received old message, ignore it
                    continue;
                } else if (session_id > my_data->buffer->session_id) { // If we received new message, start new session
                    stop_playing(my_data);
                    buffer_restart(my_data->buffer, data->BSIZE, session_id, first_byte_num,
                                   read_length - 2 * sizeof(uint64_t));
                }
                buffer_put(my_data->buffer, reading_buffer + 2 * sizeof(uint64_t), first_byte_num,
                                my_data->missing);

                if (first_byte_num >= my_data->buffer->byte0 + 3 * data->BSIZE / 4 &&
                    !my_data->buffer->playing) { // If we have enough bytes to start playing, start playing
                    my_data->buffer->playing = true;
                    CHECK_ERRNO(pthread_create(&my_data->playing_thread, 0, play_music, (void *) my_data->buffer));
                    pthread_create(&my_data->rexmit_thread, NULL, rexmit, my_data);
                }
            }
    pthread_cleanup_pop(1);
    return NULL;
}

#endif //MIMUW_SIK_RADIO_LISTENER_H
