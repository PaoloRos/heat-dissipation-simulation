#include "matrix.hh"

// argv[1]: matrix size; argv[2]: calculation step; argv[3]: execution repetitions
int main(const int argc, const char **argv)
{
    cerr << '\n';

    int i, j, m;

    // ==== Read matrix size ====

    try
    {
        if(argv[1] == nullptr)
            throw invalid_argument("Error: matrix size not entered!\n\n");
        if(stoi(argv[1]) < HS_POS_2 + 1)    // check whether the second heat source is included
            throw 1;
        if(stoi(argv[1]) > numeric_limits<short>::max())
            throw out_of_range("Error: matrix size too big!");
        
        i = stoi(argv[1]);
    }
    catch(invalid_argument& e) { cerr << e.what(); exit(-1); }
    catch(int e) {
        cerr << "Warning: incorrect matrix size -> by default matrix set to " << HS_POS_2 + 1 << 'x' << HS_POS_2 + 1 << ".\n";
        i = HS_POS_2 + 1;
    }
    catch(out_of_range& e) {
        cerr << e.what() << " -> size must be lower than " << numeric_limits<short>::max() << ".\n";
        exit(-1);
}

    const short N = short(i);

    // ==== I/O strem opening ====

    //fstream my_out[MEASURE_MAT];
    //open_file(my_out);

    fstream my_out;
    my_out.open("temp_output.txt", ios::out);

    fstream my_csv;
    my_csv.open("time.csv", ios::out);

    if(my_csv.fail() || my_out.fail()) {
        cerr << "Error in opening file.\n";
        return -1;
    }

    my_out << setw(3) << fixed << setprecision(2);

    // ==== Matrix generation ====

    Matrix mat(N);
    Matrix temp(N);
    Matrix backup = mat;

    // ==== Parameters ==== 

    const int RUN = (argv[3] == nullptr || stoi(argv[3]) < 50)? 50 + WARMUP : stoi(argv[3]) + WARMUP;
    double* exe_result = new double[RUN];

    const int STEP = (argv[2] == nullptr || stoi(argv[2]) < 1000)? 1000 : stoi(argv[2]);

    const double alpha = 0.5;   // thermal coefficient
    const double dt = 0.1;  // time step
    const double epsilon = 0.0001;  // minimum discard
    bool stop = false;
    double diff = 0;

    double start_t, end_t;

    // ==== Actualization algorithm ====

    for(int exe_i = 0, mm = 0; exe_i < RUN; ++exe_i)
    {
        start_t = omp_get_wtime();

        for(m = 0; m < STEP && !stop; ++m)
        {
            temp = mat;
            
            for(i = 1; i < N - 1; ++i)
                for(j = 1; j < N - 1; ++j) 
                    mat(i, j) = temp(i, j) + alpha * dt * ( temp(i+1,j) + temp(i,j+1) + temp(i-1,j) + temp(i,j-1) - 4*temp(i,j) );
	        
            mat(HS_POS_1, HS_POS_1) = HEAT_SOURCE_1;
	        mat(HS_POS_2, HS_POS_2) = HEAT_SOURCE_2;

            // updates first and last rows
            for(i = 1; i < N - 1; ++i)
            {
                mat(0, i) = mat(1, i);
                mat(N-1, i) = mat(N-2, i);
            }

            //updates first and last columns
            for(i = 1; i < N - 1; ++i)
            {
                mat(i, 0) = mat(i, 1);
                mat(i, N-1) = mat(i, N-2);
            }

            for(i = 0; i < N*N; ++i)
                diff += mat[i] - temp[i];

            if(diff < epsilon)
                stop = true;
            else
                diff = 0;
        }

        end_t = omp_get_wtime();
        exe_result[exe_i] = end_t - start_t;
        my_csv << end_t - start_t << '\n';

        if(exe_i == RUN - 1){ 
            my_out << mat;
            //my_out[mm] << mat;
            //++mm;
        }

        cerr << "exe_iteration: " << exe_i + 1 << " of " << RUN << " | time: " << end_t - start_t <<" | diff: " <<diff <<'\n';

        //restore variables
        mat = backup;
        stop = false;
        diff = 0;
    }

    print_stats(exe_result, RUN);

    my_csv.close(); my_out.close();

    //for(i = 0; i < MEASURE_MAT; ++i)
    //    my_out[i].close();

    delete[] exe_result;

    cerr << '\n';

    return 0;
}
