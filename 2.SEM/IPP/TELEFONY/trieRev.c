/** @file
 * Implementacja klasy przechowującej strukture trie reverse.
 *
 * @author Zuzanna Ortenburger <zo438646@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */
#include "trieRev.h"

#define NUMBER_OF_DIGITS 12 ///<liczba cyfr w numerach telefonow

/**
 * To jest struktura przechowująca wierzcholek trie.
 */
struct trieNodeReverse{
    phones * reverse; ///<napisy reprezentujacy przekierowanie
    trieNodeReverse * parent;///<rodzic wierzcholka
    trieNodeReverse * digits[NUMBER_OF_DIGITS]; ///<tablica wskaznikow na dzieci w drzewie
};

trieNodeReverse * trieNewRev(trieNodeReverse * pf){
    trieNodeReverse *res = (trieNodeReverse*)calloc(1, sizeof(trieNodeReverse));

    if(res == NULL){
        return NULL;
    }

    res->reverse = calloc(1, sizeof(phones));
    phnumInit(res->reverse);

    for(int i = 0; i < NUMBER_OF_DIGITS; i++){
        res->digits[i] = NULL;
    }

    res->parent = pf;

    return res;
}

void trieDeleteRev(trieNodeReverse *pf){
    if(pf == NULL){
        return;
    }
    while(pf != NULL){
        trieNodeReverse * act = NULL;
        for(int i = 0; i < NUMBER_OF_DIGITS; i++){
            if(pf->digits[i] != NULL){
                act = pf->digits[i];
                pf->digits[i] = NULL;
                break;
            }
        }
        if(act == NULL){
            act = pf->parent;

            pf->parent = NULL;
             if(pf->reverse != NULL){
                phnumDelete1(pf->reverse);
                pf->reverse = NULL;
            }
            free(pf);
            pf = act;
        }
        else{
            pf = act;
        }
    }
}

bool trieAddRev(trieNodeReverse *pf, char const *num, char const *forward){
    size_t i = 0;

    char digit = num[i];

    while (digit != '\0')
    {
        int intDigit = toDigit(digit);
        if (pf->digits[intDigit] == NULL) {
            pf->digits[intDigit] = trieNewRev(pf);

            if(pf->digits[intDigit] == NULL){
                return false;
            }
        }

        pf = pf->digits[intDigit];

        i++;
        digit = num[i];
        intDigit = toDigit(digit);
    }

    phnumAdd(pf->reverse, forward);

    if(pf->reverse == NULL){
        return false;
    }


    return true;
}

void trieRemoveRev(trieNodeReverse *pf, char const *forward, char const *revForward, size_t depth){
    if(forward == NULL || revForward == NULL){
        return;
    }

    while(pf != NULL ){
        char digit = forward[depth];

        if(digit == '\0'){
            if(pf != NULL){
                for(size_t i = 0; i < pf->reverse->size; i++){
                    if(strcmp(pf->reverse->numbers[i], revForward) == 0){
                        phnumRemove(pf->reverse, i);
                        return;
                    }
                }
            }
            return;
        }

        int intDigit = toDigit(digit);
        pf = pf->digits[intDigit];
        depth++;
    }
}


phones * trieGetRev(trieNodeReverse const *pf, char const *forward, size_t * i){
    char digit = forward[*i];

    phones * res = calloc(1, sizeof(phones));

    if(res == NULL){
        return NULL;
    }

    phnumInit(res);

    if(res == NULL){
        return NULL;
    }
    
    while (digit != '\0')
    {
        int intDigit = toDigit(digit);

        pf = pf->digits[intDigit];

        (*i)++;

        if (pf == NULL) {
            return res;
        }

        if(pf->reverse != NULL){
            char ** num = getNumbers(pf->reverse);
            for(size_t j = 0; j < getSize(pf->reverse); j++){
                char * s = setString2(num[j], forward, *i);
                phnumAdd(res, s);
                free(s);
            }
        }

        digit = forward[*i];
    }
    return res;
}