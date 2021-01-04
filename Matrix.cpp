#include <iostream>
#include <algorithm>
#include <queue>
#include <stdlib.h>
#include <math.h>
#include "Value.h"
#include "Matrix.h"

// default Constructor
Matrix::Matrix() {
    rows = 0; cols = 0;
}

Matrix::Matrix(int dim1, int dim2, double epsilon_init) {
    rows = dim1; col2 = dim2;
    for (int i = 0; i < dim1; i++) {
        std::vector<shared_ptr<Value>> temp;
        for (int j = 0; j < dim2; j++) {
            temp.push_back(make_value(rand() % 1 * 2*epsilon_init - epsilon_init));
        }
        m.push_back(temp);
    }
}

//Everything should take care of itself, if not will fix later.
Matrix::~Matrix() {

}

Matrix& operator+(Matrix& lhs, Matrix& rhs) {
    Matrix m;
    m* = new Matrix(lhs.rows, lhs.cols, 0);
    for (int i = 0; i < lhs.rows, i++) {
        for (int j = 0; j < lhs.cols, j++) {
            m -> set_element(i,j,get_self(*(lhs.mat[i][j]) + *(rhs.mat[i][j])));
        }
    }
    return *m;
}

Matrix& operator*(Matrix& lhs, Matrix&rhs) {
    Matrix m;
    m* = new Matrix(lhs.rows, rhs.cols, 0);
    for (int i = 0; i < lhs.rows; i++) {
        for (int j = 0; j < rhs.cols; j++) {
            for (int k = 0; k < lhs.cols; k++) {
                m -> mat[i][j] = get_self(*(m -> mat[i][j]) + *(lhs.mat[i][k]) * *(rhs.mat[k][j]));
            }
        }
    }
    return *m;
}

Matrix& operator-(Matrix& lhs, Matrix& rhs) {
    Matrix m;
    m* = new Matrix(lhs.rows, lhs.cols, 0);
    for (int i = 0; i < lhs.rows, i++) {
        for (int j = 0; j < lhs.cols, j++) {
            m -> set_element(i,j,get_self(*(lhs.mat[i][j]) - *(rhs.mat[i][j])));
        }
    }
    return *m;
}

static shared_ptr<Value> Matrix::mse(Matrix& lhs, Matrix& rhs) {
    shared_ptr<Value> Value mean_se = make_value(0);
    for (int i = 0; i < lhs.rows; i++) {
        for (int j = 0; j < lhs.cols; j++) {
            *mean_se = *mean_se + (*(lhs.mat[i][j])-*(rhs.mat[i][j])) * (*(lhs.mat[i][j])-*(rhs.mat[i][j]));
        }
    }
    return mean_se;
}

shared_ptr<Value> Matrix::get_element(int row, int col) {
    return mat[row][col];
}

void Matrix::set_element(int row, int col, shared_ptr<Value> val) {
    mat[row][col] = val;
}

void Matrix::relu() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat[i][j] = get_self(relu(*(mat[i][j])));
        }
    }
}