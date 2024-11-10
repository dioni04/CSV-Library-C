#ifndef FILTER_H
#define FILTER_H

#include "io.h"

void eq(DATA* csv, unsigned int j, char* variable);

void neq(DATA* csv, unsigned int j, char* variable);

void lt(DATA* csv, unsigned int j, char* variable);

void lte(DATA* csv, unsigned int j, char* variable);

void gt(DATA* csv, unsigned int j, char* variable);

void gte(DATA* csv, unsigned int j, char* variable);

#endif //FILTER_H
