#include "vector.h"

//funckja inicjujaca wektora
void initVector(vector *v){
    v->size = 0;
    v->actSize = 1;
    v->elements = (size_t*)malloc((v->actSize) * sizeof(size_t));
}

//funkcja podwojnie zwiekszajaca rozmiar zaalokowanej pamieci
void resizeVector(vector *v){
    v->actSize *= 2;
    v->elements = realloc(v->elements, (v->actSize) * sizeof(size_t));
}

//funkcja ustawiajaca rozmiar wektora na konkretny
void setSizeVector(vector *v){
    v->actSize = v->size;
    v->elements = realloc(v->elements, (v->actSize) * sizeof(size_t));
}

//funkcja dodajaca do wektora jeden element
void pushBack(vector *v, size_t elem){
    if(v->size == v->actSize){
        resizeVector(v);
    }

    v->elements[v->size] = elem; 
    v->size++;
}

//funkcja wypisujaca wektora
void outVector(vector *v){
    for(size_t i = 0; i < v->size; i++){
        printf("%zu ", v->elements[i]);
    }
    printf("\n");
}

//funkcja zwalniajaca zaalokowana pamiec na wektora
void freeVector(vector *v){
    free(v->elements);
}
