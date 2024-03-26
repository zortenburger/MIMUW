/*
 * Author: Zuzanna Ortenburger
 *
 * Implementation of a receiver for the sikradio program.
 */

#include <pthread.h>
#include <fcntl.h>

#include "err.h"
#include "common.h"
#include "data.h"
#include "sikradio-lookuper.h"
#include "sikradio-ui.h"


void parse_input(int argc, char *argv[], struct Receiver_data *data) {
    int opt;

    while ((opt = getopt(argc, argv, "d:C:U:b:n:R:")) != -1) {
        switch (opt) {
            case 'd':
                check_address(optarg);
                data->DISCOVER_ADDR = (optarg);
                break;
            case 'C':
                data->CTRL_PORT = read_port(optarg);
                break;
            case 'U':
                data->UI_PORT = read_port(optarg);
                break;
            case 'b':
                data->BSIZE = read_number(optarg, 1);
                break;
            case 'n':
                check_name(optarg);
                data->NAME_INPUT = optarg;
                break;
            case 'R':
                data->RTIME = read_number(optarg, 1);
                break;
            default:
                fprintf(stderr,
                        "Usage: %s [-d DISCOVER_ADDR] [-C CTRL_PORT] [-U UI_PORT] [-b BSIZE] [-n NAME] [-R RTIME]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }

}



int main(int argc, char *argv[]) {
    struct Receiver_data *receiver_data = init_receiver_data();
    parse_input(argc, argv, receiver_data);

    pthread_t ui_thread;
    CHECK_ERRNO(pthread_create(&ui_thread, 0, ui, (void *) receiver_data));

    pthread_t lookup_thread;
    CHECK_ERRNO(pthread_create(&lookup_thread, 0, lookup, (void *) receiver_data));
    CHECK_ERRNO(pthread_join(lookup_thread, NULL));
    pthread_join(ui_thread, NULL);
    turn_off_receiver(receiver_data);

    return 0;
}
