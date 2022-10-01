#pragma once

#include <iostream>
#include <ios>

namespace My_Matrix {
    class Matrix {
        protected:
        struct ProxyRow {
            int32_t *column = nullptr;
            int32_t n_cols;

            ProxyRow(int32_t n_cols);
            ProxyRow(){};
            ProxyRow(const ProxyRow &c);
            ~ProxyRow();
            
            ProxyRow& operator=(const ProxyRow &c);
            int32_t& operator[](int32_t j) const;
        };


        private:
        int32_t n_rows;
        int32_t n_cols;
        ProxyRow *mat = nullptr;


        public:
        Matrix(int32_t n_rows, int32_t n_cols);
        Matrix(const Matrix &c);
        ~Matrix();

        int32_t get_rows() const;
        int32_t get_cols() const;

        ProxyRow& operator[](const int32_t i) const;
        Matrix& operator=(const Matrix &c);
        Matrix operator*=(const int32_t a);
        Matrix operator+(const Matrix &c) const;
        bool operator==(const Matrix &c) const;
        bool operator!=(const Matrix &c) const;
        friend std::ostream& operator<<(std::ostream &os, const Matrix &c); 
    };
    
    std::ostream& operator<<(std::ostream &os, const Matrix &c); 
}