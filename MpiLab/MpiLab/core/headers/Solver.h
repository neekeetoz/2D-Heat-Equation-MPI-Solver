#pragma once
#include "DifferenceScheme.h"
#include "Matrix.h"
#include <memory>


class Solver {
public:
	Solver(DifferenceScheme scheme);
	~Solver();

	const Matrix* getMainLayer() { return m_MainLayer; };
	const Matrix* getPseudoLayer() { return m_PseudoLayer; };
	const Vector* getACoefficients() { return m_A; };
	const Vector* getBCoefficients() { return m_B; };
	const Vector* getCCoefficients() { return m_C; };
	const Vector* getDCoefficients() { return m_D; };

	virtual const Matrix* computePseudoLayer();
	virtual const Matrix* refinePseudoLayer();
	virtual const Matrix* computeNextLayer();
	const Matrix* solve(int t);

	void reset();

protected:
	void prepareABC();

protected:
	DifferenceScheme m_Scheme;
	Matrix* m_MainLayer;
	Matrix* m_PseudoLayer;

	Vector* m_A;
	Vector* m_B;
	Vector* m_C;
	Vector* m_D;
};