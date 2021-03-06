#ifndef SCHRO_POIS_H
#define SCHRO_POIS_H

#include "matrix.h"

#define DX 3.3356e-19 // dx in c*s, c*s/Angstrom
#define PI 3.14159 // pi
#define EPSo 1.656758e16 // vacuum permittivity in e/c*s*V
#define Qe 1.602e-19 // electron charge
#define hP 4.13567e-15 // eV*s
#define kB 8.61733e-5 // eV/K
#define Me 5.11e5 // electron mass in eV/c^2

//void solve_sp(Matrix dEc, Matrix eps, Matrix m, Matrix Q, Matrix E);

//void poiss_solve(Matrix Q, Matrix eps, Matrix *V);
void poiss_solve(Matrix Q, Matrix eps, Matrix *V, double top_bc, double bot_bc);
void schro_solve(Matrix U, Matrix m, Matrix* psi, Matrix* E);

Matrix diff2_n(int n);

Matrix vec2diag(Matrix A);

double sum(Matrix A);
double sum(Matrix A, int start, int end);
double max(Matrix A);
double max(Matrix A, int start, int end);
double min(Matrix A);
double min(Matrix A, int start, int end);

double max_bound(Matrix A);

Matrix transpose(Matrix A);

Matrix q_psi(Matrix U, Matrix Ef, Matrix m, Matrix E, Matrix psi, double T);
Matrix q_boltz(Matrix U, Matrix Ef, Matrix m, double T);
double fermi_integral(double E);

double fermi_level(double me, double mh, double Eg, double T, double dop_conc);

#endif // SCHRO_POIS_H
