/** @file
 * Interfejs klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Zuzanna Ortenburger <zo438646@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include "phone_forward.h"

/**
 * To jest struktura przechowująca przekierowania numerów telefonów.
 */
struct PhoneForward{
    trieNode * root; ///<korzen drzewa trie reprezentujacego przekierowania
    trieNodeReverse * rootRev;///<korzen drzewa trie reprezentujacego odwrotnosc przekierowan
};

/**
 * To jest struktura przechowująca ciąg numerów telefonów.
 */
struct PhoneNumbers{
    phones * numbers; ///<wskaznik na strukture reprezentujacą numery
};


PhoneForward * phfwdNew(void){
    PhoneForward * res;
    res = calloc(1, sizeof(PhoneForward));

    res->root = trieNew(NULL);
    res->rootRev = trieNewRev(NULL);

    if(res->root == NULL || res->rootRev == NULL || res == NULL){
        return NULL;
    }

    return res;
}

void phfwdDelete(PhoneForward *pf){
    if(pf != NULL && pf->root != NULL){
        trieDelete(pf->root);
        trieDeleteRev(pf->rootRev);

        pf->root = NULL;
        pf->rootRev = NULL;
    }

    free(pf);
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2){
    if(num1 == NULL || num2 == NULL || pf == NULL){
        return false;
    }

    if(!checkString(num1) || !checkString(num2)){
        return false;
    }

    if(cmpString(num1, num2)){
        return false;
    }

    if(trieAdd(pf->root, num1, num2, pf->rootRev) && trieAddRev(pf->rootRev, num2, num1)){
        return true;
    }
    else{
        return false;
    }
}


void phfwdRemove(PhoneForward *pf, char const *num){
    if(pf != NULL && num != NULL){
        if(!checkString(num)){
            return;
        }
        trieRemove(pf->root, num, 0, pf->rootRev);
    }
}

PhoneNumbers * phfwdGet(PhoneForward const *pf, char const *num){
    if(pf == NULL){
        return NULL;
    }

    size_t i = 0;

    PhoneNumbers * res;
    res = calloc(1, sizeof(PhoneNumbers));

    if(res == NULL){
        return res;
    } 
    res->numbers = calloc(1, sizeof(phones));

    char * s1 = NULL; 

    phnumInit(res->numbers);

    if(num == NULL || !checkString(num) || res == NULL || res->numbers == NULL){
        return res;
    }

    char * s = trieGet(pf->root, num, &i);

    if (s == NULL){
        phnumAdd(res->numbers, num);
    }
    else{
        s1 = setString2(s, num, i);
        phnumAdd(res->numbers, s1);
    }

    free(s1);
    free(s);

    return res;
}


PhoneNumbers * phfwdReverse(PhoneForward const *pf, char const *num){

    if(pf == NULL){
        return NULL;
    }

    PhoneNumbers * res = calloc(1, sizeof(PhoneNumbers));

    if(num == NULL || !checkString(num)){
        return res;
    }

    if(res == NULL){
        return NULL;
    }

    size_t i = 0;
    res->numbers = trieGetRev(pf->rootRev, num, &i);
    
    phnumAdd(res->numbers, num);
    phnumUnique(res->numbers);

    return res;
}

void phnumDelete(PhoneNumbers *pnum){
    if(pnum == NULL){
        free(pnum);
        return;
    }
    phnumDelete1(pnum->numbers);

    free(pnum);
}

char const * phnumGet(PhoneNumbers const *pnum, size_t idx){
    if(pnum == NULL){
        return NULL;
    }

    return(phnumGet1(pnum->numbers,idx));
}