#include "string.h"

// funkcja inicjujaca stringa
void initString(string *s){
    s->size = 0;
    s->actSize = 1;
    s->elements = (char*)malloc((s->actSize) * sizeof(char));
}

// funkcja podwojnie zwiekszajaca pamiec zaalokowana na stringa 
void resizeString(string *s){
    s->actSize *= 2;
    s->elements = realloc(s->elements, (s->actSize) * sizeof(char));
}

// funkcja dodajaca chara na stringa
void addChar(string *s, char elem){
    if(s->size == s->actSize){
        resizeString(s);
    }

    s->elements[s->size] = elem; 
    s->size++;
}

// funkcja wypisujaca stringa
void outString(string *s){
    for(size_t i = 0; i < s->size; i++){
        printf("%c", s->elements[i]);
    }
    printf("\n");
}

//funkcja zwalniajaca pamiec zaalokowana na stringa
void freeString(string *s){
    free(s->elements);
}