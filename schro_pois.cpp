
#include "schro_pois.h"
#include "lapacke.h"

// Functions for Schrodinger Poisson solver


void solve_sp(Matrix dEc, Matrix eps, Matrix m, Matrix Q, Matrix E)
{
    Matrix D = diff2_n(m.rows());
    dEc = vec2diag(dEc);
    eps = vec2diag(eps);
    m = vec2diag(m);
}

Matrix diff2_n(int n)
{
    Matrix d2_(n,n);
    for (int i = 0; i < n; i++)
    {
        d2_[i][i] = 2;
        if (i != 0)
        {
            d2_[i][i-1] = -1;
            d2_[i-1][i] = -1;
        }
    }
    return d2_;
}

Matrix vec2diag(Matrix A)
{
    Matrix B;
    if (A.cols() == 1)
    {
        int n = A.rows();
        B = Matrix(n,n);
        for (int i = 0; i < n; i++)
        {
            B[i][i] = A[i][0];
        }
    }
    else
    {
        //Matrix B(1,1);
    }
    return B;
}
