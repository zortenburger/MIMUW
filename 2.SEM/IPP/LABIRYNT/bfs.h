#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "bitset.h"
#include "queue.h"
#include "vector.h"


bool isCubeFree(size_t position, bitset *walls); //funkcja sprawdzajaca czy kostka byla kiedys odwiedzona lub czy jest sciana 

void visitCube(size_t position, bitset *walls); // funkcja zaznaczajaca kostke jako odwiedzona na bitsecie

void move(size_t position, vector *levels, bitset *walls, queue *q); // funkcja obliczajaca wszystkich sasiadow kostki positions, odwiedzajaca tych sasiadow i dodajaca ich do kolejki

size_t countCube(vector *positions, vector *levels); //funkcja obliczajaca jaki bit odpowiada kostce o wspolrzednych positions

void calculateLevels(vector *dimensions);// funkcja zamieniajaca rozmiary wymiarow na ich iloczyn

int bfs (vector *start, vector *stop, bitset *walls, vector *dimensions, size_t *result); // funkcja wykonujaca bfs-a (znajdujaca najkrotsza droge miedzy poczatkiem a koncem)