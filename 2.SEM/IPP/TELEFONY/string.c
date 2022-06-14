/** @file
 * Implementacja klasy przechowującej funkcje obsługujące napisy.
 *
 * @author Zuzanna Ortenburger <zo438646@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include "string.h"

#define NUMBER_OF_DIGITS 12 ///<liczba cyfr w numerach telefonow

bool isDigit(char const c){
    if(!isdigit(c) && c != '*' && c != '#'){
        return false;
    }
    return true;
}

int toDigit(char const c){
    if(isdigit(c)){
        return (int)c - '0';
    }
    else if(c == '*'){
        return 10;
    }
    else if(c == '#'){
        return 11;
    }
    return -1;
}

char * ofDigit(int c){
    char* ans = calloc(2, sizeof(char));
    ans[1] = '\0';

    if(c <= 9){
        ans[0] = '0' + c;
    }
    else if(c == 10){
        ans[0] = '*';
    }
    else if(c == 11){
        ans[0] = '#';
    }
    return ans;
}

bool checkString (char const *in){
    size_t lenght1 = 0;

    while(in[lenght1] != '\0'){
        if(!isDigit(in[lenght1])){
            return false;
        }

        lenght1++;
    }

    if(lenght1 == 0){
        return false;
    }

    return true;
}

bool cmpString (char const *num1, char const *num2){
    size_t lenght1 = 0;

    while(num1[lenght1] != '\0'){
        if(num1[lenght1] != num2[lenght1]){
            return false;
        }

        lenght1++;
    }

    if(num2[lenght1] != '\0'){
        return false;
    }

    return true;
}

char * setString (char const *in){
    size_t i = 0; 

    while(in[i] != '\0'){
        i++;
    }

    char *s = NULL;
    s = (char*)calloc((i + 1), sizeof(char));

    if(s == NULL){
        return s;
    }

    for(size_t j = 0; j <= i; j++){
        s[j] = in[j];
    }

    return s;
}

char * setString2 (char const *in1, char const *in2, size_t x){
    size_t i1 = 0; 

    while(in1[i1] != '\0'){
        i1++;
    }

    size_t i2 = 0; 

    while(in2[i2] != '\0'){
        i2++;
    }

    char *s;
    s = (char*)calloc((i2 + i1 - x + 1), sizeof(char));

    if(s == NULL){
        return NULL;
    }

    for(size_t j = 0; j < i1; j++){
        s[j] = in1[j];
    }

    for(size_t j = x; j <= i2; j++){
        s[i1 + j - x] = in2[j];
     }

    return s;
}
