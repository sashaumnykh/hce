#ifndef MATRIX_H
#define MATRIX_H

#include <cstring>
#include <concepts>

struct matrix_dimensions {
    unsigned int rows;
    unsigned int cols;

    matrix_dimensions(const unsigned int rows, const unsigned int cols) : rows(rows), cols(cols) {}
    matrix_dimensions() : matrix_dimensions(0, 0) {}

    bool operator==(matrix_dimensions &other) const {
        return rows == other.rows && cols == other.cols;
    }
};

template<typename T>
class basic_matrix {
protected:
    T *_internal;
    matrix_dimensions dims;

    [[nodiscard]] int _mem_size() const {
        return dims.cols * dims.rows;
    }

    [[nodiscard]] unsigned int resolve_addr(const unsigned int row, const unsigned int col) const {
        return row * dims.cols + col;
    }

    class _element_proxy final {
        basic_matrix<T> &ref;
        const unsigned int row_num;
        const unsigned int col_num;

    public:
        _element_proxy(basic_matrix<T> &ref, const unsigned int row_num, const unsigned int col_num) : ref(ref),
                                                                                                       row_num(row_num),
                                                                                                       col_num(col_num) {}

        _element_proxy &operator=(const T assign) {
            ref._internal[ref.resolve_addr(row_num, col_num)] = assign;

            return *this;
        }

        _element_proxy &operator+=(const T assign) {
            ref._internal[ref.resolve_addr(row_num, col_num)] += assign;

            return *this;
        }

        _element_proxy &operator-=(const T assign) {
            ref._internal[ref.resolve_addr(row_num, col_num)] -= assign;

            return *this;
        }

        _element_proxy &operator*=(const T assign) {
            ref._internal[ref.resolve_addr(row_num, col_num)] *= assign;

            return *this;
        }

        _element_proxy &operator/=(const T assign) {
            ref._internal[ref.resolve_addr(row_num, col_num)] /= assign;

            return *this;
        }

        operator T() { // NOLINT(google-explicit-constructor)
            return ref._internal[ref.resolve_addr(row_num, col_num)];
        }
    };

    class _row_proxy final {
        basic_matrix<T> &ref;
        int row_num;

    public:
        explicit _row_proxy(basic_matrix<T> &ref, const int row_num) : ref(ref), row_num(row_num) {}

        _element_proxy operator[](const unsigned int col_num) {
            return _element_proxy(ref, row_num, col_num);
        }
    };


public:
    basic_matrix(const unsigned int rows, const unsigned int cols) noexcept {
        dims = matrix_dimensions(rows, cols);
        _internal = new T[dims.rows * dims.cols];
    }

    basic_matrix(const unsigned int rows, const unsigned int cols, T init) noexcept {
        dims = matrix_dimensions(rows, cols);
        _internal = new T[dims.rows * dims.cols];

        fill(*this, init);
    }

    ~basic_matrix() noexcept {
        delete[] _internal;
    }

    basic_matrix(const basic_matrix &other) noexcept {
        dims = other.dims;
        std::memcpy(_internal, other._internal, _mem_size());
    }

    basic_matrix(basic_matrix &&other) noexcept {
        dims = other.dims;
        _internal = other._internal;
        other._internal = nullptr;
    }

    basic_matrix &operator=(const basic_matrix &other) noexcept {
        if (this == &other)
            return *this;

        int size = _mem_size();

        T *temp = new T[size];
        std::memcpy(temp, other._internal, size);
        delete[] _internal;
        _internal = temp;

        dims = other.dims;

        return *this;
    }

    basic_matrix &operator=(basic_matrix &&other) noexcept {
        if (this == &other)
            return *this;

        delete[] _internal;
        _internal = other._internal;
        other._internal = nullptr;

        dims = other.dims;

        return *this;
    }

    _row_proxy operator[](const unsigned int row) {
        return _row_proxy(*this, row);
    }

    [[nodiscard]] matrix_dimensions get_dimensions() const {
        return dims;
    }

    template<typename U> friend void fill(basic_matrix<U> &mat, U val);
    template<std::floating_point U> basic_matrix<U> friend operator*(basic_matrix<U> &lhs, basic_matrix<U> &rhs);
    basic_matrix<double> friend operator*(basic_matrix<double> &lhs, basic_matrix<double> &rhs);
};

template<typename T>
void fill(basic_matrix<T> &mat, T val) {
#pragma omp parallel for shared(mat, val) default(none)
    for (unsigned int i = 0; i < mat.get_dimensions().rows; i++) {
        for (unsigned  int j = 0; j < mat.get_dimensions().cols; j++) {
            mat[i][j] = val;
        }
    }
}


#endif //MATRIX_H

#pragma clang diagnostic pop