#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct date_t
{
    int day;
    int month;
    int year;
} date_t;


void printMatrix(int n, int matrix[n][n]);
int randomInt(int min, int max);



date_t getCurrentDate();

#endif // UTIL_H