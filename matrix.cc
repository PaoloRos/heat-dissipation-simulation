#include "matrix.hh"

//Matrix::Matrix() : el(nullptr), N(0) {}

Matrix::Matrix(int size, bool null)
{
    this->N = size;

    this->el = new double *[N];
    for(int i = 0; i < N; ++i)
        el[i] = new double[N];

    if(!null) {
        this->el[HS_POS_1][HS_POS_1] = HEAT_SOURCE_1;
        this->el[HS_POS_2][HS_POS_2] = HEAT_SOURCE_2;
    }

    //cerr << "Matrix constructed.\n";
}

Matrix::Matrix(const Matrix& other)
{
    this->N = other.N;

    this->el = new double *[N];
    int i, j;
    for(i = 0; i < N; ++i) 
    {
        el[i] = new double[N];
        for(j = 0; j < N; ++j)
            el[i][j] = other.el[i][j];
    }
}

Matrix::~Matrix()
{
    if(el != nullptr){
        for(int i = 0; i < N; ++i)
            delete []el[i];
        delete []el;
    }

    //cerr << "Matrix destroyed.\n";
}

void Matrix::copy_subMatrix(const Matrix& other, const short& y0, const short& blc_y, const short& x0, const short& blc_x)
{
    //starting (row, col) position in 'other': (y0 - 1*blc_y, x0 - 1*blc_x)
    //end position (y0 - 1*blc_y + this->N, x0 - 1*blc_x + this->N)

    for(int r = 0; r < this->N; ++r)
        for(int c = 0; c < this->N; ++c)
            this->el[r][c] = other.el[y0 - 1*blc_y + r][x0 - 1*blc_x + c];
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if(this->N != other.N) {
        cerr << "Error: size doesn't match.\n";
        exit(-1);
    }
    int i, j;
    for(i = 0; i < this->N; ++i)
        for(j = 0; j < this->N; ++j)
            this->el[i][j] = other.el[i][j];
    
    return *this;
}

ostream& operator<<(ostream& os, const Matrix& other)
{
    for(int i = 0; i < other.N; ++i)
        for(int j = 0; j < other.N; ++j)
            os << other.el[i][j] << ( (j < other.N -1)? ' ' : '\n' );

    return os;
}

double Matrix::operator-(const Matrix& other) const
{
    double res = 0;
    for(int i = 0; i < this->N; ++i)
        for(int j = 0; j < this->N; ++j)
            res += this->el[i][j] - other.el[i][j];
    return res;
}
