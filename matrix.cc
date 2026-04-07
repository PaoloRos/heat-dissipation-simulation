//===----------------------------------------------------------------------===//
//
// Progetto Heat Dissipation
// Corso di Tecniche di Programmazione Avanzata, Università di Trento
// Autore: Paolo Rossi
// Data: 06/06/2025
//
//===----------------------------------------------------------------------===//
/** @file matrix.cc
 *  @brief Implementation of Matrix methods used by sequential and OpenMP solvers.
 */

#include "matrix.hh"

//Matrix::Matrix() : el(nullptr), N(0) {}

/**
 * @brief Creates a matrix and optionally places two fixed heat sources.
 */
Matrix::Matrix(short size, bool zero)
{
    this->N = size;

    this->el = new double [this->N*this->N];
    
    if(!zero) {
        el[HS_POS_1*N + HS_POS_1] = HEAT_SOURCE_1;
        el[HS_POS_2*N + HS_POS_2] = HEAT_SOURCE_2;
    }
}

/** @brief Deep-copy constructor implemented with OpenMP SIMD copy loop. */
Matrix::Matrix(const Matrix& other)
{
    this->N = other.N;

    this->el = new double [this->N * this->N];

    #pragma omp parallel for simd schedule(static)
    for(int i = 0; i < this->N*this->N; ++i)
        this->el[i] = other.el[i];
}

/** @brief Frees dynamically allocated matrix data. */
Matrix::~Matrix()
{
    if(el != nullptr)
        delete []this->el;
}

/**
 * @brief Prints the address of one matrix element for debug purposes.
 */
void Matrix::get_ID(const short r, const short c)
{
    cout << ( (c < 0)? &(this->el[r]) : &(this->el[r*this->N + c]) ) << '\n';
}

/**
 * @brief Parallel copy helper meant to be called inside an OpenMP region.
 */
void Matrix::copy_in_parallel(const Matrix& other, const int chunk_size)
{
    if(this->N != other.N) {
        cerr << "Error: size doesn't match.\n";
        exit(-1);
    }

    #pragma omp for simd schedule(static, chunk_size)
    for(int i = 0; i < this->N * this->N; ++i)
        this->el[i] = other.el[i];
}

/**
 * @brief Assignment operator with dimension check.
 */
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

//double Matrix::operator-(const Matrix& other) const
//{
//    double res = 0;
//    for(int i = 0; i < this->N*this->N; ++i)
//        res += this->el[i] - other.el[i];
//        
//    return res;
//}

/**
 * @brief Writes matrix values to output stream in plain text table form.
 */
ostream& operator<<(ostream& os, const Matrix& other)
{
    for(int i = 0; i < other.N; ++i)
        for(int j = 0; j < other.N; ++j)
            os << other.el[i*other.N + j] << ( (j < other.N - 1)? ' ' : '\n' );

    return os;
}
