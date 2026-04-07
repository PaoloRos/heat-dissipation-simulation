/** @file seq_main.cc
 *  @brief Sequential finite-difference solver for 2D heat dissipation.
 *
 * The code evolves a temperature field over a square plate with two fixed heat
 * sources and reflective (zero-normal-gradient) boundary conditions.
 */

#include "matrix.hh"

/**
 * @brief Runs the sequential benchmark and writes outputs/timings to disk.
 * @param argc Number of CLI arguments.
 * @param argv CLI argument vector.
 * @return 0 on success, non-zero on error.
 *
 * Arguments:
 * - argv[1]: matrix size
 * - argv[2]: maximum update steps
 * - argv[3]: number of repetitions
 * - argv[4]: convergence epsilon
 */
int main(const int argc, const char **argv)
{
    cerr << '\n';

    int i, j, m;


    // ==== Lettura dimensione matrice e numero di thread ====

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


    // ==== Inizializzazione flusso I/O ====

    fstream my_out;
    my_out.open("temp_output.txt", ios::out);

    fstream my_csv;
    my_csv.open("time.csv", ios::out);

    if(my_csv.fail() || my_out.fail()) {
        cerr << "Error in opening file.\n";
        return -1;
    }

    my_out << setw(3) << fixed << setprecision(2);


    // ==== Generazione matrici ====

    Matrix mat(N);
    Matrix temp(N);
    Matrix backup = mat;


    // ==== Parametri ==== 

    const int RUN = (argv[2] == nullptr || argv[3] == nullptr || stoi(argv[3]) < 50)? 50 + WARMUP : stoi(argv[3]) + WARMUP;
    double* exe_result = new double[RUN];

    const int STEP = (argv[2] == nullptr || stoi(argv[2]) < 1000)? 1000 : stoi(argv[2]);

    const double alpha = 0.5;   // thermal coefficient
    const double dt = 0.1;  // time step
    const double epsilon = stod(argv[4]);  // minimum discard
    bool stop = false;
    double diff = 0;

    double start_t, end_t;


    // ==== Algortimo di attualizzazione ====

    for(int exe_i = 0, mm = 0; exe_i < RUN; ++exe_i)
    {
        start_t = omp_get_wtime();

        for(m = 0; m < STEP && !stop; ++m)
        {
            // 1. Copia in variabile temporanea
            temp = mat;
            
            // 2. Aggiornamento corpo della matrice
            for(i = 1; i < N - 1; ++i)
                for(j = 1; j < N - 1; ++j) 
                    mat(i, j) = temp(i, j) + alpha * dt * ( temp(i+1,j) + temp(i,j+1) + temp(i-1,j) + temp(i,j-1) - 4*temp(i,j) );
	        
            // 3. Sorgenti di calore
            mat(HS_POS_1, HS_POS_1) = HEAT_SOURCE_1;
	        mat(HS_POS_2, HS_POS_2) = HEAT_SOURCE_2;

            // 4. Aggiornamento bordi
            for(i = 1; i < N - 1; ++i)
            {
                mat(0, i) = mat(1, i);
                mat(N-1, i) = mat(N-2, i);
            }
            for(i = 1; i < N - 1; ++i)
            {
                mat(i, 0) = mat(i, 1);
                mat(i, N-1) = mat(i, N-2);
            }

            // 5. Calcolo dello scarto
            for(i = 0; i < N*N; ++i)
                diff += mat[i] - temp[i];

            // 6. Check di terminazione
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
            if(stop) { cout << "\t Time step: " << m << '\n'; }
        }

        cerr << "exe_iteration: " << exe_i + 1 << " of " << RUN << " | time: " << end_t - start_t <<" | diff: " <<diff <<'\n';

        // Ripristino variabili
        mat = backup;
        stop = false;
        diff = 0;
    }

    print_stats(exe_result, RUN);

    my_csv.close(); my_out.close();

    delete[] exe_result;

    cerr << '\n';

    return 0;
}
