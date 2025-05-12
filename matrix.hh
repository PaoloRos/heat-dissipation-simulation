#include <std_libraries.hh>

class Matrix
{
    public:
        //Initialize the matrix (at least 20x20) to a null matrix excpet for elements in position (5,5) and (20,20).
        Matrix(const int n = 20);

        ~Matrix();

        friend ostream &operator<<(ostream &os, const Matrix &other);

    private:
        double **mat;
        int N;
}