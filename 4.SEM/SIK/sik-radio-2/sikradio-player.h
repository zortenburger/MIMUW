/*
 * Author: Zuzanna Ortenburger
 *
 * Functions  playing music from buffer.
 */

#ifndef MIMUW_SIK_RADIO_PLAYER_H
#define MIMUW_SIK_RADIO_PLAYER_H

#include "err.h"
#include "common.h"
#include "buffer.h"

// Prints bytes from buffer while it's not empty
void *play_music(void *buff) {
    struct Buffer *buffer = buff;
    while (buffer_get(buffer)) {}
    return NULL;
}

#endif //MIMUW_SIK_RADIO_PLAYER_H
