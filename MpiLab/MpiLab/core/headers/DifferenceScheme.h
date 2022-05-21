#pragma once
#include "Vector.h"

/**
* Represents a 2D difference scheme for a heat equation.
* It is assummed that the heat domain is a square.
* 
* Its interface is adapted to Thomas algorithm.
*/
class DifferenceScheme {
public:
	DifferenceScheme(size_t dim, float tau, float xSpatialBorder, float ySpatialBorder) :
		m_Dim(dim), m_Tau(tau), m_XSpatialBorder(xSpatialBorder), m_YSpatialBorder(ySpatialBorder) {
		m_SpatialStep = xSpatialBorder / (dim - 1);
		m_SpatialStepSquared = m_SpatialStep * m_SpatialStep;
	};
	~DifferenceScheme() {};

	size_t getDim() const { return m_Dim; };
	void generateACoefficients(Vector& a);
	void generateBCoefficients(Vector& b);
	void generateCCoefficients(Vector& c);
	void generateDCoefficients(const Vector& left, const Vector& mid, const Vector& right, Vector& d);

	float initialCondition(float x, float y);
	float initialCondition(size_t i, size_t j);
	/*inline float leftBoundaryCondition(float y, float t);
	inline float rightBoundaryCondition(float y, float t);
	inline float topBoundaryCondition(float x, float t);
	inline float bottomBoundaryCondition(float x, float t);*/

private:
	inline float derivative2ndOrder(float left, float mid, float right) const;

private:
	size_t m_Dim;
	float m_Tau;
	float m_XSpatialBorder;
	float m_YSpatialBorder;
	float m_SpatialStep;
	float m_SpatialStepSquared;
};