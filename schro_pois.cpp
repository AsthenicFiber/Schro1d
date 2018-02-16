
#include <math.h>
#include "schro_pois.h"
#include "lapacke.h"

// Functions for Schrodinger Poisson solver

void poiss_solve(Matrix Q, Matrix eps, Matrix* V, double top_bc, double bot_bc)
{
    Matrix A = diff2_n(Q.rows());
    double coef = 1/(DX*DX); // dx in c*s
    int end = eps.rows() - 1;
    A = A*coef;
    Q *= 1e-8*1e-8*1e-8/(DX*DX*DX); // e/(c*s)^3
    Matrix B(end+1,1);
    for (int x = 0; x < end+1; x++)
    {
        B[x][0] = Q[x][0]/eps[x][0];
    }

    // set up boundary conditions
    // top contact V = 0
    A[0][0] = 1;
    A[0][1] = 0;
    B[0][0] = top_bc;
    // bottom contact dV/dx = 0
    //A[end][end-1] = -1;
    //A[end][end] = 1;
    //B[end][0] = 0;
    // bottom contact V = 0
    A[end][end-1] = 0;
    A[end][end] = 1;
    B[end][0] = bot_bc;

    int n = A.rows();
    int nrhs = B.cols();
    int lda = A.cols();
    int ldb = B.cols();
    int* ipiv = new int[A.rows()];

    int info = LAPACKE_dgesv(LAPACK_ROW_MAJOR,n,nrhs,A.mat(),lda,ipiv,B.mat(),ldb);
    info += 0;

    *V = B;
    delete [] ipiv;
    return;
}
/*
void poiss_solve(Matrix Q, Matrix eps, Matrix *V)
{
    poiss_solve(Q,eps,&V,0,0);
}
*/

void schro_solve(Matrix U, Matrix m, Matrix* psi, Matrix* E)
{
    Matrix m_inv(m.rows(),m.rows());
    double hbar = 6.58212e-16; // eV*s
    //m = m*5.11e5; // eV/c^2 electron masses

    for (int x = 0; x < m.rows(); x++)
    {
        m_inv[x][x] = 1/m[x][0];
    }

    double coef = hbar*hbar/(2*DX*DX); // hbar^2/(2*dx^2), dx in c*s

    Matrix A = m_inv*diff2_n(U.rows())*coef + vec2diag(U);
    A = (A + transpose(A))*.5;

    Matrix B(A.rows(),1);

    int n = A.rows();
    int lda = A.cols();

    int info = LAPACKE_dsyev(LAPACK_ROW_MAJOR,'V','U',n,A.mat(),lda,B.mat());
    info += 0;

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

double sum(Matrix A)
{
    return sum(A, 0, A.rows());
}

double sum(Matrix A, int start, int end)
{
    double s = 0;
    for (int i = start; i < end; i++)
    {
        s += A[i][0];
    }
    return s;
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
        if (A[i][0] < min)
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

Matrix q_psi(Matrix U, Matrix Ef, Matrix m, Matrix E, Matrix psi, double T)
{
    int length = Ef.rows();
    double kT = kB*T; //eV
    Matrix n(length,1);

    // Find maximum bounded energy
    double E_max = max_bound(U);
    //E_max = max(U);

    for (int i = 0; i < length; i++)
    {
        if (E[i][0] >= E_max)
        {
            break;
        }

        // Find energy level occupancy and concentration over x
        Matrix n_E(length,1);
        for (int j = 0; j < length; j++)
        {
            //double F = 1/(1 + exp((E[i][0] - Ef[j][0])/kT));
            double F = kT*(m[j][0]*4*PI/(hP*hP))*log(1 + exp(-(E[i][0] - Ef[j][0])/kT)); // e/cs^2
            F *= 1e8*1e8*DX*DX; // e/cs^2
            n_E[j][0] = F*psi[j][i]*psi[j][i]*1e8; // e/(cm^3)
        }

        // Add to total carriers
        n = n + n_E;
    }
    return n;
}

Matrix q_boltz(Matrix U, Matrix Ef, Matrix m, double T)
{
    int length = Ef.rows();
    double kT = kB*T; //eV
    double h = hP; //eV*s
    Matrix n = Matrix(length,1);
    for (int i = 0; i < length; i++)
    {
        double Nc = 2*pow(double(2*PI*m[i][0]*kT/(h*h)),double(1.5));
        Nc *= DX*DX*DX*1e24; // convert to 1/cm^3

        if ((U[i][0] - Ef[i][0])/kT > 1) // non-degenerate
        {
            n[i][0] = Nc*exp(-(U[i][0] - Ef[i][0])/kT);
        }
        else // degenerate
        {
            n[i][0] = Nc*2*fermi_integral(-(U[i][0]-Ef[i][0])/kT)/sqrt(PI);
        }
    }
    return n;
}

double fermi_integral(double E)
{
    double d_eta = 0.01;
    double eta = d_eta;
    double F = sqrt(eta)/(1+exp(eta-E));
    double err = F;
    while (err/F > 1e-5)
    {
        F += d_eta*sqrt(eta)/(1+exp(eta-E));
        eta += d_eta;
        err = d_eta*sqrt(eta)/(1+exp(eta-E));
    }
    return F;
}

double fermi_level(double me, double mh, double Eg, double T, double dop_conc)
{
    double kT = kB*T; // eV
    double h = hP; //eV*s
    double Nc = 2*pow(double(2*PI*me*kT/(h*h)),double(1.5));
    Nc *= DX*DX*DX*1e24; // convert to 1/cm^3
    double Nv = 2*pow(double(2*PI*mh*kT/(h*h)),double(1.5));
    Nv *= DX*DX*DX*1e24; // convert to 1/cm^3
    double ni = sqrt(Nc*Nv*exp(-Eg/kT));
    double Ef = 0;
    if (dop_conc > 0)
    {
        Ef += kT*log(dop_conc/ni);
    }
    else if (dop_conc < 0)
    {
        Ef -= kT*log(-dop_conc/ni);
    }
    else
    {
        Ef += 3*kT/4*log(mh/me);
    }
    return Ef;
}
