cmake_minimum_required(VERSION 3.22)
project(hce)

set(CMAKE_CXX_STANDARD 14)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(hce MPI/pi.cpp f.cpp MPI/matrix_multiplication.cpp MPI/quicksort.cpp OpenMP/pi.cpp OpenMP/quicksort.cpp OpenMP/matrix.cpp OpenMP/matrix.h)
target_link_libraries(hce PUBLIC MPI::MPI_CXX)
