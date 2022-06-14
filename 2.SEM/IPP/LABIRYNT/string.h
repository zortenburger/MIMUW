#pragma once
#include<stdio.h>
#include<stdlib.h>

//definicja struktury string 
typedef struct {
    size_t actSize; // rozmiar zaalokowanej pamieci
    size_t size; // liczba elementow na stringu
    char *elements; // tablica charow odpowiadajacych stringowi
} string;

void initString(string *s); // funkcja inicjujaca stringa

void resizeString(string *s); // funkcja podwojnie zwiekszajaca pamiec zaalokowana na stringa 

void addChar(string *s, char elem); // funkcja dodajaca chara na stringa

void outString(string *s); // funkcja wypisujaca stringa

void freeString(string *s); //funkcja zwalniajaca pamiec zaalokowana na stringa