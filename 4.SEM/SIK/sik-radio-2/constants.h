/*
 * Author: Zuzanna Ortenburger
 *
 * Constants used in sik radio.
 */

#ifndef MIMUW_SIK_RADIO_CONSTANTS_H
#define MIMUW_SIK_RADIO_CONSTANTS_H

#define NO_FLAGS 0
#define DEFAULT_DATA_PORT 28646
#define DEFAULT_CTRL_PORT 38646
#define DEFAULT_UI_PORT 18646
#define DEFAULT_PSIZE 512
#define DEFAULT_FSIZE 131072
#define DEFAULT_BSIZE 65536
#define DEFAULT_RTIME 250
#define DEFAULT_NAME "Nienazwany Nadajnik"
#define DEFAULT_DISCOVER_ADDR "255.255.255.255"
#define MAX_UDP_SIZE 65536
#define INACTIVITY_TIME 20
#define LOOKUP_TIME 5

#define LOOKUP "ZERO_SEVEN_COME_IN"
#define look_len 18

#define REPLY "BOREWICZ_HERE"
#define reply_len 13

#define REXMIT "LOUDER_PLEASE "
#define rexmit_len 14

#define TTL_VALUE 4

#define MAX(a, b) (((a)>(b))?(a):(b))
#define CONNECTIONS 50

#endif //MIMUW_SIK_RADIO_CONSTANTS_H
