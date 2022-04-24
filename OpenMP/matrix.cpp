#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc51-cpp"
#pragma ide diagnostic ignored "cert-msc50-cpp"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <concepts>

#include "matrix.h"

double rand_double(double min, double max) {
    double value = (double) rand() / RAND_MAX;
    return min + value * (max - min);
}

template<std::floating_point T>
basic_matrix<T> operator*(basic_matrix<T> &lhs, basic_matrix<T> &rhs) {
    assert(lhs.get_dimensions().cols == rhs.get_dimensions().rows);

    const int rows = lhs.get_dimensions().rows;
    const int cols = rhs.get_dimensions().cols;

    basic_matrix<T> result(rows, cols, 0);

#pragma omp parallel for shared(rows, cols, rhs, lhs, result) default(none)
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < cols; j++) {
            T sum = 0;

#pragma omp parallel for reduction(+:sum) shared(cols, rhs, lhs, i, j) default(none)
            for (unsigned int k = 0; k < rhs.get_dimensions().rows; k++) {
                sum += lhs[i][k] * rhs[k][j];
            }

            // Здесь не должно быть гонки
            result[i][j] += sum;
        }
    }

    return result;
}

void randfill(basic_matrix<double> &operand) {
#pragma omp parallel for shared(operand) default(none)
    for (unsigned int i = 0; i < operand.get_dimensions().rows; i++)
        for (unsigned int j = 0; j < operand.get_dimensions().cols; j++)
            operand[i][j] = rand_double(-10, 10);
}

template<std::floating_point T>
void debug_matrix(basic_matrix<T> &operand) {
    using std::cout;

    for (unsigned int i = 0; i < operand.get_dimensions().rows; i++) {
        for (unsigned int j = 0; j < operand.get_dimensions().cols; j++) {
            cout << operand[i][j] << " ";
        }

        cout << "\n";
    }
}

int main() {
    using std::cin;
    using std::cout;

    std::srand(std::time(nullptr));

    // Matrix A [nxk]
    // Matrix B [kxm]
    int n, k, m;
    cin >> n >> k >> m;

    basic_matrix<double> matrix_a(n, k);
    basic_matrix<double> matrix_b(k, m);

    randfill(matrix_a);
    randfill(matrix_b);

    auto result = matrix_a * matrix_b;

    cout << result[0][0];

    /* cout << "Matrix A:\n";
     debug_matrix(matrix_a);
     cout << "Matrix B:\n";
     debug_matrix(matrix_b);

     cout << "Result:\n";
     debug_matrix(result);*/
}

#pragma clang diagnostic pop