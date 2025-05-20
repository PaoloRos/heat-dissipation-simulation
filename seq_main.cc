#define EPSILON 0.0001  //aggiungi il check

#include "matrix.hh"

// argv[1]: matrix size; argv[2]: calculation steps; argv[3]: execution repetition
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

    if(argv[1] == nullptr || stoi(argv[1]) < HS_POS_2 + 1)
        cerr << "\nWarning: incorrect matrix size -> by default matrix set to 20x20.\n";

    Matrix mat(N);

    my_start << mat;

    // ==== Parameters ==== 

    const int RUN = (argv[3] == nullptr || stoi(argv[3]) < 50)? 50 : stoi(argv[3]);
    double* exe_result = new double[RUN];
    
    const int STEPS = stoi(argv[2]);

    const double alpha = 0.5;   // thermal coefficient
    const double dt = 0.1;  // time step

    double start_t = 0, end_t = 0;

    // ==== Actualization algorithm ====

    Matrix temp(N);

    for(int exe_i = 0; exe_i < RUN; ++exe_i)
    {
        start_t = omp_get_wtime();
        for(m = 0; m < STEPS; ++m)
        {
            temp = mat;
            for(i = 1; i < N - 1; ++i)
            {
                for(j = 1; j < N - 1; ++j) 
                {
                    if( (i == HS_POS_1 && j == HS_POS_1) || (i == HS_POS_2 && j == HS_POS_2) )
                        continue;

                    mat(i, j) = temp(i, j) + alpha * dt * ( temp(i+1,j) + temp(i,j+1) + temp(i-1,j) + temp(i,j-1) - 4*temp(i,j) );
                }
            }
        }
        end_t = omp_get_wtime();
        exe_result[exe_i] = end_t - start_t;
	    //cout << exe_result[i] << endl;
    }

    my_out << mat;

    print_stats(exe_result, RUN);

    my_start.close(); my_out.close();

    delete[] exe_result;

    cerr << '\n';

    return 0;
}
