/*
 * Author: Zuzanna Ortenburger
 *
 * Implementation of a sender for the sikradio program.
 */

#include <time.h>

#include "err.h"
#include "common.h"


char *DEST_ADDR = NULL;
uint16_t DATA_PORT = DEFAULT_DATA_PORT;
uint64_t PSIZE = DEFAULT_PSIZE;
char *NAZWA = DEFAULT_NAME;
uint64_t session_id;
uint64_t first_byte_num = 0;


int main(int argc, char *argv[]) {
    session_id = (uint64_t) time(NULL);
    session_id = htobe64(session_id);
    int opt;

    while ((opt = getopt(argc, argv, "a:P:p:n:")) != -1) {
        switch (opt) {
            case 'a':
                DEST_ADDR = optarg;
                break;
            case 'P':
                DATA_PORT = read_port(optarg);
                break;
            case 'p':
                PSIZE = atoi(optarg);
                break;
            case 'n':
                NAZWA = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -a DEST_ADDR [-P DATA_PORT] [-p PSIZE] [-n NAZWA]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (DEST_ADDR == NULL) {
        fprintf(stderr, "Error: DEST_ADDR (-a) is required\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in send_address = get_send_address(DEST_ADDR, DATA_PORT);

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        PRINT_ERRNO();
    }

    uint8_t *buffer = malloc(PSIZE * sizeof(uint8_t) + 2 * sizeof(uint64_t));

    memset(buffer, 0, PSIZE * sizeof(uint8_t) + 2 * sizeof(uint64_t));
    memcpy(buffer, &session_id, sizeof(uint64_t));

    size_t nread;

    while ((nread = fread(buffer + 2 * sizeof(uint64_t), 1, PSIZE, stdin)) == PSIZE) {
        uint64_t first_byte_num_n = htobe64(first_byte_num);
        memcpy(buffer + sizeof(uint64_t), &first_byte_num_n, sizeof(uint64_t));
        send_message(socket_fd, &send_address, buffer, PSIZE + 2 * sizeof(uint64_t));
        first_byte_num += PSIZE;
    }

    free(buffer);
    CHECK_ERRNO(close(socket_fd));

    return 0;
}
