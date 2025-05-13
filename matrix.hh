#include "std_libraries.hh"

// Matrix of dimension N, but counting: 0...N-1.
class Matrix
{
    public:
        //Initialize the matrix to a null matrix excpet for elements in position (5,5) and (20,20).
        Matrix(int);

        // forse non serve
        Matrix(const Matrix&);

        ~Matrix();

        inline int get_size() const;

        Matrix& operator=(const Matrix&);

        inline double& operator()(const int, const int) const;

        friend ostream& operator<<(ostream&, const Matrix&);

    private:
        double **el;
        int N;
};

ostream& operator<<(ostream&, const Matrix&);

int Matrix::get_size() const { return this->N; }

double& Matrix::operator()(const int r, const int c) const { return this->el[r][c]; }
