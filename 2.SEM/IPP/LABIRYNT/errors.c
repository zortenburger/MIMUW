#include "errors.h"

// funkcja zwracajaca error na wyjscie zwalniajaca pamiec i konczy program exit 1
void error(int e, vector *sizes, vector *beginning, vector *end, bitset *walls){
    fprintf(stderr, "ERROR %d\n", e);

    freeVector(sizes);
    freeVector(beginning);
    freeVector(end);
    freeBitset(walls);
    
    exit(1);
}