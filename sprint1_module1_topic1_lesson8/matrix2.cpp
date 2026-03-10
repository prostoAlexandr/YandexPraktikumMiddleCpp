#include <cstring>
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

    Matrix(Matrix &&rhd) noexcept
        : rows_(std::exchange(rhd.rows_, 0)), cols_(std::exchange(rhd.cols_, 0)),
          data_(std::exchange(rhd.data_, nullptr)) {}

    Matrix &operator=(Matrix &&rhd) noexcept {
        if (this != &rhd) {
            rows_ = std::exchange(rhd.rows_, 0);
            cols_ = std::exchange(rhd.cols_, 0);
            delete[] data_;
            data_ = std::exchange(rhd.data_, nullptr);
        }
        return *this;
    }

    void swap(Matrix &rhd) {
        size_t temp_rows = rows_;
        size_t temp_cols = cols_;
        int *temp_data = data_;

        rows_ = rhd.rows_;
        cols_ = rhd.cols_;
        data_ = rhd.data_;

        rhd.rows_ = temp_rows;
        rhd.cols_ = temp_cols;
        rhd.data_ = temp_data;
    }

    friend void swap(Matrix &lhd, Matrix &rhd) { lhd.swap(rhd); }

    // Copy-And-Swap — создаём итоговый результат во временном объекте, а в конце перемещаем его на место оригинального
    // объекта
    void Extend(const Matrix &rhd) {
        Matrix res{rows_ * cols_, rhd.rows_ * rhd.cols_};

        for (size_t i = 0; i < rhd.rows_ * rhd.cols_; ++i) {
            /* код для увеличения размера матрицы с использованием другой матрицы */
        }

        this->swap(res);
    }

    // Copy-And-Swap — создаём итоговый результат во временном объекте, а в конце перемещаем его на место оригинального
    // объекта
    void Shrink(const Matrix &rhd) {
        Matrix res{rows_ * cols_ / 2, rhd.rows_ * rhd.cols_ / 2};

        for (size_t i = 0; i < rhd.rows_ * rhd.cols_ / 2; ++i) {
            /* код для уменьшения размера матрицы с использованием другой матрицы */
        }

        this->swap(res);
    }

    size_t rows_;
    size_t cols_;
    int *data_;
};

Matrix MultiplyMatrix(const Matrix m1, const Matrix m2) {
    Matrix res{m1.cols_, m2.rows_};

    /* код перемножения матриц */

    return res;
}

int main() {
    Matrix m1(3, 5);
    Matrix m2(5, 3);

    /* код заполнения матриц значениями */

    Matrix m3 = MultiplyMatrix(std::move(m1), std::move(m2));

    return 0;
}