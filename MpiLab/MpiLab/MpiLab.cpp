// MpiLab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>
#include "core/headers/Matrix.h"
#include "core/headers/Solver.h"
#include "core/headers/MpiSolverFast.h"
#include "mpi.h"
#include <chrono>
#include <utility>
#include <iostream>
#include <stdlib.h>

int DEFAULT_SCHEME_DIM = 256;
int N_TEST_CYCLES = 50;


void testMpi(int argc, char* argv[]) {
    int schemeDim = DEFAULT_SCHEME_DIM;
    if (argc > 1)
        schemeDim = atoi(argv[1]);

    DifferenceScheme scheme(schemeDim, 0.01f, M_PI, M_PI);
    MpiSolverFast solver(scheme);
    
    const Matrix* result = solver.solve(10);
}

void testSigleThread(int argc, char* argv[]) {
    int schemeDim = DEFAULT_SCHEME_DIM;
    if (argc > 1)
        schemeDim = atoi(argv[1]);

    DifferenceScheme scheme(schemeDim, 0.01f, M_PI, M_PI);
    Solver solver(scheme);
    solver.solve(10);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int pid;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    if (argc > 1 && pid == 0)
        std::cout << "Received scheme dim = " << argv[1] << std::endl;

    long long duration = 0;
    for (int i = 0; i < N_TEST_CYCLES; i++) {
        auto t1 = std::chrono::high_resolution_clock::now();
        testMpi(argc, argv);
        auto t2 = std::chrono::high_resolution_clock::now();
        duration += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    }

    if (pid == 0)
        std::cout << "Execution time (ms): " << (double)duration / 1000 / N_TEST_CYCLES << std::endl;

    MPI_Finalize();
}