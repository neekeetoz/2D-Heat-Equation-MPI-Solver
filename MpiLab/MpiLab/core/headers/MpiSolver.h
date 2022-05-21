#pragma once
#include "Solver.h"


class MpiSolver : public Solver {
public:
	MpiSolver(DifferenceScheme scheme);
	~MpiSolver();

	const Matrix* computePseudoLayer() override;
	const Matrix* refinePseudoLayer() override;
	const Matrix* computeNextLayer() override;

private:
	void MPISynchronizeMainLayer();
	void MPISynchronizePseudoLayer();

private:
	int m_ProcessId;
	int m_NProcesses;
	Matrix* m_DataExchangeBuff;
};