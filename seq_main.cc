#include "matrix.hh"

int main(const int argc, const char **argv)
{
    // ==== I/O strem opening ====

    fstream my_start, my_out;
    my_start.open("starting_mat.txt", ios::out);
    my_out.open("output.txt", ios::out);

    if(my_start.fail() || my_out.fail()) {
        cerr << "Error in opening file.\n";
        return -1;
    }

    // setting to format %3.2f
    my_start << setw(3) << fixed << setprecision(2);
    my_out << setw(3) << fixed << setprecision(2);

    int i, j, m;   // counters

    // ==== Matrix generation ====

    const int N = (argv[1] == nullptr || stoi(argv[1]) < 20)? 20 : stoi(argv[1]);

    if(argv[1] == nullptr || stoi(argv[1]) < 20)
        cerr << "\nWarning: incorrect matrix size -> by default matrix set to 20x20.\n";

    Matrix mat(N);

    my_start << mat;

    // ==== Parameters ==== 

    const int STEPS = stoi(argv[2]);

    const double alpha = 0.5;   // thermal coefficient
    const double dt = 0.1;  // time step

    // ==== Actualization algorithm ====

    Matrix temp(N);

    for(m = 0; m < STEPS; ++m)
    {
        temp = mat;
        for(i = 1; i < N - 1; ++i)
        {
            for(j = 1; j < N - 1; ++j) 
            {
                // sarà il metodo più efficiente?
                if( (i == 4 && j == 4) || (i == 19 && j == 19) )
                    continue;
                
                mat(i, j) = temp(i, j) + alpha * dt * ( temp(i+1,j) + temp(i,j+1) + temp(i-1,j) + temp(i,j-1) - 4*temp(i,j) );
            }
        }
    }

    my_out << mat;

    my_start.close(); my_out.close();

    cerr << '\n';

    return 0;
}