#define HEAT_SOURCE_1 5
#define HEAT_SOURCE_2 3
#define HS_POS_1 5
#define HS_POS_2 20

#include "std_libraries.hh"

// Devo fare questo upgrade:
// salvare i dati in un array allineato -> aggiornare classe
// provare senza cambiare il codice parallelo
// provare a inserire metodo di copia: #pragma nel metodo di modo tale che accedo al puntatore

// Matrix of dimension N, but counting: 0...N-1.
class Matrix
{
    public:
        //Matrix();
    
        //Initialize the matrix to a null matrix. In case 'null' == false -> elements in position (5,5) and (20,20) have default values.
        Matrix(short size, bool zero = false);

        Matrix(const Matrix&);

        ~Matrix();

        int get_size() const;

        void get_ID(const short r, const short c = -1);

        void copy_in_parallel(const Matrix&, const int chunk_size);

        Matrix& operator=(const Matrix&);

        // Access to the element by coordinates (row,col).
        double& operator()(const int, const int) const;

        // Access to the element by array position.
        double& operator[](const int) const;

        double operator-(const Matrix&) const;

        friend ostream& operator<<(ostream&, const Matrix&);

        private:
        double* el;
        short N;
};

ostream& operator<<(ostream&, const Matrix&);

inline int Matrix::get_size() const { return this->N; }

inline double& Matrix::operator()(const int r, const int c) const { return this->el[r*this->N + c]; }

inline double& Matrix::operator[](const int idx) const { return this->el[idx]; }

