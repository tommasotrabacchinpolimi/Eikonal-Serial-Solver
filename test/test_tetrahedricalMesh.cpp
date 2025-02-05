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
    const double tol = 1e-6;
    const double infinity_value = 2000;
    if(argc == 5)
    {
        // Retrieve parameters
        std::string input_fileName = argv[1];
        int num_threads = std::atoi(argv[2]);

        // Instantiating mesh
        TetrahedricalMesh<D> mesh(input_fileName);

        // Setting boundary
        std::vector<int> boundary;
        boundary.push_back(mesh.getNearestVertex(std::array<double, D>({0, 0, 0})));
        // boundary.push_back(mesh.getNearestVertex(std::array<double, D>({1, 1, 1})));

        // Setting velocity matrix
        typename Eikonal::Eikonal_traits<D, 1>::AnisotropyM M;
        M << 1, 0, 0,
             0, 1, 0,
             0, 0, 1;

        // Instantiating Eikonal Solver
        SerialEikonalSolver<D, N> serial_solver(mesh, boundary, M, tol, infinity_value);
        ParallelEikonalSolver<D, N> parallel_solver(mesh, boundary, M, num_threads, tol, infinity_value);

        // SERIAL
        auto start1 = std::chrono::high_resolution_clock::now();
        serial_solver.solve();
        auto stop1 = std::chrono::high_resolution_clock::now();

        // PARALLEL
        auto start2 = std::chrono::high_resolution_clock::now();
        parallel_solver.solve();
        auto stop2 = std::chrono::high_resolution_clock::now();

        // Performance Result Table
        std::cout << "===============================================" << std::endl;
        std::cout << "               EIKONAL SOLVER" << std::endl;
        std::cout << "Input: " << input_fileName << std::endl;
        std::cout << "Number of threads = " << num_threads << std::endl;
        std::cout << std::endl;
        std::cout << "Serial execution time = " <<
                  std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() << " microseconds." << std::endl;

        std::cout << "Parallel execution time = " <<
                  std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() << " microseconds." << std::endl;

        std::cout << "Speed Up = " << (double)std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() /
                (double)std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() << std::endl;
        std::cout << std::endl;

        /* OPTINAL: to visualize result in terminal
        std::cout << "RESULTS:  serial   parallel\n";
        for(size_t i = 0; i < serial_solver.getSolutions().size(); i++){
            std::cout << serial_solver.getSolutions()[i] << "  -  "
                      << serial_solver.getSolutions()[i] << std::endl;
        }
        std::cout << std::endl;
         */

        // Writing the output file
        std::string output_fileName = argv[4];
        std::string flag = argv[3];
        std::string fileName = "../test/output_meshes/" + output_fileName + ".vtk";
        if(flag == "s") {
            std::cout << "Serial output can be found in ../test/output_meshes" << std::endl <<
                    "with name: " << output_fileName << ".vtk" << std::endl;
            serial_solver.getSolutionsVTK(fileName);
        } else {
            std::cout << "Parallel output can be found in ../test/output_meshes" << std::endl <<
                               "with name: " << output_fileName << ".vtk" << std::endl;
           parallel_solver.getSolutionsVTK(fileName);
        }
        std::cout << "===============================================" << std::endl;
    }
    else
    {
        std::cout << "Wrong argument passed to the program\n";
    }

    return 0;
}