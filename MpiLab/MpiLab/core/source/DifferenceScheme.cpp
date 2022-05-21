#include "../headers/DifferenceScheme.h"
#include <exception>
#include <stdexcept>
#include <string>
#include <math.h>

/*
* This file contains an implementation of a difference scheme for my particular 
* heat equation system.
* 
* du/dt = d2u/dx2 + d2u/dy2
* u(x, y, 0) = sin(x) sin(y)
* u(0, y, t) = u(pi, y, t) = 0
* u(x, 0, t) = u(x, pi, t) = 0
* 
* Analytical solution:
* u(x, y, t) = sin(x) sin(y) exp(-2 t)
*/

inline void sizeCheck(size_t receivedSize, size_t expectedSize) {
	if (receivedSize != expectedSize)
		throw std::invalid_argument(
			"Expected vector of size=" + std::to_string(expectedSize) +
			" but received vec.size=" + std::to_string(receivedSize));
}

void DifferenceScheme::generateACoefficients(Vector& a) {
	sizeCheck(a.size(), m_Dim);

	a[0] = 0.f;
	a[m_Dim - 1] = 0.f;

	for (int i = 1; i < m_Dim - 1; i++)
		a[i] = m_Tau / m_SpatialStepSquared / 2.f;
}

void DifferenceScheme::generateBCoefficients(Vector& b) {
	sizeCheck(b.size(), m_Dim);

	b[0] = 1.f;
	b[m_Dim - 1] = 1.f;

	for (int i = 1; i < m_Dim - 1; i++)
		b[i] = -(1.f + m_Tau / m_SpatialStepSquared);
}

void DifferenceScheme::generateCCoefficients(Vector& c) {
	sizeCheck(c.size(), m_Dim);

	c[0] = 0.f;
	c[m_Dim - 1] = 0.f;

	for (int i = 1; i < m_Dim - 1; i++)
		c[i] = m_Tau / m_SpatialStepSquared / 2.f;
}

inline float DifferenceScheme::derivative2ndOrder(float left, float mid, float right) const {
	return (left - 2.f * mid + right) / m_SpatialStepSquared;
}


void DifferenceScheme::generateDCoefficients(const Vector& left, const Vector& mid, const Vector& right, Vector& d) {
	//sizeCheck(d.size(), m_Dim);

	d[0] = 0.f;
	d[m_Dim - 1] = 0.f;

	for (int i = 1; i < m_Dim - 1; i++)
		d[i] = -mid[i] - derivative2ndOrder(left[i], mid[i], right[i]) * m_Tau / 2.f;
}

float DifferenceScheme::initialCondition(float x, float y) {
	return sin(x) * sin(y);
}

float DifferenceScheme::initialCondition(size_t i, size_t j) {
	return sin(i * m_SpatialStep) * sin(j * m_SpatialStep);
}