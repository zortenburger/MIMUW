#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "output.h"
#include "bfs.h"
#include "input.h"

void error (int e, vector *sizes, vector *beginning, vector *end, bitset *walls); // funkcja zwracajaca error na wyjscie zwalniajaca pamiec i konczy program exit 1