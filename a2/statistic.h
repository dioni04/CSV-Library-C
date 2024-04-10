#ifndef STATISTIC_H
#define STATISTIC_H

#include "io.h"
#include "list.h"


float aritMean(float* array, unsigned int size);

NODE* modeN(float* array, NODE** list, unsigned int size);
NODE* modeS(char** array, NODE** list, unsigned int size);

float median(float* array, unsigned int size);
float stdDeviation(float* array, float mean, unsigned int size);

float min(float* array, unsigned int size);
float max(float* array, unsigned int size);

#endif //STATISTIC_H
