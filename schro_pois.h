#ifndef SCHRO_POIS_H
#define SCHRO_POIS_H

#include "matrix.h"

#define DX 3.3e-19 // dx in c*s
#define PI 3.14159 // dx in c*s

//void solve_sp(Matrix dEc, Matrix eps, Matrix m, Matrix Q, Matrix E);

void poiss_solve(Matrix Q, Matrix eps, Matrix *V);
void schro_solve(Matrix U, Matrix m, Matrix* psi, Matrix* E);

Matrix diff2_n(int n);

Matrix vec2diag(Matrix A);

double max(Matrix A);
double max(Matrix A, int start, int end);
double min(Matrix A);
double min(Matrix A, int start, int end);

double max_bound(Matrix A);

Matrix transpose(Matrix A);

#endif // SCHRO_POIS_H
