#pragma once
#include<stdio.h>
#include<stdlib.h>

typedef struct {
    size_t actSize; // rozmiar zaalokowanej pamieci na wektora
    size_t size; // liczba elementow na wektorze
    size_t *elements; // tablica reprezentujaca elementy na wektorze 
} vector;

void initVector(vector *v); //funckja inicjujaca wektora

void resizeVector(vector *v); //funkcja podwojnie zwiekszajaca rozmiar zaalokowanej pamieci

void setSizeVector(vector *v); //funkcja ustawiajaca rozmiar wektora na konkretny

void pushBack(vector *v, size_t elem); //funkcja dodajaca do wektora jeden element

void outVector(vector *v); //funkcja wypisujaca wektora

void freeVector(vector *v);//funkcja zwalniajaca zaalokowana pamiec na wektora