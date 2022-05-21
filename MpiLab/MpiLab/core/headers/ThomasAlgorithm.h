#pragma once
#include "Vector.h"

/*
* Thomas algorithm for solving three diagonal systems of linear equations.
* 
* All the vectors must have the same size.
* 
* @param[in] a
* Vector of upper diagonal coefficients.
* 
* @param[in] b
* Vector of diagonal coefficients.
* 
* @param[in] d
* Vector of diagonal coefficents.
* 
* @param[out]
* Vector where to the results.
*/
void thomasAlgorithm(Vector& a, Vector& b, Vector& c, Vector& d, Vector& out);