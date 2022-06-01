#pragma once
#define IND(row,col,n) ((row)*n+(col))

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


/*
* Cache efficient matrix transposition. Works only for square matrices!
* 
* @param[in] A
* Matrix to transpose.
* 
* @param[out] B
* Trasposition result will be written here.
* 
* @param[in] N
* Size of the matrix (number of columns).
* 
* @param[in] tile_size
* Tile size.
*/
void transposeTiled(float* A, float* B, int N, int tile_size);