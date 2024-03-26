/*
 * Source: MIMUW Concurrent Programming lesson scenario (Laboratorium 9 - biblioteka pthread)
 * Modified by: Zuzanna Ortenburger
 *
 * Implementation of a circular buffer structure working with multiple threads.
 */

#ifndef MIMUW_SIK_RADIO_BUFFER_H
#define MIMUW_SIK_RADIO_BUFFER_H


#include "err.h"
#include "common.h"


struct Buffer {
    uint8_t *buffer; // A circular buffer of integers.
    uint64_t read_pos, write_pos; // Positions in buffer for reading and for writing (begin and end of buffer).
    pthread_mutex_t mutex; // Mutex for exclusive access to buffer.
    pthread_cond_t not_empty; // Signaled when buffer is not empty.
    uint64_t session_id;    // ID of the current session.
    uint64_t byte0;       // Number of the first byte received in the current session.
    uint64_t PSIZE;     // Size of a single packet.
    uint64_t size;    // Size of data in buffer.
    bool playing;   // True if the playing thread is running.
    bool *missing; // Array of missing packets.
};

// Initializes a buffer.
void buffer_init(struct Buffer *b, uint64_t BSIZE, uint64_t session_id, uint64_t byte0, uint64_t PSIZE) {
    b->buffer = calloc(BSIZE, sizeof(uint8_t));
    if(b->buffer == NULL) {
        fatal("calloc");
    }
    CHECK(pthread_mutex_init(&b->mutex, NULL));
    CHECK(pthread_cond_init(&b->not_empty, NULL));
    b->session_id = session_id;
    b->byte0 = byte0;
    b->PSIZE = PSIZE;
    b->playing = false; // The playing thread is not running yet.
    b->size = (BSIZE / PSIZE) * PSIZE;
    b->missing = calloc(b->size / PSIZE, sizeof(bool));
    b->read_pos = byte0 % b->size;
    b->write_pos = b->read_pos;

}

// Restarts a buffer.
void buffer_restart(struct Buffer *b, uint64_t BSIZE, uint64_t session_id, uint64_t byte0, uint64_t PSIZE) {
    memset(b->buffer, 0, BSIZE);
    b->session_id = session_id;
    b->byte0 = byte0;
    b->PSIZE = PSIZE;
    b->playing = false; // The playing thread is not running yet.
    b->size = (BSIZE / PSIZE) * PSIZE;
    b->missing = realloc(b->missing, b->size / PSIZE * sizeof(bool));
    memset(b->missing, 0, b->size / PSIZE);
    b->read_pos = byte0 % b->size;
    b->write_pos = b->read_pos;

}

// Ends the current session and destroys the buffer.
void buffer_destroy(struct Buffer *b) {
    free(b->buffer);
    free(b->missing);
    CHECK(pthread_cond_destroy(&b->not_empty));
    CHECK(pthread_mutex_destroy(&b->mutex));
}

// Prints all the missing packets in the buffer that are before the last received packet.
void print_missing(struct Buffer *b, uint64_t last_received) {
    uint64_t pop = 0;
    if (b->write_pos > b->size) {
        pop = b->write_pos - b->size;
    }
    uint64_t oldest_byte = MAX(b->byte0, pop); // The oldest byte in the buffer.

    for (; oldest_byte < last_received; oldest_byte += b->PSIZE) {
        uint64_t poz = oldest_byte % b->size;
        if (b->missing[poz / b->PSIZE]) {
            fprintf(stderr, "MISSING: BEFORE %ld EXPECTED %ld\n", last_received, oldest_byte);
        }
    }
}

// Puts data in the buffer.
void buffer_put(struct Buffer *b, uint8_t *data, uint64_t first_byte_num) {

    CHECK(pthread_mutex_lock(&b->mutex));
    bool was_empty = false;

    if (first_byte_num < b->write_pos && first_byte_num >= b->read_pos) { // It's the packet that was lost.
        memcpy(b->buffer + (first_byte_num % b->size), data, b->PSIZE); // Put the packet in the right place.
    } else {
        was_empty = (b->read_pos == b->write_pos);
        while (b->write_pos < first_byte_num) { // Fill the buffer with zeros until the first byte of the packet and mark the packets as missing.
            memset(b->buffer + (b->write_pos % b->size), 0, b->PSIZE);
            b->missing[(b->write_pos % b->size) / b->PSIZE] = true;
            b->write_pos += b->PSIZE;
        }

        memcpy(b->buffer + (b->write_pos % b->size), data, b->PSIZE);
        b->write_pos += b->PSIZE;
    }

    b->missing[(first_byte_num % b->size) / b->PSIZE] = false;

    print_missing(b, first_byte_num);

    if(b->write_pos > b->read_pos + b->size) { // If the buffer is full, remove the oldest packet.
        b->read_pos = b->write_pos - b->size;
    }

    if (was_empty)
        CHECK(pthread_cond_signal(&b->not_empty));

    CHECK(pthread_mutex_unlock(&b->mutex));
}

// Prints PSIZE of data from the buffer.
bool buffer_get(struct Buffer *b) {
    CHECK(pthread_mutex_lock(&b->mutex));

    if (!b->playing) {
        CHECK(pthread_mutex_unlock(&b->mutex));
        return false;
    }

    while (b->write_pos == b->read_pos) {
        CHECK(pthread_cond_wait(&b->not_empty, &b->mutex));
        if (!b->playing) {
            CHECK(pthread_mutex_unlock(&b->mutex));
            return false;
        }
    }

    fwrite(b->buffer + (b->read_pos % b->size), 1, b->PSIZE, stdout);

    b->read_pos += b->PSIZE;

    CHECK(pthread_mutex_unlock(&b->mutex));

    return true;
}

// Ends the playing thread.
void buffer_end(struct Buffer *b) {
    CHECK(pthread_mutex_lock(&b->mutex));
    b->playing = false;
    if (b->read_pos == b->write_pos) {
        CHECK(pthread_cond_signal(&b->not_empty));
    }
    CHECK(pthread_mutex_unlock(&b->mutex));
}


#endif //MIMUW_SIK_RADIO_BUFFER_H
