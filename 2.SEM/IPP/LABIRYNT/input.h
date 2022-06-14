#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include <stdint.h>
#include "vector.h"
#include "errors.h"
#include "string.h"
#include "bitset.h"


bool readNumber (size_t *result); // funkcja wczytująca liczbę z zakresu [1, SIZE_MAX] ze standardowego wejścia

bool readNumber32 (uint32_t *result); // funkcja wczytująca liczbę z zakresu [0, UINT32_MAX] ze standardowego wejścia

int next(); //funkcja prawdzajaca czy nastepny znak to bialy znak lub /n lub EOF

bool readLine (vector *v); // funkcja wczytuje linie zlozona z liczb z zakresu [1; SIZE_MAX] i dodaje je na vectora v

bool is16Digit (char c); // fukncja sprawdzajaca czy znak jest cyfra w zapisie 16 liczby

bool read4aLine (bitset *b, size_t *size); // funkcja wczytujaca 4 linie w wersji z zapisem 16 liczby 

bool read4bLine (uint32_t *a, uint32_t *b, uint32_t *m, uint32_t *r, uint32_t *s0); // funkcja wczytujaca 4 linie w wersji z R 

void change4b_4a(uint32_t *a, uint32_t *b, uint32_t *m, uint32_t *r, uint32_t *s0, bitset *v, size_t *size); //funkcja zamieniajaca reprezentacje labityntu z wersji z R na bitset scian labiryntu

bool read4Line (bitset *v, size_t *size); // funkcja wczytujaca 4 linie 

size_t calculateSize(vector *v); //funkcja obliczajaca rozmiar calego labiryntu 

bool isCubeIn (vector *sizes, vector *act); // funkcja sprawdzajaca czy kostka o wspolrzednych act miesci sie w labiryncie o wymiarach sizes

bool isEOF(); // funkcja sprawdzajaca czy wystapil EOF

bool goodAlloc (vector *beginning, vector *end, bitset *walls, vector *sizes); //funkcja sprawdzjaca czy wszystkie struktury sa dobrze zaalokowane

int readData ( vector *beginning, vector *end, bitset *walls, vector *sizes); //funkcja wczytujaca cale wejscie i zwracajaca rodzaj lub brak bledu