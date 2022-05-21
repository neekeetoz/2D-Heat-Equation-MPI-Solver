#pragma once
#include "Vector.h"


class Matrix {
public:
	Matrix(size_t dim1, size_t dim2);
	~Matrix();

	size_t getDim1() const;
	size_t getDim2() const;
	size_t getSize() const { return m_Dim1 * m_Dim2; };

	Vector getHorizontalSlice(size_t i);
	Vector getVerticalSlice(size_t i);

	float get(size_t i, size_t j) const;
	void set(size_t i, size_t j, float val);
	float* getDataPtr() { return m_Data; };

	// Only for squre matrices
	void transpose();

	void print() const;

private:
	float* m_Data;
	size_t m_Dim1;
	size_t m_Dim2;
};