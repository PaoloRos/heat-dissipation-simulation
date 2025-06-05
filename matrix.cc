#include "matrix.hh"

//Matrix::Matrix() : el(nullptr), N(0) {}

Matrix::Matrix(short size, bool zero)
{
    this->N = size;

    this->el = new double [this->N*this->N];
    
    if(!zero) {
        el[HS_POS_1*N + HS_POS_1] = HEAT_SOURCE_1;
        el[HS_POS_2*N + HS_POS_2] = HEAT_SOURCE_2;
    }
}

Matrix::Matrix(const Matrix& other)
{
    this->N = other.N;

    this->el = new double [this->N*this->N];
    
    for(int i = 0; i < this->N*this->N; ++i)
        this->el[i] = other.el[i];
}

Matrix::~Matrix()
{
    if(el != nullptr)
        delete []this->el;
}

void Matrix::get_ID(const short r, const short c)
{
    cout << ( (c < 0)? &(this->el[r]) : &(this->el[r*this->N + c]) ) << '\n';
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if(this->N != other.N) {
        cerr << "Error: size doesn't match.\n";
        exit(-1);
    }

    for(int i=0; i < this->N*this->N; ++i)
        this->el[i] = other.el[i];

    return *this;
}

double Matrix::operator-(const Matrix& other) const
{
    double res = 0;
    for(int i = 0; i < this->N*this->N; ++i)
        res += this->el[i] - other.el[i];
        
    return res;
}

ostream& operator<<(ostream& os, const Matrix& other)
{
    for(int i = 0; i < other.N; ++i)
        for(int j = 0; j < other.N; ++j)
            os << other.el[i*other.N + j] << ( (j < other.N - 1)? ' ' : '\n' );

    return os;
}