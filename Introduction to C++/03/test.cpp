#include <iostream>
#include <vector>
#include <functional>
#include <fstream>
#include <assert.h>
#include "my_matrix.hpp"


void initialize_matrix(const My_Matrix::Matrix &mat) {
    for (int32_t i = 0; i < mat.get_rows(); ++i) {
        for (int32_t j = 0; j < mat.get_cols(); ++j) {
            mat[i][j] = i * mat.get_cols() + j;
        }
    }
}


void test_1() {
    std::cout << "Test 1: Matrix initialization + reading elements + matrix ostream + Wrong matrix initialization" << std::endl;
    std::cout << "-----------" << std::endl;

    My_Matrix::Matrix mat(10, 10);
    initialize_matrix(mat);
    bool bad_mat_alloc = false;

    try {
        My_Matrix::Matrix mat1(-1, 0);
    } catch(std::bad_alloc &a) {
        bad_mat_alloc = true;
    }

    std::cout << mat;

    assert(mat[2][0] == 20);
    assert(mat[9][9] == 99);
    assert(mat[0][0] == 0);
    assert(mat[1][5] == 15);
    assert(mat[3][7] == 37);
    assert(bad_mat_alloc);

    std::cout << "-----------" << std::endl;
    std::cout << "Test 1: OK!" << std::endl << std::endl;
}


void test_2() {
    std::cout << "Test 2: std::out_of_range" << std::endl;

    My_Matrix::Matrix mat(5, 5);
    initialize_matrix(mat);
    int32_t catch_num = 0;

    try {
        mat[5][0] = 5;
    } catch (std::out_of_range &a) {
        catch_num++;
    }
    try {
        mat[-1][2] = 5;
    } catch (std::out_of_range &a) {
        catch_num++;
    }
    try {
        mat[3][5] = 5;
    } catch (std::out_of_range &a) {
        catch_num++;
    }
    try {
        mat[2][-1] = 5;
    } catch (std::out_of_range &a) {
        catch_num++;
    }
    try {
        mat[-1][6] = 5;
    } catch (std::out_of_range &a) {
        catch_num++;
    }

    assert(catch_num == 5);

    std::cout << "Test 2: OK!" << std::endl << std::endl;;    
}


void test_3() {
    std::cout << "Test 3: Operators: =, !=, ==" << std::endl;

    My_Matrix::Matrix mat(12, 13);
    initialize_matrix(mat);

    My_Matrix::Matrix mat1(3, 4);
    mat1 = mat;

    for (int32_t i = 0; i < mat.get_rows(); ++i) {
        for (int32_t j = 0; j < mat.get_cols(); ++j) {
            assert(mat1[i][j] == mat[i][j]);
        }
    }

    assert(mat1 == mat);

    mat1[10][10] = -5;

    assert(mat1 != mat);

    std::cout << "Test 3: OK!" << std::endl << std::endl;;    
}


void test_4() {
    std::cout << "Test 4: Operators ==, != on different size matrices" << std::endl;

    My_Matrix::Matrix mat1(9, 9), mat2(9, 8);

    assert(!(mat1 == mat2));
    assert(mat1 != mat2);
 
    std::cout << "Test 4: OK!" << std::endl << std::endl;    
}


void test_5() {
    std::cout << "Test 5: get_cols, get_rows methods" << std::endl;

    My_Matrix::Matrix mat(3, 4);

    assert(mat.get_cols() == 4 && mat.get_rows() == 3);

    std::cout << "Test 5: OK!" << std::endl << std::endl;    
}


void test_6() {
    std::cout << "Test 6: Correct operators +, *=" << std::endl;
    std::cout << "-----------" << std::endl;

    My_Matrix::Matrix mat1(5, 7), mat2(5, 7), mat3(5, 7);

    initialize_matrix(mat1);
    initialize_matrix(mat2);

    mat3 = mat1 + mat2;

    for (int32_t i = 0; i < mat1.get_rows(); ++i) {
        for (int32_t j = 0; j < mat1.get_cols(); ++j) {
            assert(mat3[i][j] == mat1[i][j] + mat2[i][j]);
        }
    }

    std::cout << mat1 + mat2 << std::endl;

    assert(mat1 == mat2);

    mat1 *= 7;

    for (int32_t i = 0; i < mat1.get_rows(); ++i) {
        for (int32_t j = 0; j < mat1.get_cols(); ++j) {
            assert(mat1[i][j] == 7 * mat2[i][j]);
        }
    }
    
    std::cout << "-----------" << std::endl;
    std::cout << "Test 6: OK!" << std::endl << std::endl;  
}


void test_7() {
    std::cout << "Test 7: incorrect operator +" << std::endl;

    My_Matrix::Matrix mat1(5, 6), mat2(7, 8);
    initialize_matrix(mat1);
    initialize_matrix(mat2);
    bool caught = false;

    try {
        mat1 + mat2;
    } catch(std::out_of_range &a) {
        caught = true;
    }

    assert(caught);

    std::cout << "Test 7: OK!" << std::endl << std::endl;
}


int main(void) {
    std::vector<std::function<void()>> tests{test_1, test_2, test_3, test_4, test_5, test_6, test_7};   
    for (size_t i = 0; i < tests.size(); ++i) {
        tests[i]();
    }
    return 0;
}