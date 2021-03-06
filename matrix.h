#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

//typedef float mel;
typedef double mel;

class Matrix
{
public:
    Matrix(int rows, int cols);
    Matrix();
    ~Matrix();
    Matrix(const Matrix &B);
    Matrix(Matrix&& B);
    Matrix& operator = (const Matrix &B);
    Matrix& operator = (Matrix&& B);

    mel& get(int row, int col);
    mel get(int row, int col) const;
    mel* operator [](const int &row);
    mel& operator ()(const int &row, const int &col);
    mel operator ()(const int &row, const int &col) const;

    Matrix operator + (const Matrix &B);
    Matrix operator += (const Matrix &B);
    Matrix operator * (const mel &s);
    mel operator *= (const mel &s);
    Matrix operator * (const Matrix &B);
    Matrix operator ++ (int);

    void print_matrix();
    int length();
    int rows();
    int cols();
    mel * mat();

    void activation();

private:
    int m,n;
    mel *p;
    //std::vector<mel> p;
};

#endif // MATRIX_H
