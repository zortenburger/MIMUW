#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>

typedef struct queueElement queueElement;

//definicja elementu kolejki
struct  queueElement{
   size_t position; // wartosc 
   queueElement *next; // wskaznik na nastepny element kolejki
};

typedef struct {
   queueElement *first; //pierwszy element kolejki
   queueElement *last; // ostatni element kolejki
   size_t size; //liczba elementow na kolejce
} queue;

void initQueueElement(queueElement *qe, size_t position); // funkcja inicjujaca element kolejki

void initQueue(queue *q); //funkcja inicjujaca kolejke

void addQueue(queue *q, size_t position); // funkcja dodajaca element do kolejki

void removeFirst(queue *q); //funkcja usuwajaca pierwszy element kolejki

size_t getPosition(queue *q);// funkcja zwracajaca pierwszy element kolejki

void freeQueue (queue *q);//funkcja zwalniajaca cala kolejke