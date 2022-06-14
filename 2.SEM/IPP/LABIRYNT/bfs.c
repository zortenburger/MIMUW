#include "bfs.h"

//funkcja sprawdzajaca czy kostka byla kiedys odwiedzona lub czy jest sciana zwraca:
//false - kostka to sciana
//false - kostka byla odwiedzona
// true - w przeciwnym wypadku
bool isCubeFree(size_t position, bitset *walls){
    if (!checkBitset(walls, position-1)){
        return true;
    }
    return false;
}

// funkcja zaznaczajaca kostke jako odwiedzona na bitsecie
void visitCube(size_t position, bitset *walls){
    addBitPos(walls, position-1);
}

// funkcja obliczajaca wszystkich sasiadow kostki positions
// odwiedzajaca tych sasiadow i dodajaca ich do kolejki
void move(size_t position, vector *levels, bitset *walls, queue *q){
    size_t next = position;

    for(size_t i = 0; i < levels->size - 1; i++) {
        if(position + levels->elements[i] <= levels->elements[levels->size - 1]){ // sprawdzam czy nie wyszlam poza labirynt
            next = position + levels->elements[i]; // zwiekszam jeden z wymiarow o 1
            if(isCubeFree(next, walls) && ((position - 1) / levels->elements[i+1] == (next - 1) / levels->elements[i+1])){
                // sprawdzam czy kostka zostala odwiedzona oraz czy jestem na tym samym poziomie
                visitCube(next, walls);
                addQueue(q, next);
            }
        }
        if(position > levels->elements[i]){ // sprawdzam czy nie zejde ponizej 0
            next = position - levels->elements[i]; // zmniejszam jeden z wymiarow o 1
            if(isCubeFree(next, walls) && ((position - 1) / levels->elements[i+1] == (next - 1) / levels->elements[i+1])){
                // sprawdzam czy kostka zostala odwiedzona oraz czy jestem na tym samym poziomie
                visitCube(next, walls);
                addQueue(q, next);
            }
        }
    }
}

//funkcja obliczajaca jaki bit odpowiada kostce o wspolrzednych positions
size_t countCube(vector *positions, vector *levels){
    size_t result = 0;

    for(size_t i = 0; i < levels->size - 1; i++){
        result += (positions->elements[i]-1)*levels->elements[i];
    }

    return result;
}

// funkcja zamieniajaca rozmiary wymiarow na ich iloczyn
void calculateLevels(vector *dimensions){
    size_t pop = dimensions->elements[0];
    dimensions->elements[0] = 1;

    for(size_t i = 1; i < dimensions->size; i++){
        size_t akt = dimensions->elements[i];
        dimensions->elements[i] = dimensions->elements[i-1] * pop;
        pop = akt;
    }

    pushBack(dimensions, dimensions->elements[dimensions->size-1] * pop);
}

// funkcja wykonujaca bfs-a (znajdujaca najkrotsza droge miedzy poczatkiem a koncem)
// zwraca:
// 2 - poczatkowa kostka jest zajeta
// 3 - koncowa kostka jest zajeta
// 6 - nie znaleziono drogi
// 7 - znaleziono droge
int bfs(vector *start, vector *stop, bitset *walls, vector *dimensions, size_t *result){
    *result = 0; // wynik

    queue q; // kolejka niezbedna do bfsa
    initQueue(&q);

    calculateLevels(dimensions);

    //kostki numeruje [1; rozmiar labiryntu]
    size_t nrStart = countCube(start, dimensions) + 1UL; 
    size_t nrEnd = countCube(stop, dimensions) + 1UL; 

    if(!isCubeFree(nrStart, walls)){
        freeQueue(&q);
        return 2; // poczatkowa kostka jest zajeta
    }
    if(!isCubeFree(nrEnd, walls)){
        freeQueue(&q);
        return 3; // koncowa kostka jest zajeta
    }

    if(nrStart == nrEnd){
        return 7; // poczatkowa i koncowa kostka sa w tym samym miejscu
    }

    addQueue(&q, nrStart);
    visitCube(nrStart, walls);
    addQueue(&q, 0); // uzywam 0 jako flaga niezbedna do zwiekszenia wyniku

    size_t actCube;

    while((q.size) > 1){
        actCube = getPosition(&q);
        removeFirst(&q);

        if(actCube == 0){
            (*result)++;
            addQueue(&q, 0);
        }
        else {
            if(actCube == nrEnd){
                freeQueue(&q);
                return 7;
            }
            move(actCube, dimensions, walls, &q);
        }
    }

    freeQueue(&q);
    
    return 6;
}