cmake_minimum_required(VERSION 3.0.0)
project(5 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDART 20+)
set(CMAKE_CXX_STANDART_REQUIRED true)

list(APPEND CMAKE_CXX_FLAGS "-Wall -Werror -Wsign-compare")

set(CMAKE_CXX_FLAGS_ASAN "-g --std=c++20 -fsanitize=address,undefined -fno-sanitize-recover=all -O2"
    CACHE STRING "Compiler flags in asan build"
    FORCE
)

set(CMAKE_CXX_FLAGS_SHADREL "-O2 -DNDEBUG"
    CACHE STRING "Compiler flags in release contest build"
    FORCE
)
add_executable(1 problem_A.cpp)
add_executable(3 problem_C.cpp)
add_executable(5 problem_E.cpp)
add_executable(6 problem_F.cpp)
add_executable(7 problem_G.cpp)
add_executable(8 problem_H.cpp)
add_executable(20 C.cpp)
add_executable(3_C problem_3_C.cpp)
add_executable(2_B problem_2_B.cpp)
add_executable(2_D problem_2_D.cpp)
add_executable(2 problem_B.cpp)
add_executable(3_D problem_3_D.cpp)
add_executable(3_E problem_3_E.cpp)
add_executable(p1 pointers_1.cpp)
add_executable(p2 pointers_2.cpp)

add_executable(naiv lab_1/naiv.cpp)

add_executable(dinamic lab_2/dinamic.cpp lab_2/Knapsack.hpp)
add_executable(2-approx lab_2/2-approx.cpp lab_2/Knapsack.hpp)
set_target_properties(dinamic
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lab_2"
)
set_target_properties(2-approx
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lab_2"
)