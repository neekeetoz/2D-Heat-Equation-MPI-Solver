#include "../headers/MpiSolverFast.h"
#include "../headers/Math.h"
#include "mpi.h"



MpiSolverFast::MpiSolverFast(DifferenceScheme scheme) : Solver(scheme) {
	MPI_Comm_rank(MPI_COMM_WORLD, &m_ProcessId);
	MPI_Comm_size(MPI_COMM_WORLD, &m_NProcesses);

	m_DataExchangeBuff = new Matrix(scheme.getDim(), scheme.getDim());
}

MpiSolverFast::~MpiSolverFast() {
	delete m_DataExchangeBuff;
}

const Matrix* MpiSolverFast::computePseudoLayer() {
	size_t pBlockSize = m_Scheme.getDim() / m_NProcesses;
	size_t start = pBlockSize * m_ProcessId;
	size_t end = pBlockSize * (m_ProcessId + 1);

	if (start == 0)
		start++;

	if (end == m_Scheme.getDim())
		end--;

	for (size_t i = start; i < end; i++) {
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

const Matrix* MpiSolverFast::refinePseudoLayer() {
	size_t pBlockSize = m_Scheme.getDim() / m_NProcesses;
	size_t start = pBlockSize * m_ProcessId;
	size_t end = pBlockSize * (m_ProcessId + 1);

	if (start == 0)
		start++;

	if (end == m_Scheme.getDim())
		end--;

	for (size_t i = start; i < end; i++) {
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

const Matrix* MpiSolverFast::computeNextLayer() {
	computePseudoLayer();
	MPISynchronizePseudoLayer();
	refinePseudoLayer();
	MPISynchronizeMainLayer();
	return m_MainLayer;
}

void MpiSolverFast::MPISynchronizePseudoLayer() {
	size_t pBlockSize = m_Scheme.getDim() / m_NProcesses;
	size_t nReceiveElements = pBlockSize * m_Scheme.getDim();
	MPI_Allgather(
		m_PseudoLayer->getDataPtr() + nReceiveElements * m_ProcessId, nReceiveElements, MPI_FLOAT,
		m_PseudoLayer->getDataPtr(), nReceiveElements, MPI_FLOAT, MPI_COMM_WORLD);
}

void MpiSolverFast::MPISynchronizeMainLayer() {
	size_t pBlockSize = m_Scheme.getDim() / m_NProcesses;
	size_t start = pBlockSize * m_ProcessId;
	size_t end = pBlockSize * (m_ProcessId + 1);

	for (size_t i = start; i < end; i++) {
		for (size_t j = 0; j < m_Scheme.getDim(); j++)
			m_DataExchangeBuff->set(i, j, m_MainLayer->get(j, i));
	}

	size_t nReceiveElements = pBlockSize * m_Scheme.getDim();

	MPI_Allgather(
		m_DataExchangeBuff->getDataPtr() + nReceiveElements * m_ProcessId, nReceiveElements, MPI_FLOAT,
		m_DataExchangeBuff->getDataPtr(), nReceiveElements, MPI_FLOAT, MPI_COMM_WORLD);

	transposeTiled(m_DataExchangeBuff->getDataPtr(), m_MainLayer->getDataPtr(), m_MainLayer->getDim1(), 16);
}

