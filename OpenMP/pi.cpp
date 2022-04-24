#include <iostream>
#include <iomanip>
#include "../f.cpp"
#include <chrono>

using namespace std;

int main(int argc, char **argv)
{
    int i, n, rank;
    double x, pi, pi_i, divider, start, end;
    start = clock();
    if (rank == 0)
    {
        n = 10000000;
    }
    divider = 1 / ((double)n);

#pragma omp parallel for num_threads(2)
    for (i = 0; i < n-1; i += 1)
    {
        x = ((double)i + 0.5)*divider;
        pi_i = 4 / (1 + x * x);
        pi += pi_i * divider;
    }

    end = clock();

    std::cout << "pi: " << std::fixed << std::setprecision(8) << pi << std::endl;
    std::cout << "time: " << ((float) end - start) << std::endl;

    return 0;
}
