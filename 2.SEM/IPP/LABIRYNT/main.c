#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include "input.h"
#include "output.h"
#include "bfs.h"

int main(){
    vector sizes;
    vector beginning;
    vector end;
    bitset walls;

    initVector(&sizes);
    initVector(&beginning);
    initVector(&end);
    initBitset(&walls);

    int err = readData(&beginning, &end, &walls, &sizes);

    if(err < 6){
        error(err, &sizes, &beginning, &end, &walls);
    }

    size_t result;

    err = bfs(&beginning, &end, &walls, &sizes, &result);

    if(err < 6){
        error(err, &sizes, &beginning, &end, &walls);
    }

    printOutput(&result, err);

    freeVector(&sizes);
    freeVector(&beginning);
    freeVector(&end);
    freeBitset(&walls);
    
    return 0;
}