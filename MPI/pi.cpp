#include <iostream>
#include <mpi.h>
#include <iomanip>
#include "../f.cpp"
#include <chrono>

using namespace std;

int main(int argc, char **argv)
{
    int i, n, size, rank;
    double x, pi, pi_i, rez, divider, start, end;
    double sum = 0.0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
    {
        n = 1000000;
        start = MPI_Wtime();
    }
    divider = 1 / ((double)n);
    for (i = rank; i < n-1; i += size)
    {
        x = ((double)i + 0.5)*divider;
        pi_i = 4 / (1 + x * x);
        sum += pi_i * divider;
    }
    rez = sum;

    MPI_Reduce(&rez, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    end = MPI_Wtime();
    if (rank == 0)
    {
        std::cout << "pi: " << std::fixed << std::setprecision(8) << pi << std::endl;
        std::cout << "time: " << end - start << std::endl;
    }
    MPI_Finalize();
    return 0;
}
