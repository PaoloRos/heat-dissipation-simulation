//===----------------------------------------------------------------------===//
//
// Progetto Heat Dissipation
// Corso di Tecniche di Programmazione Avanzata, Università di Trento
// Autore: Paolo Rossi
// Data: 06/06/2025
//
//===----------------------------------------------------------------------===//

/** @file matrix.hh
 *  @brief Matrix data structure and low-level operations for the heat diffusion solver.
 */

/** @brief Intensity of heat source 1. */
#define HEAT_SOURCE_1 5
/** @brief Intensity of heat source 2. */
#define HEAT_SOURCE_2 3
/** @brief Row/column position of heat source 1. */
#define HS_POS_1 5
/** @brief Row/column position of heat source 2. */
#define HS_POS_2 20

#include "std_libraries.hh"

/**
 * @class Matrix
 * @brief Square matrix stored in a contiguous 1D buffer.
 *
 * The class provides 2D and linear indexing operators and utility methods used
 * by both sequential and OpenMP solvers.
 */
class Matrix
{
    public:
    
        /**
         * @brief Builds an @p size x @p size matrix.
         * @param size Matrix side length.
         * @param zero When false, default heat sources are injected at fixed coordinates.
         */
        Matrix(short size, bool zero = false);

        /** @brief Copy constructor. */
        Matrix(const Matrix&);

        /** @brief Releases matrix-owned memory. */
        ~Matrix();

        /** @brief Returns matrix side length N. */
        int get_size() const;

        /**
         * @brief Prints the memory address of one matrix element.
         * @param r Row index or linear index when @p c < 0.
         * @param c Column index (optional).
         */
        void get_ID(const short r, const short c = -1);

        /**
         * @brief Copies all elements from another matrix inside a parallel region.
         * @param other Source matrix.
         * @param chunk_size Chunk size for OpenMP static scheduling.
         */
        void copy_in_parallel(const Matrix&, const int chunk_size);

        /** @brief Assignment with size consistency check. */
        Matrix& operator=(const Matrix&);

        /**
         * @brief Accesses one element by matrix coordinates.
         * @param r Row index.
         * @param c Column index.
         */
        double& operator()(const int, const int) const;

        /**
         * @brief Accesses one element by linear index.
         * @param idx Linear index in [0, N*N).
         */
        double& operator[](const int) const;

        //double operator-(const Matrix&) const; -> verifica che non serva

        /** @brief Stream output of matrix values row by row. */
        friend ostream& operator<<(ostream&, const Matrix&);

    private:
        /** @brief Contiguous matrix storage (row-major). */
        double* el;
        /** @brief Matrix side length. */
        short N;
};

/** @brief Stream output operator for Matrix. */
ostream& operator<<(ostream&, const Matrix&);

inline int Matrix::get_size() const { return this->N; }

inline double& Matrix::operator()(const int r, const int c) const { return this->el[r*this->N + c]; }

inline double& Matrix::operator[](const int idx) const { return this->el[idx]; }

