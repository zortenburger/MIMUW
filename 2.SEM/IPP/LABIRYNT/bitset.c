#include "bitset.h"

// funkcja inicjujaca bitset
void initBitset(bitset *b){
    b->size = 0; 
    b->position = 0;
    b->count = 0;
    b->actSize = 1;
    b->elements = (uint32_t*)calloc((b->actSize), sizeof(uint32_t));
}

//funkcja podwojnie zwiekszajaca rozmiar zaalokowanej pamieci dla bitseta
void resizeBitset(bitset *b){
    b->actSize *= 2;
    b->elements = realloc(b->elements, (b->actSize) * sizeof(uint32_t));
}

//funckja ustawiajaca rozmiar bitseta na konkretny
void setSizeBitset(bitset *b, size_t size){
    b->actSize = (size / 32UL) + 1UL;
    b->size = (size / 32UL) + 1UL;
    free(b->elements);
    b->elements = (uint32_t*)calloc((b->actSize), sizeof(uint32_t));
}

// funkcja zapalajaca 4 bity w bitsecie odpowiadajace liczbie w zapisie 16
// zwraca false gdy wystapi blad i true w przeciwnym wypadku
bool addBit(bitset *b, char elem){
    if(b->count == b->actSize){
        return false;
    }

    int temp16 = 0; //liczba zamieniona z zapisu 16 na 10

    if(isdigit(elem)){
        temp16 = (int)elem - (int)'0';
    } 
    else {
        temp16 = 10 + (int)elem - (int)'A';
    }

    for(int i = 0; i < 4; i++){
        if((temp16 & (1 << i)) != 0){
            b->elements[b->count] += (1 << b->position); 
        }
        b->position++;
    }

    if(b->position == 32){
        b->count++;
        b->position = 0;
    }

    return true;
}

// funkcja zapalajaca konkretny bit w bitsecie
void addBitPos(bitset *b, size_t pos){
    size_t i = pos / 32;
    size_t j = pos % 32;

    if((b->elements[i] & (1UL << j)) == 0){
        b->elements[i] += (1UL << j); 
    }
}

// funkcja sprawdzajaca czy konkretny bit jest zapalony w bitsecie
bool checkBitset(bitset *b, size_t pos){
    size_t i = pos / 32;
    size_t j = pos % 32;
    
    if((b->elements[i] & (1UL << j)) != 0){
        return true;
    } 

    return false;
}

//funkcja wypisujaca bitseta
void outBitset(bitset *b){
    for(size_t i = 0; i < b->size; i++){
        for(size_t j = 0; j < 32; j++){
            if((b->elements[i] & (1<<j)) != 0){
                printf("1");
            }
            else {
                printf("0");
            }
        }
    }
    for(size_t j = 0; j < b->position; j++){
        if((b->elements[b->size] & (1<<j)) != 0){
            printf("1");
        }
        else {
            printf("0");
        }
    }
}

// funkcja odwracajaca stringa bedacego liczba w zapisie 16 i zapalajaca odpowiednie bity
// zwraca false gdy wystapi blad i true w przeciwnym wypadku
bool toBitset (string *s, bitset *b){
    size_t i;

    for(i = s->size; i>0; i--){
        size_t j = i - 1;
        if(!addBit(b, s->elements[j]) ){
            break;
        }
    }

    for(; i>0; i--){
        size_t j = i - 1;
        if(s->elements[j] != '0'){
            return false;
        }
    }

    return true;
}

//funkcja zwalniajaca pamiec uzywana przez bitseta
void freeBitset(bitset *b){
    free(b->elements);
}