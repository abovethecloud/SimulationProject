#ifndef UTILS_H
#define UTILS_H

#include "globals.h"

/*
 * Function:  approx_equal
 * --------------------
 * decides if two long double values - d1 and d2 - are close enough to be considered equal
 * by checking wether the absolute value of the difference |d1 - d2| is less than a small epsilon.
 *
 *  returns: 1 if the values are approximately equal
 *           0 if the values are not approximately equal
 */
int approx_equal(long double d1, long double d2);

#endif
