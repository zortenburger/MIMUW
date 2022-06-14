#include "output.h"

//funkcja wypisujaca najkrotsza sciezke w labiryncie 
void printNumber(size_t *number){
    printf("%zu\n", *number);
}

//funkcja wypisujaca NO WAY gdy nie ma drogi
void noWay(){
    printf("NO WAY\n");
}

//funkcja wypisujaca odpowiedz do zadania
void printOutput (size_t *number, int fl){
    if(fl == 6){
        noWay();
    }
    else {
        printNumber(number);
    }
}