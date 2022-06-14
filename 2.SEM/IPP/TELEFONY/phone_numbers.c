/** @file
 * Interfejs klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Zuzanna Ortenburger <zo438646@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include "phone_numbers.h"

char ** getNumbers(phones* phnum){
    return phnum->numbers;
}

size_t getSize(phones* phnum){
    return phnum->size;
}

void phnumInit(phones * phone){
    phone->size = 0;
    phone->actSize = 1;
    phone->numbers = calloc(phone->actSize, sizeof(char*));
}

void phnumResize(phones *s){
    s->actSize *= 2;
    s->numbers = realloc(s->numbers, (s->actSize) * sizeof(char*));
}

void phnumAdd(phones *s, const char * elem){
    if(elem == NULL){
        return;
    }

    if(s == NULL){
        return;
    }
    
    if(s->size == s->actSize){
        phnumResize(s);
    }

    if(s->numbers != NULL){
        s->numbers[s->size] = setString(elem); 
    }

    s->size++;
}

void phnumRemove(phones *pnum, int idx){
    free((char *)pnum->numbers[idx]);
    pnum->numbers[idx] = pnum->numbers[pnum->size - 1];
    pnum->numbers[pnum->size - 1] = NULL;
    pnum->size--;
}

int compare(const void *a, const void *b){
    if(a == NULL && b == NULL){
        return 0;
    }
    else if(a == NULL){
        return 1;
    }
    else if(b == NULL){
        return -1;
    }

    const char * num1 = *(const char **) a;
    const char * num2 = *(const char **) b;

    if(num1 == NULL && num2 == NULL){
        return 0;
    }
    else if(num1 == NULL){
        return 1;
    }
    else if(num2 == NULL){
        return -1;
    }

    size_t lenght1 = 0;

    while(num1[lenght1] != '\0'){
        if(toDigit(num1[lenght1]) < toDigit(num2[lenght1])){
            return -1;
        }
        else if(toDigit(num1[lenght1]) > toDigit(num2[lenght1])){
            return 1;
        }
        lenght1++;
    }

    if(num2[lenght1] != '\0'){
        return -1;
    }

    return 0;
}

void phnumSort(phones *pnum){
    qsort(pnum->numbers, pnum->size, sizeof(const char*), compare);
}

void phnumUnique(phones *pnum){
    phnumSort(pnum);

    for(int i = pnum->size - 1; i > 0; i--)
    {
        if(cmpString(pnum->numbers[i], pnum->numbers[i-1])){
            phnumRemove(pnum, i);
        }
    }

    phnumSort(pnum);

}

void phnumDelete1(phones *pnum){
    if(pnum == NULL){
        return;
    }

    for(size_t i = 0; i < pnum->size; i++){
        if(pnum->numbers[i] != NULL){
            free(pnum->numbers[i]);
            pnum->numbers[i] = NULL;
        }
    }

    free(pnum->numbers);

    free(pnum);
}

char const * phnumGet1(phones const *pnum, size_t idx){
    if(pnum == NULL){
        return NULL;
    }

    if(idx >= pnum->size){
        return NULL;
    }
    
    return (pnum->numbers[idx]);
}