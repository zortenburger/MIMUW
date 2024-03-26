/*
 * Author: Zuzanna Ortenburger
 *
 * Functions for user interface.
 */

#ifndef MIMUW_SIK_RADIO_UI_H
#define MIMUW_SIK_RADIO_UI_H

#include "err.h"
#include "common.h"
#include "radio.h"
#include "data.h"
#include "sikradio-switch.h"
#include <poll.h>

#define BUF_SIZE 1024
#define QUEUE_LENGTH 5
#define TIMEOUT 5000
#define CLEAR_SCREEN_LINE "\033[2J\033[1;1H"


inline static int open_socket() {
    int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd < 0) {
        PRINT_ERRNO();
    }

    return socket_fd;
}

inline static int accept_connection(int socket_fd, struct sockaddr_in *client_address) {
    socklen_t client_address_length = (socklen_t) sizeof(*client_address);

    int client_fd = accept(socket_fd, (struct sockaddr *) client_address, &client_address_length);
    if (client_fd < 0) {
        PRINT_ERRNO();
    }

    return client_fd;
}

inline static void start_listening(int socket_fd, size_t queue_length) {
    CHECK_ERRNO(listen(socket_fd, queue_length));
}

void refresh_screen(struct Receiver_data *receiver_data) {
    struct pollfd *poll_descriptors = receiver_data->poll_descriptors;
    int write_result;
    for (int i = 1; i < CONNECTIONS; ++i) {
        if (poll_descriptors[i].fd != -1) {
            write_result = write(poll_descriptors[i].fd, CLEAR_SCREEN_LINE, 11);
            write_result = write(poll_descriptors[i].fd,
                                 "------------------------------------------------------------------------\n\r\n\r SIK Radio\n\r\n\r------------------------------------------------------------------------\n\r\n\r",
                                 167);
            writeRadios(receiver_data->active_radios, poll_descriptors[i].fd);
            write_result = write(poll_descriptors[i].fd,
                                 "------------------------------------------------------------------------\n\r",
                                 75);
            if (write_result < 0) {
                PRINT_ERRNO();
            }
        }
    }
}


void *ui(void *data) {
    struct Receiver_data *receiver_data = (struct Receiver_data *) data;
    struct pollfd *poll_descriptors = receiver_data->poll_descriptors;
    char buf[BUF_SIZE];
    poll_descriptors[0].fd = open_socket();
    bind_socket(poll_descriptors[0].fd, receiver_data->UI_PORT);
    set_port_reuse(poll_descriptors[0].fd);
    start_listening(poll_descriptors[0].fd, QUEUE_LENGTH);
    do {
        for (int i = 0; i < CONNECTIONS; ++i) {
            poll_descriptors[i].revents = 0;
        }

        int poll_status = poll(poll_descriptors, CONNECTIONS, TIMEOUT);
        if (poll_status == -1) {
            if (errno == EINTR)
                fprintf(stderr, "Interrupted system call\n");
            else
                PRINT_ERRNO();
        } else if (poll_status > 0) {
            if ((poll_descriptors[0].revents & POLLIN)) {
                /* Przyjmuję nowe połączenie */
                int client_fd = accept_connection(poll_descriptors[0].fd, NULL);

                bool accepted = false;
                for (int i = 1; i < CONNECTIONS; ++i) {
                    if (poll_descriptors[i].fd == -1) {
                        poll_descriptors[i].fd = client_fd;
                        poll_descriptors[i].events = POLLIN;
                        accepted = true;
                        unsigned char cmd[] = {255, 251, 3, 255, 251, 1}; //255 is IAC, 251 is DO, 3 is SGA, 1 is ECHO
                        if (write(client_fd, cmd, sizeof(cmd)) < 0) {
                            PRINT_ERRNO();
                        }
                        refresh_screen(receiver_data);
                        break;
                    }
                }
                if (!accepted) {
                    CHECK_ERRNO(close(client_fd));
                    fprintf(stderr, "Too many clients\n");
                }
            }
            for (int i = 1; i < CONNECTIONS; ++i) {
                if (poll_descriptors[i].fd != -1 && (poll_descriptors[i].revents & (POLLIN | POLLERR))) {
                    ssize_t received_bytes = read(poll_descriptors[i].fd, buf, BUF_SIZE);
                    if (received_bytes < 0) {
                        fprintf(stderr, "Error when reading message from connection %d (errno %d, %s)\n", i, errno,
                                strerror(errno));
                        CHECK_ERRNO(close(poll_descriptors[i].fd));
                        poll_descriptors[i].fd = -1;
                    } else if (received_bytes == 0) {
                        CHECK_ERRNO(close(poll_descriptors[i].fd));
                        poll_descriptors[i].fd = -1;
                    } else {
                        buf[received_bytes] = '\0';

                        //sprawdz czy wczytalismy strzalke w gore
                        if (strcmp(buf, "\x1B[A") == 0) {
                            // handle up arrow key event
                            if (radio_switch_up(receiver_data->active_radios)) {
                                stop_current(receiver_data);
                                start_current(receiver_data);
                            }
                            refresh_screen(data);
                        } else if (strcmp(buf, "\x1B[B") == 0) {
                            // handle down arrow key event
                            if (radio_switch_down(receiver_data->active_radios)) {
                                stop_current(receiver_data);
                                start_current(receiver_data);
                            }
                            refresh_screen(data);
                        }

                    }
                }
            }
        }
    } while (true);

    if (poll_descriptors[0].fd >= 0)
        CHECK_ERRNO(close(poll_descriptors[0].fd));
    exit(EXIT_SUCCESS);
}

#endif //MIMUW_SIK_RADIO_UI_H
