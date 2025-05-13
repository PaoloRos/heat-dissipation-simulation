#include "matrix.hh"

Matrix::Matrix(const int n)
{
    this->N = n;

    this->mat = new double *[N];
    for(int i = 0; i < N; ++i)
        mat[i] = new double[N];

    this->mat[4][4] = 5;
    this->mat[19][19] = 3;

    //cerr << "Matrix constructed.\n";
}

Matrix::~Matrix()
{
    for(int i = 0; i < N; ++i)
        delete []mat[i];
    delete []mat;

    //cerr << "Matrix destroyed.\n";
}

ostream &operator<<(ostream &os, const Matrix &other)
{
    for(int i = 0; i < other.N; ++i)
        for(int j = 0; j < other.N; ++j)
            os << other.mat[i][j] << ( (j < other.N -1)? ' ' : '\n' );

    return os;
}
