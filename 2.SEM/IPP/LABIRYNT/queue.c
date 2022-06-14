#include "queue.h"

// funkcja inicjujaca element kolejki
void initQueueElement(queueElement *qe, size_t position){
    qe->position = position;
    qe->next = NULL;
}

//funkcja inicjujaca kolejke
void initQueue(queue *q){
    q->first = NULL;
    q->last = NULL;
    q->size = 0;
}

// funkcja dodajaca element do kolejki
void addQueue(queue *q, size_t position){
    queueElement *qe = (queueElement*)malloc(sizeof(struct queueElement));

    initQueueElement(qe, position);

    if(q->size == 0UL){ // kolejka jest pusta
        q->first = qe;
        q->last = qe;
        q->size++;
        return;
    }

    queueElement *temp = q->last;
    temp->next = qe;
    q->last = qe;
    q->size++;   
}

//funkcja usuwajaca pierwszy element kolejki
void removeFirst(queue *q){ 
    queueElement *temp = q->first;
    q->first = temp->next;
    q->size--;
    free(temp);
}

// funkcja zwracajaca pierwszy element kolejki
size_t getPosition(queue *q){
    return ((q->first)->position);
}

//funkcja zwalniajaca cala kolejke
void freeQueue (queue *q){
    while(q->size > 0){
        removeFirst(q);
    }
    return;
}