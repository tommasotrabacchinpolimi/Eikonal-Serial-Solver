//
// Created by sabri on 01/12/2023.
//

#include <string>
#include <iostream>
#include <cmath>
#include <chrono>

#include "../src/TetrahedricalMesh.h"
#include "../src/SerialEikonalSolver.h"
#include "../src/ParallelEikonalSolver.h"

int main(int argc, char* argv[]){
    constexpr int N = 4;
    constexpr int D = 3;
    if(argc > 0)
    {
        // Retrieve parameters
        const std::string input_fileName = argv[1];
        int num_threads = 4;
        if(argc == 3){
            num_threads = std::atoi(argv[2]);
        } else {
            num_threads = std::atoi(argv[3]);
        }

        // Instantiating mesh
        TetrahedricalMesh<D> mesh(input_fileName);

        // Setting boundary
        std::vector<int> boundary;
        boundary.push_back(mesh.getNearestVertex(std::array<double, D>({0, 0, 0})));

        // Setting velocity matrix
        typename Eikonal::Eikonal_traits<D, 1>::MMatrix M;
        M << 1, 0, 0,
                0, 3, 0,
                0, 0, 7;

        // Instantiating Eikonal Solver
        SerialEikonalSolver<D, N> serial_solver(mesh, boundary, M);
        ParallelEikonalSolver<D, N> parallel_solver(mesh, boundary, M, num_threads);

        // SERIAL
        auto start1 = std::chrono::high_resolution_clock::now();
        serial_solver.solve_vector();
        auto stop1 = std::chrono::high_resolution_clock::now();

        // PARALLEL
        auto start2 = std::chrono::high_resolution_clock::now();
        parallel_solver.solve();
        auto stop2 = std::chrono::high_resolution_clock::now();

        // Print
        std::cout << "SOLUTION:\n";
        for(size_t i = 0 ; i < serial_solver.getSolutions().size(); i++){
            std::cout << serial_solver.getSolutions()[i] << " " << parallel_solver.getSolutions()[i] << "\n";
        }

        // Performance Result Table

        std::cout << "Execution time serial = " <<
                  std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() << std::endl;

        std::cout << "Execution time parallel = " <<
                  std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() << std::endl;

        std::cout << "speed up = " << (double)std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() /
                (double)std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() << std::endl;

        // Writing the output file
        std::string output_fileName = "output";
        if(argc == 4){
            output_fileName = argv[2];
        }
        serial_solver.getSolutionsVTK(output_fileName);
    }
    else
    {
        std::cout << "No argument passed to the program\n";
    }

    return 0;
}