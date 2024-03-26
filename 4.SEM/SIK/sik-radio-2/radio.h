/*
 * Author: Zuzanna Ortenburger
 *
 * Implementation of a struct representing radio and radio list working with multiple threads.
 */

#ifndef MIMUW_SIK_RADIO_RADIO_H
#define MIMUW_SIK_RADIO_RADIO_H

#include "err.h"
#include "common.h"

struct Radio {
    char *name;
    char *address;
    uint16_t port;
    uint64_t series;
    struct Radio *prev;
    struct Radio *next;
    struct sockaddr_in *addr;
};

struct RadioList {
    struct Radio *head;
    size_t count;
    pthread_mutex_t mutex;
    struct Radio *current;
    bool turned_on;
};

void destroyRadio(struct Radio *radio) {
    free(radio->name);
    free(radio->address);
    free(radio);
}

struct RadioList *initializeRadioList() {
    struct RadioList *radioList = malloc(sizeof(struct RadioList));
    radioList->head = NULL;
    radioList->count = 0;
    CHECK(pthread_mutex_init(&radioList->mutex, NULL));
    radioList->current = NULL;
    radioList->turned_on = false;
    return radioList;
}

void destroyRadioList(struct RadioList *radioList) {
    struct Radio *current = radioList->head;
    struct Radio *next;
    while (current != NULL) {
        next = current->next;
        destroyRadio(current);
        current = next;
    }
    CHECK(pthread_mutex_destroy(&radioList->mutex));
    free(radioList);
}

struct Radio *newRadio(char *name, char *address, uint16_t port, uint64_t series, struct sockaddr_in *addr) {
    struct Radio *radio = malloc(sizeof(struct Radio));
    radio->name = malloc(strlen(name) + 1);
    memcpy(radio->name, name, strlen(name) + 1);
    radio->address = malloc(strlen(address) + 1);
    memcpy(radio->address, address, strlen(address) + 1);
    radio->port = port;
    radio->series = series;
    radio->prev = NULL;
    radio->next = NULL;
    radio->addr = addr;
    return radio;
}

bool remove_old_radios(struct RadioList *radioList, uint64_t series) {
    bool changed = false;
    CHECK(pthread_mutex_lock(&radioList->mutex));

    //usun radia ktore sa za stare
    struct Radio *current = radioList->head;
    struct Radio *next;
    while (current != NULL) {
        if (current->series + INACTIVITY_TIME <= series) {
            changed = true;
            if (radioList->current == current) {
                radioList->current = NULL;
            }
            if (current->prev == NULL) {
                radioList->head = current->next;
            } else {
                current->prev->next = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }
            radioList->count--;
            next = current->next;
            destroyRadio(current);
        } else {
            next = current->next;
        }
        current = next;
    }
    CHECK(pthread_mutex_unlock(&radioList->mutex));
    return changed;
}

bool addRadio(struct RadioList *radioList, char *name, char *address, uint16_t port, uint64_t series,
              struct sockaddr_in *addr) {
    CHECK(pthread_mutex_lock(&radioList->mutex));
    if (radioList->count == 0) {
        radioList->head = newRadio(name, address, port, series, addr);
        radioList->count++;
        CHECK(pthread_mutex_unlock(&radioList->mutex));
        return true;
    }
    // sprawdzamy czy radio o takiej nazwie już istnieje
    struct Radio *current = radioList->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && strcmp(current->address, address) == 0 && current->port == port) {
            current->series = series;
            pthread_mutex_unlock(&radioList->mutex);
            return false;
        }
        current = current->next;
    }

    struct Radio *radio = newRadio(name, address, port, series, addr);

    if (strcmp(radioList->head->name, name) >= 0) {
        radio->next = radioList->head;
        radioList->head = radio;
        radio->next->prev = radio;
    } else {
        struct Radio *current = radioList->head;
        while (!(current->next == NULL ||
                 (strcmp(current->name, name) < 0 && strcmp(current->next->name, name) >= 0))) {
            current = current->next;
        }
        if (current->next != NULL) {
            current->next->prev = radio;
        }
        radio->next = current->next;
        radio->prev = current;
        current->next = radio;
    }
    radioList->count++;
    CHECK(pthread_mutex_unlock(&radioList->mutex));
    return true;
}

struct Radio *findRadio(struct RadioList *radioList, char *name, char *address, uint16_t port) {
    CHECK(pthread_mutex_lock(&radioList->mutex));
    struct Radio *current = radioList->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && (address == NULL || strcmp(current->address, address) == 0) &&
            (port == 0 || current->port == port)) {
            CHECK(pthread_mutex_unlock(&radioList->mutex));
            return current;
        }
        current = current->next;
    }
    CHECK(pthread_mutex_unlock(&radioList->mutex));
    return NULL;
}

bool radio_switch_up(struct RadioList *radioList) {
    CHECK(pthread_mutex_lock(&radioList->mutex));
    if (radioList->current == NULL || radioList->current->prev == NULL) {
        CHECK(pthread_mutex_unlock(&radioList->mutex));
        return false;
    } else if (radioList->current->prev != NULL) {
        radioList->current = radioList->current->prev;
        CHECK(pthread_mutex_unlock(&radioList->mutex));
        return true;
    }
    return false;
}

bool radio_switch_down(struct RadioList *radioList) {
    CHECK(pthread_mutex_lock(&radioList->mutex));
    if (radioList->current == NULL) {
        radioList->current = radioList->head;
        CHECK(pthread_mutex_unlock(&radioList->mutex));
        return true;
    } else if (radioList->current->next == NULL) {
        CHECK(pthread_mutex_unlock(&radioList->mutex));
        return false;
    } else if (radioList->current->next != NULL) {
        radioList->current = radioList->current->next;
        CHECK(pthread_mutex_unlock(&radioList->mutex));
        return true;
    }
    return false;
}

void writeRadios(struct RadioList *radioList, int fd) {
    CHECK(pthread_mutex_lock(&radioList->mutex));
    struct Radio *current = radioList->head;
    while (current != NULL) {
        char *name = malloc(strlen(current->name) + 7);
        if (current == radioList->current) {
            sprintf(name, " > %s\n\r\n\r", current->name);
        } else {
            sprintf(name, "%s\n\r\n\r", current->name);
        }
        if (write(fd, name, strlen(name)) < 0) {
            PRINT_ERRNO();
        }
        current = current->next;
    }
    CHECK(pthread_mutex_unlock(&radioList->mutex));
}


#endif //MIMUW_SIK_RADIO_RADIO_H
