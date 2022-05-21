#include "../headers/Matrix.h"


Matrix::Matrix(size_t dim1, size_t dim2) : m_Dim1(dim1), m_Dim2(dim2) {
	m_Data = new float[dim1 * dim2];
}

Matrix::~Matrix() {
	delete m_Data;
}

size_t Matrix::getDim1() const {
	return m_Dim1;
}

size_t Matrix::getDim2() const {
	return m_Dim2;
}

Vector Matrix::getHorizontalSlice(size_t i) {
	float* ptr = m_Data + i * m_Dim2;
	return Vector(ptr, m_Dim2, 1);
}

Vector Matrix::getVerticalSlice(size_t i) {
	float* ptr = m_Data + i;
	return Vector(ptr, m_Dim1, m_Dim2);
}

float Matrix::get(size_t i, size_t j) const {
	return m_Data[i * m_Dim2 + j];
}

void Matrix::set(size_t i, size_t j, float val) {
	m_Data[i * m_Dim2 + j] = val;
}

void Matrix::transpose() {
	for (size_t i = 0; i < m_Dim1; i++) {
		for (size_t j = 0; j < m_Dim2; j++) {
			m_Data[i * m_Dim2 + j] = m_Data[j * m_Dim2 + i];
		}
	}
}

void Matrix::print() const {
	for (size_t i = 0; i < m_Dim1; i++) {
		printf("[ ");
		for (size_t j = 0; j < m_Dim2; j++) {
			printf("%.3f\t", get(i, j));
		}
		printf(" ]\n");
	}
}