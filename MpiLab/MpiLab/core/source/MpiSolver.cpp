#include "../headers/MpiSolver.h"
#include "../headers/ThomasAlgorithm.h"
#include "mpi.h"



MpiSolver::MpiSolver(DifferenceScheme scheme) : Solver(scheme) {
	MPI_Comm_rank(MPI_COMM_WORLD, &m_ProcessId);
	MPI_Comm_size(MPI_COMM_WORLD, &m_NProcesses);

	m_DataExchangeBuff = new Matrix(scheme.getDim(), scheme.getDim());
}

MpiSolver::~MpiSolver() {
	delete m_DataExchangeBuff;
}

const Matrix* MpiSolver::computePseudoLayer() {
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

const Matrix* MpiSolver::refinePseudoLayer() {
	size_t pBlockSize = m_Scheme.getDim() / m_NProcesses;
	size_t start = pBlockSize * m_ProcessId;
	size_t end = pBlockSize * (m_ProcessId + 1);
	// 
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

const Matrix* MpiSolver::computeNextLayer() {
	computePseudoLayer();
	MPISynchronizePseudoLayer();
	refinePseudoLayer();
	MPISynchronizeMainLayer();
	return m_MainLayer;
}


void MpiSolver::MPISynchronizePseudoLayer() {
	size_t pBlockSize = m_Scheme.getDim() / m_NProcesses;
	size_t nReceiveElements = pBlockSize * m_Scheme.getDim();
	MPI_Allgather(
		m_PseudoLayer->getDataPtr() + nReceiveElements * m_ProcessId, nReceiveElements, MPI_FLOAT,
		m_PseudoLayer->getDataPtr(), nReceiveElements, MPI_FLOAT, MPI_COMM_WORLD);
}

#define IND(row,col,n) ((row)*n+(col))

void transpose_tiled(float* A, float* B, int N, int tile_size) {
	for (int i = 0; i < N; i += tile_size) {
		for (int j = 0; j < N; j += tile_size) {
			for (int p = 0; p < tile_size; p++) {
				for (int q = 0; q < tile_size; q++) {
					B[IND(i + p, j + q, N)] = A[IND(j + q, i + p, N)];
				}
			}
		}
	}
}

void MpiSolver::MPISynchronizeMainLayer() {
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

	// Could try a tiled matrix transposition
	/*for (size_t i = 0; i < m_Scheme.getDim(); i++) {
		for (size_t j = 0; j < m_Scheme.getDim(); j++)
			m_MainLayer->set(j, i, m_DataExchangeBuff->get(i, j));
	}*/
	// Need to chose optimal tile size
	transpose_tiled(m_DataExchangeBuff->getDataPtr(), m_MainLayer->getDataPtr(), m_MainLayer->getDim1(), 16);
}

