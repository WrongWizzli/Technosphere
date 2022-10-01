#include "my_matrix.hpp"


My_Matrix::Matrix::ProxyRow::ProxyRow(int32_t n_cols) {
    this->n_cols = n_cols;
    column = new int32_t[n_cols];
}


My_Matrix::Matrix::ProxyRow::~ProxyRow() {
    delete[] column;
}


My_Matrix::Matrix::ProxyRow::ProxyRow(const ProxyRow &c) {
    n_cols = c.n_cols;
    column = new int32_t[n_cols];
    for (int i = 0; i < n_cols; ++i) {
        column[i] = c.column[i];
    }
}


My_Matrix::Matrix::ProxyRow& My_Matrix::Matrix::ProxyRow::operator=(const ProxyRow &c) {
    if (this == &c) {
        return *this;
    }
    if (column != nullptr) {
        delete[] column;
    }
    n_cols = c.n_cols;
    column = new int32_t[n_cols];
    for (int i = 0; i < n_cols; ++i) {
        column[i] = c.column[i];
    }
    return *this;
}


int32_t& My_Matrix::Matrix::ProxyRow::operator[](int32_t j) const {
    if (j < 0 || j >= n_cols) {
        throw std::out_of_range("");
    }
    return column[j];
}


My_Matrix::Matrix::Matrix(int32_t n_rows, int32_t n_cols) {
    this->n_rows = n_rows;
    this->n_cols = n_cols;
    mat = new ProxyRow[n_rows];
    for (int i = 0; i < n_rows; ++i) {
        mat[i] = ProxyRow(n_cols);
    }
}


My_Matrix::Matrix::Matrix(const Matrix &c) {
    n_rows = c.n_rows;
    n_cols = c.n_cols;
    mat = new ProxyRow[n_rows];
    for (int32_t i = 0; i < n_rows; ++i) {
        mat[i] = ProxyRow(n_cols);
    }
}


My_Matrix::Matrix::~Matrix() {
    delete[] mat;
}


int32_t My_Matrix::Matrix::get_rows() const {
    return n_rows;
}


int32_t My_Matrix::Matrix::get_cols() const {
    return n_cols;
}


My_Matrix::Matrix My_Matrix::Matrix::operator*=(const int32_t a) {
    for (int32_t i = 0; i < n_rows; ++i) {
        for (int32_t j = 0; j < n_cols; ++j) {
            mat[i][j] *= a;
        }
    }
    return *this;
}


My_Matrix::Matrix My_Matrix::Matrix::operator+(const Matrix &c) const {
    if (n_rows != c.n_rows || n_cols != c.n_cols) {
        throw std::out_of_range("");
    }
    Matrix tmp(n_rows, n_cols);
    for (int32_t i = 0; i < n_rows; ++i) {
        for (int32_t j = 0; j < n_cols; ++j) {
            tmp.mat[i][j] = mat[i][j] + c.mat[i][j];
        }
    }
    return tmp;
}


bool My_Matrix::Matrix::operator==(const Matrix &c) const {
    if (c.n_cols != n_cols || c.n_rows != n_rows) {
        return false;
    }
    for (int32_t i = 0; i < n_rows; ++i) {
        for (int32_t j = 0; j < n_cols; ++j) {
            if (mat[i][j] != c.mat[i][j]) {
                return false;
            }
        }
    }
    return true;
}


bool My_Matrix::Matrix::operator!=(const Matrix &c) const {
    if (c.n_cols != n_cols || c.n_rows != n_rows) {
        return true;
    }
    for (int32_t i = 0; i < n_rows; ++i) {
        for (int32_t j = 0; j < n_cols; ++j) {
            if (mat[i][j] != c.mat[i][j]) {
                return true;
            }
        }
    }
    return false;
}


My_Matrix::Matrix::ProxyRow& My_Matrix::Matrix::operator[](const int32_t i) const {
    if (i < 0 || i >= n_rows) {
        throw std::out_of_range("");
    }
    return mat[i];
}


std::ostream& My_Matrix::operator<<(std::ostream &os, const My_Matrix::Matrix &c) {
    for (int32_t i = 0; i < c.get_rows(); ++i) {
        for (int32_t j = 0; j < c.get_cols(); ++j) {
            os << c[i][j] << "\t";
        }
        os << std::endl;
    }
    return os;
}


My_Matrix::Matrix& My_Matrix::Matrix::operator=(const Matrix &c) {
    if (this == &c) {
        return *this;
    }
    if (mat != nullptr) {
        delete[] mat;
    }
    n_rows = c.n_rows;
    n_cols = c.n_cols;
    mat = new ProxyRow[n_rows];
    for (int32_t i = 0; i < n_rows; ++i) {
        mat[i] = c.mat[i];
    }
    return *this;
}