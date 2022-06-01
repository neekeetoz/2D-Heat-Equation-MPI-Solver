#include "../headers/Vector.h"
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>


Vector::Vector(size_t n) : 
	m_Size(n), m_IsMemoryOwner(true), m_Stride(1) {
	m_Data = new float[n];
}

Vector::Vector(float* data, size_t n, size_t stride) :
	m_Data(data), m_Size(n), m_IsMemoryOwner(false), m_Stride(stride) {}

Vector::~Vector() {
	if (m_IsMemoryOwner)
		delete m_Data;
}

void Vector::print() const {
	printf("[ ");
	for (size_t i = 0; i < m_Size; i++)
		printf("%1.3f ", operator[](i));
	printf(" ]");
}

float& Vector::operator[](size_t i) {
#if defined _DEBUG
	if (i > m_Size)
		throw std::out_of_range(
			"Index " + std::to_string(i) + " is out of range=" + std::to_string(m_Size));
#endif
	return m_Data[m_Stride * i];
}

float Vector::operator[](size_t i) const {
#if defined _DEBUG
	if (i > m_Size)
		throw std::out_of_range(
			"Index " + std::to_string(i) + " is out of range=" + std::to_string(m_Size));
#endif
	return m_Data[m_Stride * i];
}