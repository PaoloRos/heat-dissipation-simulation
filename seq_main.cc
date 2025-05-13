#include "matrix.hh"

int main(const int argc, const char **argv)
{
    // ==== I/O strem opening ====

    fstream my_start;
    my_start.open("starting_mat.txt", ios::out);

    if(my_start.fail()) {
        cerr << "Error in opening file.\n";
        return -1;
    }

    // setting to format %3.2f
    my_start << setw(3) << fixed << setprecision(2);

    int i, j;

    // ==== Matrix generation ====

    int n;

    if(argv[1] == nullptr || stoi(argv[1]) < 24)
    {
        cerr << "Warning: incorrect matrix size -> by default matrix set to 24x24.\n";
        n = 24;
    } else
        n = stoi(argv[1]);

    Matrix mat(n);

    my_start << mat;



    my_start.close();

    return 0;
}