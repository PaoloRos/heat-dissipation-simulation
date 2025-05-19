#define HEAT_SOURCE_1 5
#define HEAT_SOURCE_2 3
#define HS_POS_1 5
#define HS_POS_2 20

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

        int get_size() const;

        Matrix& operator=(const Matrix&);

        double& operator()(const int, const int) const;

        friend ostream& operator<<(ostream&, const Matrix&);

    private:
        double **el;
        int N;
};

ostream& operator<<(ostream&, const Matrix&);

inline int Matrix::get_size() const { return this->N; }

inline double& Matrix::operator()(const int r, const int c) const { return this->el[r][c]; }
