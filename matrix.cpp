#include "matrix.h"
#include <iostream>
#include <iterator>
#include <cmath>


// checked_array_iterator is defiend by MSVC 2005 and later
// https://msdn.microsoft.com/en-us/library/aa985928(v=vs.80).aspx
//#if defined(_MSC_VER) || _MSC_VER < 1400
// MSVC 2005 has _MSC_VER = 1400
// https://en.wikipedia.org/wiki/Visual_C%2B%2B#Common_MSVC_version

// http://stackoverflow.com/questions/25716841/checked-array-iteratort-in-c11#comment40464386_25716929

/*
namespace stdext
{
    template<typename T> inline T checked_array_iterator(T arr, size_t)
    {
        return arr;
    }
}
*/

//#endif


// Simple constructor
Matrix::Matrix(int rows, int cols) : m(rows), n(cols), p(new mel[rows*cols])
{
    if (m < 0 || n < 0)
    {
        std::cout << "\nMatrix dimensions must be positive.\n";
        m = 1;
        n = 1;
        throw std::exception();
    }
    if (m > 10000 || n > 10000)
    {
        std::cout << "\nMatrix dimensions exceed 1000.\n";
        m = 1;
        n = 1;
        throw std::exception();
    }
    for (int i = 0; i < m*n; i++)
        p[i] = 0;
}

Matrix::Matrix() : m(1), n(1), p(new mel[1])
{
    if (m < 0 || n < 0)
    {
        std::cout << "\nMatrix dimensions must be positive.\n";
        m = 1;
        n = 1;
        throw std::exception();
    }
    if (m > 10000 || n > 10000)
    {
        std::cout << "\nMatrix dimensions exceed 1000.\n";
        m = 1;
        n = 1;
        throw std::exception();
    }
    p[0] = 0;
}

// Destructor
Matrix::~Matrix()
{
    if (p != nullptr)
    {
        delete [] p;
        p = nullptr;
    }
    //std::cout << "shutting down\n";
}

// Copy constructor
Matrix::Matrix(const Matrix &B) : m(B.m), n(B.n), p(new mel[B.m*B.n])
{
    #if defined(_MSC_VER) && _MSC_VER >= 1400
    std::copy(B.p, B.p + B.m*B.n, stdext::checked_array_iterator<mel*>(p, B.m*B.n));
    #else
    std::copy(B.p, B.p + B.m*B.n, p);
    #endif
}

// Move constructor
Matrix::Matrix(Matrix&& B) : m(1), n(1), p(nullptr)
{
    m = B.m;
    n = B.n;
    p = B.p;

    B.m = 1;
    B.n = 1;
    B.p = nullptr;
}

// Copy assignment operator
Matrix& Matrix::operator = (const Matrix &B)
{
    if (this != &B)
    {
        delete [] p;

        p = new mel[B.m*B.n];

        m = B.m;
        n = B.n;

        #if defined(_MSC_VER) && _MSC_VER >= 1400
        std::copy(B.p, B.p + B.m*B.n, stdext::checked_array_iterator<mel*>(p, B.m*B.n));
        #else
        std::copy(B.p, B.p + B.m*B.n, p);
        #endif
        //std::copy(B.p, B.p + B.m*B.n, p);
        //std::copy(B.p, B.p + B.m*B.n, stdext::checked_array_iterator<mel*>(p, B.m*B.n));
    }
    return *this;
}

// Move assignment operator
Matrix& Matrix::operator = (Matrix&& B)
{
    if (this != &B)
    {
        delete [] p;

        m = B.m;
        n = B.n;
        p = B.p;

        B.m = 1;
        B.n = 1;
        B.p = nullptr;
    }
    return *this;
}

mel& Matrix::get(int row, int col)
{
    return p[row*n+col];
}

mel Matrix::get(int row, int col) const
{
    return p[row*n+col];
}

mel* Matrix::operator [] (const int &row)
{
    return row * n + p;
    //return &p[row*n];
}

mel& Matrix::operator ()(const int &row, const int &col)
{
    return get(row,col);
}

mel Matrix::operator ()(const int &row, const int &col) const
{
    return get(row,col);
}

Matrix Matrix::operator + (const Matrix &B)
{
    if (m != B.m || n != B.n)
    {
        std::cout << "Matrices dimensions do not match for + operation." << std::endl;
        throw std::exception();
    }

    Matrix C(m,n);

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C.get(i,j) = get(i,j) + B.get(i,j);
            //C[i][j] = get(i,j) + B[i][j];
            //C(i,j) = get(i,j) + B(i,j);
            //C.set(i,j,get(i,j) + B.get(i,j));
        }
    }
    return C;
}

Matrix Matrix::operator += (const Matrix &B)
{
    if (m != B.m || n != B.n)
    {
        std::cout << "Matrices dimensions do not match for + operation." << std::endl;
        throw std::exception();
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            get(i,j) = get(i,j) + B.get(i,j);
            //set(i,j,get(i,j) + B.get(i,j));
        }
    }
    return B;
}

Matrix Matrix::operator * (const mel &s)
{
    Matrix C(m,n);
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C.get(i,j) = get(i,j)*s;
            //C.set(i,j,get(i,j)*s);
        }
    }
    return C;
}

mel Matrix::operator *=(const mel &s)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            get(i,j) = get(i,j)*s;
            //set(i,j,get(i,j)*s);
        }
    }
    return s;
}

Matrix Matrix::operator * (const Matrix &B)
{
    if (n != B.m)
    {
        std::cout << "Matrices dimensions do not match for * operation." << std::endl;
        throw std::exception();
    }

    Matrix C(m,B.n);
    for (int i = 0; i < C.m; i++)
    {
        for (int j = 0; j < C.n; j++)
        {
            mel sum = 0;
            for (int k = 0; k < n; k++)
            {
                sum += get(i,k) * B.get(k,j);
            }
            C.get(i,j) = sum;
            //C.set(i,j,sum);
        }
    }
    return C;
}

// Transpose operator
Matrix Matrix::operator ++ (int)
{
    Matrix C(n,m);

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C.get(j,i) = get(i,j);
            //C.set(j,i,get(i,j));
        }
    }
    return C;
}

void Matrix::print_matrix()
{
    std::cout << "\n" << m << " x " << n << std::endl;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cout << "\t" << get(i,j);
        }
        std::cout << std::endl;
    }
    return;
}

int Matrix::length()
{
    return m*n;
}

int Matrix::rows()
{
    return m;
}

int Matrix::cols()
{
    return n;
}

mel* Matrix::mat()
{
    return p;
}

void Matrix::activation()
{
    for (int i = 0; i < m*n; i++)
    {
        p[i] = 1/(1 + exp(p[i]));
    }
}
