/** @file
 * Implementacja klasy przechowującej strukture trie.
 *
 * @author Zuzanna Ortenburger <zo438646@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */
#include "trie.h"

#define NUMBER_OF_DIGITS 12 ///<liczba cyfr w numerach telefonow

/**
 * To jest struktura przechowująca wierzcholek trie.
 */
struct trieNode{
    char * forward; ///<napis reprezentujjacy przekierowanie
    trieNode * parent; ///<rodzic wierzchołka
    trieNode * digits[NUMBER_OF_DIGITS];///<tablica wskaznikow na dzieci w drzewie
};

trieNode * trieNew(trieNode *pf){
    trieNode *res = (trieNode*)calloc(1, sizeof(trieNode));

    if(res == NULL){
        return NULL;
    }

    res->forward = NULL;

    for(int i = 0; i < NUMBER_OF_DIGITS; i++){
        res->digits[i] = NULL;
    }

    res->parent = pf;

    return res;
}

void trieDelete(trieNode *pf){
    if(pf == NULL){
        return;
    }

    while(pf != NULL){
        trieNode * act = NULL;
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

            if(pf->forward != NULL){
                free(pf->forward);
                pf->forward = NULL;
            }

            free(pf);
            pf = act;
        }
        else{
            pf = act;
        }
    }
}

bool trieAdd(trieNode *pf, char const *num, char const *forward, trieNodeReverse *pfRev){
    size_t i = 0;

    char digit = num[i];

    while (digit != '\0')
    {
        int intDigit = toDigit(digit);
        if (pf->digits[intDigit] == NULL) {
            pf->digits[intDigit] = trieNew(pf);

            if(pf->digits[intDigit] == NULL){
                return false;
            }
        }

        pf = pf->digits[intDigit];
        i++;
        digit = num[i];
        intDigit = toDigit(digit);
    }
    if(pf->forward != NULL){
        trieRemoveRev(pfRev, pf->forward, num, 0);
        free(pf->forward);
        pf->forward = NULL;
    }

    pf->forward = setString(forward);

    if(pf->forward == NULL){
        return false;
    }

    return true;
}


/** @brief Usuwa poddrzewo.
 * Usuwa poddrzewo, do ktorego prowadzi sciezka,
 * ktorej krawedzie tworza napis @p num. Jeśli nie ma takiej ściezki
 * lub napis nie reprezentuje numeru, nic nie robi. Jednoczesnie usuwa 
 * odwrotne przekierowania ze struktury pfRev.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą drzewo.
 * @param[in] forward    – wskaźnik na napis reprezentującykrawedzie na sciezce.
 * @param[in, out] pfRev - wskaźnik na strukturę przechowującą trieRev
 */
static void trieRemoveSub(trieNode *pf, trieNodeReverse *pfRev, char const *forward){
    if(pf == NULL){
        return;
    }
    
    for(int i = 0; i < NUMBER_OF_DIGITS; i++){
        if(pf->digits[i] != NULL){
            char * s1 = ofDigit(i);
            char * s = setString2(forward, s1, 0);
            free(s1);
            trieRemoveSub(pf->digits[i], pfRev, s);
            free(s);

            free(pf->digits[i]);
            pf->digits[i] = NULL;
        }
    }

    if(pf->forward != NULL){
        trieRemoveRev(pfRev, pf->forward, forward, 0);
        free(pf->forward);
        pf->forward = NULL;
    }
}


void trieRemove(trieNode *pf, char const *forward, size_t depth, trieNodeReverse *pfRev){
    if(forward == NULL){
        return;
    }

    while(pf != NULL){
        char digit = forward[depth];
        if(digit == '\0'){
            if(pf != NULL){
                trieRemoveSub(pf, pfRev,  forward);
            }
            return;
        }

        int intDigit = toDigit(digit);
        pf = pf->digits[intDigit];
        depth++;
    }
}


char * trieGet(trieNode const *pf, char const *forward, size_t * i){
    char digit = forward[*i];

    char * res;
    res = NULL;

    size_t j = *i;

    while (digit != '\0')
    {
        int intDigit = toDigit(digit);

        pf = pf->digits[intDigit];

        (*i)++;

        if (pf == NULL) {
            *i = j;
            return res;
        }

        if(pf->forward != NULL){
            free(res);
            res = setString(pf->forward);
            j = *i;
        }

        digit = forward[*i];
    }

    if(pf->forward != NULL){
        free(res);
        res = setString(pf->forward);
        j = *i;
    }
    *i = j;

    return res;
}