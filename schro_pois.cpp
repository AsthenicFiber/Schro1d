
#include "schro_pois.h"
#include "lapacke.h"

// Functions for Schrodinger Poisson solver

/*
void solve_sp(Matrix dEc, Matrix eps, Matrix m, Matrix Q, Matrix E)
{
    Matrix D = diff2_n(m.rows());
    dEc = vec2diag(dEc);
    eps = vec2diag(eps);
    m = vec2diag(m);
}
*/

void poiss_solve(Matrix Q, Matrix eps, Matrix* V)
{
    Matrix A = diff2_n(Q.rows());
    Matrix B(eps.rows(),1);
    for (int x = 0; x < eps.rows(); x++)
    {
        B[x][0] = Q[x][0]/eps[x][0];
    }

    int n = A.rows();
    int nrhs = B.cols();
    int lda = A.cols();
    int ldb = B.cols();
    int* ipiv = new int[A.rows()];

    LAPACKE_dgesv(LAPACK_ROW_MAJOR,n,nrhs,A.mat(),lda,ipiv,B.mat(),ldb);

    *V = B;
    delete [] ipiv;
    return;
}

void schro_solve(Matrix U, Matrix m, Matrix* psi, Matrix* E)
{
    Matrix m_inv(m.rows(),m.rows());
    double hbar = 6.58212e-16;

    for (int x = 0; x < m.rows(); x++)
    {
        m_inv[x][x] = 1/m[x][0];
    }

    Matrix A = m_inv*diff2_n(U.rows())*(-hbar*hbar/2) + vec2diag(U);
    A = (A + transpose(A))*.5;

    Matrix B(A.rows(),1);

    int n = A.rows();
    int lda = A.cols();

    int info = LAPACKE_dsyev(LAPACK_ROW_MAJOR,'V','U',n,A.mat(),lda,B.mat());

    *psi = A;
    *E = B;
    return;
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

double max(Matrix A)
{
    return max(A, 0, A.rows());
}

double max(Matrix A, int start, int end)
{
    double max = A[start][0];
    for (int i = start; i < end; i++)
    {
        if (A[i][0] > max)
        {
            max = A[i][0];
        }
    }
    return max;
}

double min(Matrix A)
{
    return min(A, 0, A.rows());
}

double min(Matrix A, int start, int end)
{
    double min = A[start][0];
    for (int i = start; i < end; i++)
    {
        if (A[i][0] > min)
        {
            min = A[i][0];
        }
    }
    return min;
}

double max_bound(Matrix A)
{
    double min_A = A[1][0];
    int min_in = 1;
    for (int i = 0; i < A.rows(); i++)
    {
        if (A[i][0] < min_A)
        {
            min_A = A[i][0];
            min_in = i;
        }
    }
    double E_max1 = max(A,0,min_in);
    double E_max2 = max(A,min_in,A.rows());
    if (E_max2 > E_max1)
    {
        return E_max1;
    }
    else
    {
        return E_max2;
    }
}

Matrix transpose(Matrix A)
{
    Matrix B(A.cols(), A.rows());
    for (int i = 0; i < A.cols(); i++)
    {
        for (int j = 0; j < A.rows(); j++)
        {
            B[i][j] = A[j][i];
        }
    }
    return B;
}
