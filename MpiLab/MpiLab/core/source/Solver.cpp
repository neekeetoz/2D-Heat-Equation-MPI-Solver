#include "../headers/Solver.h"
#include "../headers/Math.h"

Solver::Solver(DifferenceScheme scheme) : m_Scheme(scheme) {
	m_MainLayer = new Matrix(scheme.getDim(), scheme.getDim());
	m_PseudoLayer = new Matrix(scheme.getDim(), scheme.getDim());

	m_A = new Vector(scheme.getDim());
	m_B = new Vector(scheme.getDim());
	m_C = new Vector(scheme.getDim());
	m_D = new Vector(scheme.getDim());

	reset();
}

Solver::~Solver() {
	delete m_MainLayer;
	delete m_PseudoLayer;

	delete m_A;
	delete m_B;
	delete m_C;
	delete m_D;
}

void Solver::reset() {
	for (size_t i = 0; i < m_Scheme.getDim(); i++) {
		for (size_t j = 0; j < m_Scheme.getDim(); j++) {
			m_MainLayer->set(i, j, m_Scheme.initialCondition(i, j));
			m_PseudoLayer->set(i, j, 0.f);
		}
	}
}

void Solver::prepareABC() {
	m_Scheme.generateACoefficients(*m_A);
	m_Scheme.generateBCoefficients(*m_B);
	m_Scheme.generateCCoefficients(*m_C);
}

const Matrix* Solver::computePseudoLayer() {
	for (size_t i = 1; i < m_Scheme.getDim() - 1; i++) {
		prepareABC();
		Vector topSlice = m_MainLayer->getHorizontalSlice(i - 1);
		Vector midSlice = m_MainLayer->getHorizontalSlice(i);
		Vector botSlice = m_MainLayer->getHorizontalSlice(i + 1);
		m_Scheme.generateDCoefficients(topSlice, midSlice, botSlice, *m_D);
		Vector pseudoSlice = m_PseudoLayer->getHorizontalSlice(i);
		thomasAlgorithm(*m_A, *m_B, *m_C, *m_D, pseudoSlice);
	}
	return m_PseudoLayer;
}

const Matrix* Solver::refinePseudoLayer() {
	for (size_t i = 1; i < m_Scheme.getDim() - 1; i++) {
		prepareABC();
		Vector lefSlice = m_PseudoLayer->getVerticalSlice(i - 1);
		Vector midSlice = m_PseudoLayer->getVerticalSlice(i);
		Vector rigSlice = m_PseudoLayer->getVerticalSlice(i + 1);
		m_Scheme.generateDCoefficients(lefSlice, midSlice, rigSlice, *m_D);
		Vector mainSlice = m_MainLayer->getVerticalSlice(i);
		thomasAlgorithm(*m_A, *m_B, *m_C, *m_D, mainSlice);
	}
	return m_MainLayer;
}

const Matrix* Solver::computeNextLayer() {
	computePseudoLayer();
	return refinePseudoLayer();
}

const Matrix* Solver::solve(int t) {
	for (int i = 0; i < t; i++)
		computeNextLayer();
	return m_MainLayer;
}