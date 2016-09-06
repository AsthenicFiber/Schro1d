#ifndef SCHRO_POIS_H
#define SCHRO_POIS_H

#include "matrix.h"

void solve_sp(Matrix dEc, Matrix eps, Matrix m, Matrix Q, Matrix E);

Matrix diff2_n(int n);

Matrix vec2diag(Matrix A);

#endif // SCHRO_POIS_H
