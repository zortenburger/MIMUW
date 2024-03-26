/*
 * Author: Zuzanna Ortenburger
 *
 * Implementation of a sender for the sikradio program.
 */

#include <time.h>

#include "err.h"
#include "common.h"
#include "fifo.h"
#include "sikradio-controler.h"
#include "data.h"


void parse_input(int argcc, char **argv, struct sender_data *data) {
    int opt;
    while ((opt = getopt(argcc, argv, "a:P:C:p:f:n:R:")) != -1) {
        switch (opt) {
            case 'a':
                check_address(optarg);
                is_multicast_address(optarg);
                data->MCAST_ADDR = optarg;
                break;
            case 'P':
                data->DATA_PORT = read_port(optarg);
                break;
            case 'C':
                data->CTRL_PORT = read_port(optarg);
                if (data->CTRL_PORT == data->DATA_PORT) {
                    fprintf(stderr, "Error: CTRL_PORT and DATA_PORT cannot be the same\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'p':
                data->PSIZE = read_psize(optarg);
                break;
            case 'f':
                data->FSIZE = read_number(optarg, 0);
                break;
            case 'n':
                check_name(optarg);
                data->NAME = optarg;
                break;
            case 'R':
                data->RTIME = read_number(optarg, 1);
                break;
            default:
                fprintf(stderr,
                        "Usage: %s -a MCAST_ADDR [-P DATA_PORT] [-C CTRL_PORT] [-p PSIZE] [-f FSIZE] [-n NAZWA] [-R RTIME]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (data->MCAST_ADDR == NULL) {
        fprintf(stderr, "Error: MCAST_ADDR (-a) is required\n");
        exit(EXIT_FAILURE);
    }
    set_reply_msg(data);
}


void *send_music(void *arg) {

    pthread_t ctrl_thread;
    CHECK(pthread_create(&ctrl_thread, 0, control, arg));

    struct sender_data *data = (struct sender_data *) arg;
    struct Queue *fifo = data->rexmit_data->fifo;
    int socket_fd = data->rexmit_data->socket_fd;
    size_t size = data->PSIZE * sizeof(uint8_t) + 2 * sizeof(uint64_t);
    uint64_t first_byte_num = 0;
    uint8_t *buffer = malloc(size);
    memset(buffer, 0, size);
    memcpy(buffer, &data->session_id, sizeof(uint64_t));

    size_t nread;
    while ((nread = fread(buffer + 2 * sizeof(uint64_t), 1, data->PSIZE, stdin)) == data->PSIZE) {
        uint64_t first_byte_num_n = htobe64(first_byte_num);
        memcpy(buffer + sizeof(uint64_t), &first_byte_num_n, sizeof(uint64_t));
        push_fifo(fifo, buffer + 2 * sizeof(uint64_t), first_byte_num);
        CHECK(pthread_mutex_lock(&data->rexmit_data->socket_mutex));
        send_message(socket_fd, buffer, size, NO_FLAGS);
        CHECK(pthread_mutex_unlock(&data->rexmit_data->socket_mutex));
        first_byte_num += data->PSIZE;
    }
    data->working = false;
    free(buffer);
    CHECK(pthread_cancel(ctrl_thread));
    CHECK(pthread_join(ctrl_thread, NULL));
    turn_off(data);
    return NULL;
}


int main(int argc, char *argv[]) {

    struct sender_data *data = init_sender_data();
    parse_input(argc, argv, data);

    init_connection(data);

    pthread_t sending_thread;
    CHECK(pthread_create(&sending_thread, 0, send_music, (void *) data));


    CHECK(pthread_join(sending_thread, NULL));

    return 0;
}
