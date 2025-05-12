#include <matrix.hh>

int main(const int argc, const char **argv)
{
    // ==== I/O strem opening ====

    fstream my_out;
    my_out.open("starting_mat.txt", ios::out);

    if(my_out.fail()) {
        cerr << "Error in opening file.\n";
        return -1;
    }

    int i, j;

    // ==== Matrix generation ====

    Matrix mat(stoi(argv[1]));

    



    my_out.close();

    return 0;
}

double** alloc_matrix(const int &LEN)
{
    double **mat = new double *[LEN];   //LEN rows
    for(int i = 0; i < LEN; ++i)
        mat[i] = new double[LEN];   //LEN columns
    return mat;
}

void dealloc_matrix(double **mat, const int &LEN)
{
    for(int i = 0; i < LEN; ++i)
        delete []mat[i];
    delete []mat;
}

void print_matrix(double **mat, const int &LEN, const fstream &file)
{
    for(int i = 0; i < LEN; ++i)
    {
        for(int j = 0; j < LEN; ++j)
    }
}