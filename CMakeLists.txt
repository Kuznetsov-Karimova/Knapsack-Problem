set(CMAKE_MAKE_PROGRAM make)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDART_REQUIRED true)

cmake_minimum_required(VERSION 3.15)
project(Knapsack-problem CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(dinamic src/dinamic.cpp src/Knapsack.hpp)
add_executable(2-approx src/2-approx.cpp src/Knapsack.hpp)
add_executable(ptas src/ptas.cpp src/Knapsack.hpp)
add_executable(branch_and_bound src/branch_and_boundary_m.cpp src/Knapsack.hpp)