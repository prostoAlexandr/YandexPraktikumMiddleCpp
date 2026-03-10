#include <cstring>
#include <iostream>
#include <utility>

class Matrix {
public:
    Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(new int[rows * cols]{}) {}

    ~Matrix() { delete[] data_; }

    Matrix(const Matrix &rhd) : rows_(rhd.rows_), cols_(rhd.cols_), data_(new int[rhd.rows_ * rhd.cols_]) {
        std::memcpy(data_, rhd.data_, rows_ * cols_ * sizeof(int));
    }

    Matrix &operator=(const Matrix &rhd) {
        if (this != &rhd) {
            delete[] data_;

            rows_ = rhd.rows_;
            cols_ = rhd.cols_;

            data_ = new int[rhd.rows_ * rhd.cols_];
            std::memcpy(data_, rhd.data_, rows_ * cols_ * sizeof(int));
        }
        return *this;
    }

    // Ваш код здесь
    Matrix(Matrix &&rhd) noexcept : rows_(rhd.rows_), cols_(rhd.cols_), data_(std::exchange(rhd.data_, nullptr)) {}

    Matrix &operator=(Matrix &&rhd) noexcept {
        if (&rhd == this) {
            return *this;
        }

        using std::swap;
        swap(*this, rhd);
        return *this;
    }

    void swap(Matrix &rhd) noexcept {
        size_t rows_tmp = rows_;
        rows_ = rhd.rows_;
        rhd.rows_ = rows_tmp;

        size_t cols_tmp = cols_;
        cols_ = rhd.cols_;
        rhd.cols_ = cols_tmp;

        int *data_tmp = data_;
        data_ = rhd.data_;
        rhd.data_ = data_tmp;
    }

    friend void swap(Matrix &lhd, Matrix &rhd) { lhd.swap(rhd); }

private:
    size_t rows_;
    size_t cols_;
    int *data_;
};

int main() {
    Matrix m(3, 4);
    Matrix n(4, 3);
    m = std::move(n);
}