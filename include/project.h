#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <stdio.h>

#pragma pack(1)

typedef struct {
double x, y;
unsigned char stare;
} TRide;

typedef struct {
char nume[20], nrmasina[8];
double x, y;
unsigned int nrc;
TRide **rides;
} TDriver;

#pragma pack()

/** First 3 bits will hold the number of stars
 * 4th bit shows us if a ride is canceled or not
 * 5th bit shows us if a ride is ongoing or not
 */
#define GET_STARS(state) (state & 7)
#define CANCELED_RIDE(state) (state & 8)
#define ONGOING_RIDE(state) (state & 16)

/** Returns an array with n products, each missing the i-th element of the array.
 *  outputFile  open text file for writing
 *  array       array containing @arrayLength integer elements
 *  arrayLength length of @array
 * 	@returns 	a newly allocated array, with the desired products
 */
int *arrayProduct(int *array, int arrayLength);

/** Rotates a given matrix 90 degrees to the left.
 *  matrix      square matrix of size @matrixSize
 *  matrixSize  number of lines and columns in @matrix
 *  @returns    a newly allocated matrix, with rotated the elements of @matrix
 */
int **rotateMatrix(int **matrix, int matrixSize);

/** Returns an array with the result of n queries, each asking for a submatrix sum.
 *  outputFile  open text file for writing
 *  matrix      matrix
 *  queriesNo   number of queries
 *  queries     queries of format <x1, y1, x2, y2>; x - lines; y - columns
 *              eg: first query occupies the first four indexes of the array
 *	@returns 	a newly allocated array, with the desired subsums
 */
int *subMatrixesSums(int **matrix, int queriesNo, int *queries);

/** Allocates a TDriver element dynamically.
 *  ridesNo     number of rides
 *  @returns    driver element
 */
TDriver *allocDriver(int ridesNo);

/** Allocates an array of TDriver elements dynamically.
 *  driversNo       number of drivers
 *  driversRidesNo  array containing the number of rides for each driver
 *  @returns        array of drivers
 */
TDriver **allocDrivers(int driversNo, int *driversRidesNo);

/** Reads the drivers from a binary file. First reads the number of drivers and
 * then the array of rides. Then, reads each driver.
 *  inputFile   open binary file for reading
 *  driversNo   parameter to return the number of drivers
 *  @returns    array of driver elements
 */
TDriver **readDrivers(FILE *inputFile, int *driversNo);

/** Prints the drivers into a binary file.
 *  outputFile  open binary file for writing
 *  drivers     array of driver elements to be printed
 *  driversNo   number of drivers
 */
void printDrivers(FILE *outputFile, TDriver **drivers, int driversNo);

/** Returns the name of the driver with the highest rating.
 *  drivers     array of driver elements
 *  driversNo   number of drivers
 *  @returns    name of the driver with the most stars
 */
char *maxRatingDriverName(TDriver **drivers, int driversNo);

/** Returns an array of the closest n drivers to a coordinate.
 *  drivers     array of driver elements
 *  driversNo   number of drivers
 *  desiredX    OX axis coordinate
 *  desiredY    OY axis coordinate
 *  resultsNo   length of returned array
 *  @returns    a new array of pointers to the closest @resultsNo drivers to
 *              (@desiredX, @desiredY)
 */
TDriver **getClosestDrivers(TDriver **drivers, int driversNo, double desiredX,
    double desiredY, int resultsNo);

/** Frees a driver.
 *  driver  driver
 */
void freeDriver(TDriver *driver);

/** Frees an array of driver elements.
 *  drivers     array of driver elements
 *  driversNo   number of drivers
 */
void freeDrivers(TDriver **drivers, int driversNo);

#endif // _PROJECT_H_
