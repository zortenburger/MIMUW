/*
 * Source: MIMUW Computer Networks lesson scenario (Scenariusz 3 - UDP)
 *
 * Functions and macros common to both sender and receiver.
 * UDP communication.
 */

#ifndef MIMUW_SIK_RADIO_COMMON_H
#define MIMUW_SIK_RADIO_COMMON_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <signal.h>

#include "err.h"

#define NO_FLAGS 0
#define DEFAULT_DATA_PORT 28646
#define DEFAULT_PSIZE 512
#define DEFAULT_NAME "Nienazwany Nadajnik"
#define DEFAULT_BSIZE 65536
#define MAX(a, b) (((a)>(b))?(a):(b))

uint16_t read_port(char *string) {
    errno = 0;
    unsigned long port = strtoul(string, NULL, 10);
    PRINT_ERRNO();
    if (port > UINT16_MAX) {
        fatal("%ul is not a valid port number", port);
    }

    return (uint16_t) port;
}

struct sockaddr_in get_send_address(char *host, uint16_t port) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    struct addrinfo *address_result;
    CHECK(getaddrinfo(host, NULL, &hints, &address_result));

    struct sockaddr_in send_address;
    send_address.sin_family = AF_INET; // IPv4
    send_address.sin_addr.s_addr =
            ((struct sockaddr_in *) (address_result->ai_addr))->sin_addr.s_addr; // IP address
    send_address.sin_port = htons(port); // port from the command line

    freeaddrinfo(address_result);

    return send_address;
}


int bind_socket(uint16_t port) {
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0); // creating IPv4 UDP socket
    ENSURE(socket_fd > 0);
    // after socket() call; we should close(sock) on any execution path;

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; // IPv4
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // listening on all interfaces
    server_address.sin_port = htons(port);

    // bind the socket to a concrete address
    CHECK_ERRNO(bind(socket_fd, (struct sockaddr *) &server_address,
                     (socklen_t) sizeof(server_address)));

    return socket_fd;
}

size_t receive_message(int socket_fd, struct sockaddr_in *receive_address, uint8_t *buffer, size_t max_length) {
    int receive_flags = 0;
    socklen_t address_length = (socklen_t) sizeof(*receive_address);
    errno = 0;
    ssize_t received_length = recvfrom(socket_fd, buffer, max_length, receive_flags,
                                       (struct sockaddr *) receive_address, &address_length);
    if (received_length < 0) {
        PRINT_ERRNO();
    }
    return (size_t) received_length;
}


void
send_message(int socket_fd, const struct sockaddr_in *send_address, const uint8_t *message, size_t message_length) {
    int send_flags = 0;
    socklen_t address_length = (socklen_t) sizeof(*send_address);
    errno = 0;
    ssize_t sent_length = sendto(socket_fd, message, message_length, send_flags,
                                 (struct sockaddr *) send_address, address_length);
    if (sent_length < 0) {
        PRINT_ERRNO();
    }
    ENSURE(sent_length == (ssize_t) message_length);
}

#endif //MIMUW_SIK_RADIO_COMMON_H
