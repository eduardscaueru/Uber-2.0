//SCAUERU EDUARD 313CD
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "include/project.h"

int *arrayProduct(int *array, int arrayLength)
{
    int *v = malloc(arrayLength * sizeof(int)), i, p = 1;

    for(i = 0; i < arrayLength; i++)
    {
        p *= array[i];
    }
    for(i = 0; i < arrayLength; i++)
    {
        v[i] = p / array[i];
    }

    return v;
}

int **rotateMatrix(int **matrix, int matrixSize)
{
    int **m= malloc(matrixSize * sizeof(int*)), i, j, ii, jj;

    for(i = 0; i < matrixSize; i++)
    {
        m[i] = malloc(matrixSize * sizeof(int));
    }

    jj = 0;
    for(i = 0; i < matrixSize; i++)
    {
        ii = matrixSize - 1;
        for(j = 0; j < matrixSize; j++)
        {
            m[ii][jj] = matrix[i][j];
            ii--;
        }
        jj++;
    }

    return m;
}

int *subMatrixesSums(int **matrix, int queriesNo, int *queries)
{
    int *sums = malloc(queriesNo * sizeof(int)), nr = 0, i, j ,k = 0, s, x1, y1, x2, y2;

    while(k < queriesNo * 4)
    {
        s = 0;
        x1 = queries[k];
        y1 = queries[k+1];
        x2 = queries[k+2];
        y2 = queries[k+3];
        k += 4;
        for(i = x1; i <= x2 ; i++)
        {
            for(j = y1; j <= y2; j++)
            {
                s += matrix[i][j];
            }
        }
        sums[nr++] = s;
    }

    return sums;
}

TDriver *allocDriver(int ridesNo)
{
    TDriver *driver = (TDriver*)malloc(sizeof(TDriver));

    driver->rides = (TRide**)malloc(ridesNo * sizeof(TRide*));
    for(int i = 0; i < ridesNo; i++)
    {
        driver->rides[i] = (TRide*)malloc(sizeof(TRide));
    }

    return driver;
}

TDriver **allocDrivers(int driversNo, int *driversRidesNo)
{
    TDriver **drivers = (TDriver**)malloc(driversNo * sizeof(TDriver*));

    for(int i = 0; i < driversNo; i++)
    {
        drivers[i] = allocDriver(driversRidesNo[i]);
    }

    return drivers;
}

TDriver **readDrivers(FILE *inputFile, int *driversNo)
{
    unsigned int *v;
    int i, j;

    fread(driversNo, sizeof(int), 1, inputFile);
    v = (unsigned int*)malloc(*driversNo * sizeof(unsigned int));
    fread(v, sizeof(unsigned int), *driversNo, inputFile);
    TDriver **drivers = allocDrivers(*driversNo, v);

    for(i = 0; i < *driversNo; i++)
    {
        drivers[i]->nrc = v[i];
        fread(drivers[i]->nume, sizeof(char) * 20, 1, inputFile);
        fread(drivers[i]->nrmasina, sizeof(char) * 8, 1, inputFile);
        fread(&drivers[i]->x, sizeof(double), 1, inputFile);
        fread(&drivers[i]->y, sizeof(double), 1, inputFile);
        for(j = 0; j < v[i]; j++)
        {
            fread(&drivers[i]->rides[j]->x, sizeof(double), 1, inputFile);
            fread(&drivers[i]->rides[j]->y, sizeof(double), 1, inputFile);
            fread(&drivers[i]->rides[j]->stare, sizeof(unsigned char), 1, inputFile);
        }
    }
    free(v);

    return drivers;
}

void printDrivers(FILE *outputFile, TDriver **drivers, int driversNo)
{
    for(int i = 0; i < driversNo; i++)
    {
        fwrite(drivers[i]->nume, sizeof(char) * 20, 1, outputFile);
        fwrite(drivers[i]->nrmasina, sizeof(char) * 8, 1, outputFile);
        fwrite(&drivers[i]->x, sizeof(double), 1, outputFile);
        fwrite(&drivers[i]->y, sizeof(double), 1, outputFile);
        for(int j = 0; j < drivers[i]->nrc; j++)
        {
            fwrite(&drivers[i]->rides[j]->x, sizeof(double), 1, outputFile);
            fwrite(&drivers[i]->rides[j]->y, sizeof(double), 1, outputFile);
            fwrite(&drivers[i]->rides[j]->stare, sizeof(unsigned char), 1, outputFile);
        }
    }
}

char *maxRatingDriverName(TDriver **drivers, int driversNo)
{
    int i, j, x, stars, nr = 0;
    double rating, _rating, rides;

    rating = -1;
    _rating = -2;
    for(i = 0; i < driversNo; i++)
    {
        stars = 0;
        rides = 0;
        for(j = 0; j < drivers[i]->nrc; j++)
        {
            x = drivers[i]->rides[j]->stare;
            stars += GET_STARS(x);
            rides += CANCELED_RIDE(x)/8 + ONGOING_RIDE(x)/16;
        }
        rating = stars / (drivers[i]->nrc - rides);
        if(rating > _rating)
        {
            _rating = rating;
            nr = i;
        }
    }
    
    return drivers[nr]->nume;
}

TDriver **getClosestDrivers(TDriver **drivers, int driversNo, double desiredX,
    double desiredY, int resultsNo)
{
    TDriver **sol = malloc(resultsNo * sizeof(TDriver*));
    int i, k, nr, *f = calloc(driversNo, sizeof(int));
    double dist, _dist;

    for(k = 0; k < resultsNo; k++)
    {
        nr = 0;
        _dist = 100000;
        for(i = 0; i < driversNo; i++)
        {
            if(f[i] == 0)
            {
                dist = (drivers[i]->x - desiredX) * (drivers[i]->x - desiredX) + (drivers[i]->y - desiredY) * (drivers[i]->y - desiredY);
                if(dist < _dist)
                {
                    _dist = dist;
                    nr = i;
                }
                else
                {
                    if(dist == _dist)
                    {
                        if(strcmp(drivers[i]->nume, drivers[nr]->nume) < 0)
                        {
                            nr = i;
                        }
                    }
                }
            }
        }
        sol[k] = drivers[nr];
        f[nr]++;
    }
    free(f);
    
    return sol;
}

void freeDriver(TDriver *driver)
{
    for(int i = 0; i < driver->nrc; i++)
    {
        free(driver->rides[i]);
    }
    free(driver->rides);
    free(driver);
}

void freeDrivers(TDriver **drivers, int driversNo)
{
    for(int i = 0; i < driversNo; i++)
    {
        freeDriver(drivers[i]);
    }
    free(drivers);
}
