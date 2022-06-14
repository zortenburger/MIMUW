#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include "string.h"

typedef struct {
    size_t actSize;  // ilosc zaalokowanej pamieci dla bitseta
    size_t size; // ile uint32 mam na bitsecie 
    size_t count; 
    size_t position; // pozycja bita w aktualnym uincie32
    uint32_t *elements; // tablica reprezentujaca bitset
} bitset;

void initBitset(bitset *b); // funkcja inicjujaca bitset

void resizeBitset(bitset *b); //funkcja podwojnie zwiekszajaca rozmiar zaalokowanej pamieci dla bitseta

void setSizeBitset(bitset *b, size_t size); //funckja ustawiajaca rozmiar bitseta na konkretny

bool addBit(bitset *b, char elem); // funkcja zapalajaca 4 bity w bitsecie odpowiadajace liczbie w zapisie 16

void addBitPos(bitset *b, size_t pos); // funkcja zapalajaca konkretny bit w bitsecie

void outBitset(bitset *b); //funkcja wypisujaca bitseta

bool checkBitset(bitset *b, size_t pos); // funkcja sprawdzajaca czy konkretny bit jest zapalony w bitsecie

bool toBitset (string *s, bitset *b); // funkcja odwracajaca stringa bedacego liczba w zapisie 16 i zapalajaca odpowiednie bity

void freeBitset (bitset *q); //funkcja zwalniajaca pamiec uzywana przez bitseta