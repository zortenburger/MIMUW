/*
 * Author: Zuzanna Ortenburger
 *
 * Changing current radio.
 */

#ifndef MIMUW_SIK_RADIO_SWITCH_H
#define MIMUW_SIK_RADIO_SWITCH_H

#include "err.h"
#include "common.h"
#include "radio.h"
#include "data.h"
#include "sikradio-listener.h"

void stop_current(struct Receiver_data *data) {
    if (data->listening_thread != 0) {
        pthread_cancel(data->listening_thread);
        pthread_join(data->listening_thread, NULL);
        close(data->socket_fd);
        data->listening_thread = 0;
    }
}

void start_current(struct Receiver_data *data) {
    if (data->listening_thread == 0) {
        data->active_radios->turned_on = true;
        CHECK(pthread_create(&data->listening_thread, NULL, listen_music, (void *) data));
    }
}

#endif //MIMUW_SIK_RADIO_SWITCH_H
