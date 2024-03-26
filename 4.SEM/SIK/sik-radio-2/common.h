/*
 * Author: Zuzanna Ortenburger
 *
 * Functions common to both sender and receiver.
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
#include <pthread.h>
#include <stdlib.h>

#include "constants.h"
#include "err.h"


static inline void check_address(const char *string) {
    struct in_addr addr;
    if (inet_pton(AF_INET, string, &addr) <= 0) {
        fprintf(stderr, "ERROR: Invalid IP address %s\n", string);
        exit(EXIT_FAILURE);
    }
}

static inline void is_multicast_address(const char *ip_address) {
    char *token;
    char *copy = strdup(ip_address);
    token = strtok(copy, ".");
    int first_octet = atoi(token);

    free(copy);

    if (first_octet >= 224 && first_octet <= 239) {
        return ;
    } else {
        fprintf(stderr, "ERROR: Invalid IP address %s\n", ip_address);
        exit(EXIT_FAILURE);
    }
}

static inline bool is_valid_ip(const char *string) {
    struct in_addr addr;
    if (inet_pton(AF_INET, string, &addr) <= 0) {
        return 0;
    }
    char *token;
    char *copy = strdup(string);
    token = strtok(copy, ".");
    int first_octet = atoi(token);

    free(copy);

    if (first_octet >= 224 && first_octet <= 239) {
        return true;
    } else {
        return false;
    }
}

static inline void check_name(const char *string) {
    if (strlen(string) == 0) {
        fprintf(stderr, "ERROR: Name cannot be empty\n");
        exit(EXIT_FAILURE);
    }

    for(size_t i = 0; i<strlen(string); i++) {
        if (string[i] < 32 ) {
            fprintf(stderr, "ERROR: Name cannot be empty\n");
            exit(EXIT_FAILURE);
        }
    }
}


static inline bool is_valid_name(const char *string) {
    if (strlen(string) == 0) {
        return false;
    }
    for(size_t i = 0; i<strlen(string); i++) {
        if (string[i] < 32 ) {
            return false;
        }
    }
    return true;
}

static inline uint16_t read_port(const char *string) {
    char *endptr;
    unsigned long port = strtoul(string, &endptr, 10);
    PRINT_ERRNO();
    if (*endptr != '\0' || port > UINT16_MAX) {
        fatal("%s is not a valid port number", string);
        exit(EXIT_FAILURE);
    }
    return (uint16_t) port;
}

static inline bool is_valid_port(const char *string) {
    char *endptr;
    unsigned long port = strtoul(string, &endptr, 10);
    if (*endptr != '\0' || port > UINT16_MAX) {
        return 0;
    }
    return 1;
}

static inline uint16_t read_psize(const char *string) {
    char *endptr;
    long long size = strtoll(string, &endptr, 10);
    PRINT_ERRNO();
    if (*endptr != '\0' || size > MAX_UDP_SIZE || size < 1) {
        fatal("%s is not a valid psize", string);
        exit(EXIT_FAILURE);
    }
    return (uint16_t) size;
}

static inline uint64_t read_number(const char *string, uint64_t lower_bound) {
    char *endptr;
    long long size = strtoll(string, &endptr, 10);
    PRINT_ERRNO();
    if (*endptr != '\0' || size < (long long)lower_bound) {
        fatal("%s is not a valid number", string);
        exit(EXIT_FAILURE);
    }
    return (uint64_t) size;
}

struct sockaddr_in get_send_address(char *host, uint16_t port) {
    /* ustawienie adresu i portu odbiorcy */
    struct sockaddr_in remote_address;
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(port);
    if (inet_aton(host, &remote_address.sin_addr) == 0) {
        fprintf(stderr, "ERROR: inet_aton - invalid multicast address\n");
        exit(EXIT_FAILURE);
    }
    return remote_address;
}

inline static int open_udp_socket() {
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        PRINT_ERRNO();
    }
    return socket_fd;
}

inline static void connect_socket(int socket_fd, const struct sockaddr_in *address) {
    CHECK_ERRNO(connect(socket_fd, (struct sockaddr *) address, sizeof(*address)));
}

inline static void bind_socket(int socket_fd, uint16_t port) {
    struct sockaddr_in address;
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = htonl(INADDR_ANY); // listening on all interfaces
    address.sin_port = htons(port);

    // bind the socket to a concrete address
    CHECK_ERRNO(bind(socket_fd, (struct sockaddr *) &address,
                     (socklen_t) sizeof(address)));
}

int receive_message(int socket_fd, struct sockaddr_in *receive_address, uint8_t *buffer, size_t max_length) {
    int receive_flags = 0;
    socklen_t
            address_length = (socklen_t)
            sizeof(*receive_address);
    errno = 0;
    ssize_t received_length = recvfrom(socket_fd, buffer, max_length, receive_flags,
                                       (struct sockaddr *) receive_address, &address_length);
    if (received_length < 0) {
        return -1;
    }
    return (size_t) received_length;
}


inline static void
send_message_to(int socket_fd, const struct sockaddr_in *send_address, const uint8_t *message, size_t message_length) {
    int send_flags = 0;
    socklen_t
            address_length = (socklen_t)
            sizeof(*send_address);
    errno = 0;
    ssize_t sent_length = sendto(socket_fd, message, message_length, send_flags,
                                 (struct sockaddr *) send_address, address_length);
    if (sent_length < 0) {
        PRINT_ERRNO();
    }
    ENSURE(sent_length == (ssize_t) message_length);
}

inline static void send_message(int socket_fd, const void *message, size_t length, int flags) {
    errno = 0;
    ssize_t sent_length = send(socket_fd, message, length, flags);
    if (sent_length < 0) {
        PRINT_ERRNO();
    }
    ENSURE(sent_length == (ssize_t) length);
}

inline static void set_port_reuse(int socket_fd) {
    int option_value = 1;
    CHECK_ERRNO(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &option_value, sizeof(option_value)));
}

#endif //MIMUW_SIK_RADIO_COMMON_H
