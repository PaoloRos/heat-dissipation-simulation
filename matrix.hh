#define HEAT_SOURCE_1 5
#define HEAT_SOURCE_2 3
#define HS_POS_1 5
#define HS_POS_2 20

#include "std_libraries.hh"

// Matrix of dimension N, but counting: 0...N-1.
class Matrix
{
    public:
        //Matrix();
    
        //Initialize the matrix to a null matrix. In case 'null' == false -> elements in position (5,5) and (20,20) have default values.
        Matrix(short size, bool null = false);

        Matrix(const Matrix&);

        ~Matrix();

        int get_size() const;

        void copy_subMatrix(const Matrix& other, const short& y0, const short& x0);
        
        void get_ID(const short& r, const short& c);

        Matrix& operator=(const Matrix&);

        double& operator()(const int, const int) const;

        double& operator[](const int) const;

        friend ostream& operator<<(ostream&, const Matrix&);

        double operator-(const Matrix&) const;


    private:
        double **el;
        short N;
};

ostream& operator<<(ostream&, const Matrix&);

inline int Matrix::get_size() const { return this->N; }

inline double& Matrix::operator()(const int r, const int c) const { return this->el[r][c]; }

inline double& Matrix::operator[](const int idx) const { return this->el[idx/N][idx%N]; }
