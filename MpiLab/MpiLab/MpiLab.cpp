// MpiLab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>
#include "core/headers/Matrix.h"
#include "core/headers/Solver.h"
#include "core/headers/MpiSolver.h"
#include "mpi.h"
#include <chrono>
#include <utility>
#include <iostream>

//int main()
//{
//    std::cout << "Hello World!\n";
//
//    Matrix mat(4, 4);
//    mat.print();
//
//    for (int i = 0; i < 4; i++) {
//        for (int j = 0; j < 4; j++)
//            mat.set(i, j, (float)i * j * j * j);
//    }
//
//    mat.print();
//    printf("\n");
//    mat.getHorizontalSlice(1).print();
//    printf("\n");
//    mat.getVerticalSlice(1).print();
//    printf("\n");
//
//    Vector temp = mat.getVerticalSlice(1);
//    temp[0] = 100.f;
//    mat.print();
//}


void testMpi(int argc, char* argv[]) {
    int pid;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    printf("%d \n", pid);
    DifferenceScheme scheme(1024, 0.01f, M_PI, M_PI);
    MpiSolver solver(scheme);
    
    const Matrix* result = solver.solve(10);
    if (pid == 0) {
        //result->print();
    }
}

void testSigleThread(int argc, char* argv[]) {
    DifferenceScheme scheme(10, 1.f, M_PI, M_PI);
    Solver solver(scheme);
    solver.solve(3)->print();
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    auto t1 = std::chrono::high_resolution_clock::now();
    testMpi(argc, argv);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << duration / 1000;
    MPI_Finalize();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
