/*
 * Author: Zuzanna Ortenburger
 *
 * Implementation of a receiver for the sikradio program.
 */

#include <pthread.h>

#include "err.h"
#include "common.h"
#include "buffer.h"

char *DEST_ADDR = NULL;
uint16_t DATA_PORT = DEFAULT_DATA_PORT;
uint64_t BSIZE = DEFAULT_BSIZE;
int opt;

// Prints bytes from buffer while it's not empty
void *play_music(void *buff) {
    struct Buffer *buffer = buff;
    while (buffer_get(buffer)) {}
    return NULL;
}

// Listens for incoming messages and puts them into buffer
void *listen_music() {
    int socket_fd = bind_socket(DATA_PORT);

    struct sockaddr_in expected_address = get_send_address(DEST_ADDR, 0); // Address we expect to receive from
    struct sockaddr_in client_address;

    size_t read_length = 0;
    uint64_t session_id = 0;
    uint64_t first_byte_num = 0;

    struct Buffer *buffer = malloc(sizeof(struct Buffer));
    buffer_init(buffer, BSIZE, 0, 0, DEFAULT_PSIZE);

    pthread_t playing_thread;

    uint8_t *reading_buffer = malloc(BSIZE);

    while (true) {
        memset(reading_buffer, 0, BSIZE);

        read_length = receive_message(socket_fd, &client_address, reading_buffer, BSIZE + 2 * sizeof(uint64_t));

        if (client_address.sin_addr.s_addr != expected_address.sin_addr.s_addr) { // Check if we received from expected address
            continue;
        }

        memcpy(&session_id, reading_buffer, sizeof(uint64_t));
        session_id = be64toh(session_id);

        memcpy(&first_byte_num, reading_buffer + sizeof(uint64_t), sizeof(uint64_t));
        first_byte_num = be64toh(first_byte_num);

        if (session_id < buffer->session_id) { // If we received old message, ignore it
            continue;
        } else if (session_id > buffer->session_id) { // If we received new message, start new session
            //zacznij od nowa
            buffer_end(buffer);
            CHECK_ERRNO(pthread_join(playing_thread, NULL));
            buffer_restart(buffer, BSIZE, session_id, first_byte_num, read_length - 2 * sizeof(uint64_t));
            buffer_put(buffer, reading_buffer + 2 * sizeof(uint64_t), first_byte_num);
        } else { // If we received message from current session, put it into buffer
            buffer_put(buffer, reading_buffer + 2 * sizeof(uint64_t), first_byte_num);
        }

        if (first_byte_num >= buffer->byte0 + 3 * BSIZE / 4 && !buffer->playing) { // If we have enough bytes to start playing, start playing
            //zacznij grac
            buffer->playing = true;
            CHECK_ERRNO(pthread_create(&playing_thread, 0, play_music, (void *) buffer));
        }
    }

    CHECK_ERRNO(pthread_join(playing_thread, NULL));
    buffer_destroy(buffer);
    CHECK_ERRNO(close(socket_fd));

    return NULL;
}


int main(int argc, char *argv[]) {
    while ((opt = getopt(argc, argv, "a:P:b:")) != -1) {
        switch (opt) {
            case 'a':
                DEST_ADDR = optarg;
                break;
            case 'P':
                DATA_PORT = read_port(optarg);
                break;
            case 'b':
                BSIZE = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-P DATA_PORT] [-b BSIZE] [-p PSIZE]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (DEST_ADDR == NULL) {
        fprintf(stderr, "Error: DEST_ADDR (-a) is required\n");
        exit(EXIT_FAILURE);
    }

    pthread_t listening_thread;
    CHECK_ERRNO(pthread_create(&listening_thread, 0, listen_music, NULL));
    CHECK_ERRNO(pthread_join(listening_thread, NULL));

    return 0;
}
