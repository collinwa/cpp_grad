#include <iostream>
#include <algorithm>
#include <queue>
#include <stdlib.h>
#include <math.h>
#include "Value.h"
class Matrix {
    public:
       friend Matrix& operator+(Matrix& lhs, Matrix& rhs);
       friend Matrix& operator*(Matrix& lhs, Matrix& rhs);        
       friend Matrix& operator-(Matrix& lhs, Matrix& rhs);
       static shared_ptr<Value> mse(Matrix& lhs, Matrix& rhs);

       shared_ptr<Value> get_element(int row, int col);
       void set_element(int row, int col, shared_ptr<Value> val);
       void relu();

       Matrix();
       Matrix(int dim1, int dim2, double epsilon_init);
       ~Matrix();
    private:
        int rows, cols;
        vector< vector< shared_ptr<Value> >  > mat;
};