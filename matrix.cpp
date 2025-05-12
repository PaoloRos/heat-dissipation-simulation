#include <matrix.hh>

Matrix::Matrix(const int n)
{
    if(n == 20)
        cerr << "Warning: size lower than 20x20 -> by default matrix set to 20x20.\n";

    this->N = n;

    this->mat = new double *[N];
    for(int i = 0; i < N; ++i)
        mat[i] = new double[N];

    this->mat[5][5] = 5;
    this->mat[20][20];

    cout << "Matrix constructed\n";
}

Matrix::~Matrix()
{
    for(int i = 0; i < N; ++i)
        delete []mat[i];
    delete []mat;
}

ostream &operator<<(ostream &os, const Matrix &other)
{
    for(int i = 0; i < other.N; ++i)
    {
        for(int j = 0; j < other.N; ++j)
            os << other.mat[i][j] << " ";
        os << "\n";
    }
}
