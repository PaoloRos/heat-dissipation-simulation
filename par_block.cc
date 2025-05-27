#include "matrix.hh"

// argv[1]: matrix size; argv[2]: calculation steps; argv[3]: execution repetitions
int main(const int argc, const char **argv)
{
    if(argv[1] == nullptr) {
        cerr << "Error: matrix size not entered!\n";
        exit(-1);
    }

    if( (stoi(argv[1]) % THD) != 0 ) {  // gestire se nullptr
        cerr << "Error: size of matrix MUST be a multiple of " << THD << "threads!\n";
        exit(-1);
    }

    // ==== I/O strem opening ====

    fstream my_start, my_out;
    my_start.open("starting_mat.txt", ios::out);
    my_out.open("par_output.txt", ios::out);

    if(my_start.fail() || my_out.fail()) {
        cerr << "Error in opening file.\n";
        return -1;
    }

    // setting to format %3.2f
    my_start << setw(3) << fixed << setprecision(2);
    my_out << setw(3) << fixed << setprecision(2);

    int i, t;   // counters

    // ==== Matrix generation ====

    /* $$$$ aggiungi un check per dimensione short $$$$ */
    // by default 24 -> considering I've 4 core
    const short N = (argv[1] == nullptr || stoi(argv[1]) < HS_POS_2 + 1)? 24 : stoi(argv[1]);

    if(argv[1] == nullptr || stoi(argv[1]) < HS_POS_2 + 1)
    { cerr << "\nWarning: incorrect matrix size -> by default matrix set to 24x24.\n"; }

    Matrix mat(N);

    my_start << mat;

    // ==== Parameters ==== 

    const int RUN = 50 + WARMUP;//(argv[3] == nullptr || stoi(argv[3]) < 100)? 100 + WARMUP : stoi(argv[3]) + WARMUP;
    double* exe_result = new double[RUN];

    const int STEP = (argv[2] == nullptr || stoi(argv[2]) < 1000)? 1000 : stoi(argv[2]);
    if(argv[2] == nullptr || stoi(argv[2]) < 1000)
    { cerr << "\nWarning: incorrect calculation steps -> by default set to 1000.\n"; }

    const double alpha = 0.5;   // thermal coefficient
    const double dt = 0.1;  // time step
    const double epsilon = 0.0001;  // minimum discard
    bool stop = false;
    double diff = 0;

    double start_t, end_t;

    // Variables to divide the matrix in multiple smaller matrices
    const short blocks_per_row = sqrt(THD);
    const short B = N / blocks_per_row; // block dimension
    
    Matrix backup = mat;
    Matrix temp(N, true);
    Matrix submat(B+1, true);

    // ==== Actualization algorithm ====
    
    // per il debug
    fstream out_temp[4];
    out_temp[0].open("temp0.txt", ios::out);
    out_temp[1].open("temp1.txt", ios::out);
    out_temp[2].open("temp2.txt", ios::out);
    out_temp[3].open("temp3.txt", ios::out);
    
    for(i = 0; i < 4; ++i)
        out_temp[i] << setw(3) << fixed << setprecision(2);

    for(int exe_i = 0; exe_i < 1/*RUN*/; ++exe_i)
    {
        start_t = omp_get_wtime();

        for(t = 0; t < STEP && !stop; ++t)   //cycle that flows through time
        {
            cerr <<t << ": prima copia\n";

            //copy of the temporary matrix
            #pragma omp parallel for num_threads(THD)
            for(int k = 0; k < N*N; ++k)
                temp[k] = mat[k];

            cerr << t <<": dopo copia\n";
            cerr << t << ": prima body\n";

            //matrix body actualization
            #pragma omp parallel num_threads(THD)
            {
                const short t_ID = omp_get_thread_num();

                // Division in multiple matrices (chatGPT)
                const short block_row = t_ID / blocks_per_row;
                const short block_col = t_ID % blocks_per_row;
                const short r_on_mat = block_row * (B - 1), c_on_mat = block_col * (B - 1); // position first el. of the external block in the original matrix

                for(short r = 1; r < B; ++r)
                    for(short c = 1; c < B; ++c)
                        mat(r_on_mat + r, c_on_mat + c) = temp(r_on_mat + r, c_on_mat + c) + alpha * dt * ( temp(r_on_mat + r + 1, c_on_mat + c) + temp(r_on_mat + r, c_on_mat+ c + 1) + temp(r_on_mat + r - 1, c_on_mat + c) + temp(r_on_mat + r, c_on_mat + c - 1) - 4*temp(r_on_mat + r, c_on_mat + c) );
            }

            cerr << t <<": dopo body\n";

            //heat sources restoring
            mat(HS_POS_1, HS_POS_1) = HEAT_SOURCE_1;
            mat(HS_POS_2, HS_POS_2) = HEAT_SOURCE_2;
            
            //my_out << mat <<"\n\n";
            cerr << t <<": prima borrrdi\n";
            
            //border actualization
            #pragma omp parallel sections    //potenziare operazione per simd
            {
                #pragma omp section //first row
                {
                    //#pragma omp simd
                    for(int k = 1; k < N - 1; ++k)
                        mat(0, k) = mat(1, k);
                }
                #pragma omp section //last row
                {
                    //#pragma omp simd
                    for(int k = 1; k < N - 1; ++k)
                        mat(N - 1, k) = mat(N - 2, k);
                }
                #pragma omp section //first column
                {
                    //#pragma omp simd
                    for(int k = 1; k < N - 1; ++k)
                        mat(k, 0) = mat(k, 1);
                }
                #pragma omp section //last column
                {
                    //#pragma omp simd
                    for(int k = 1; k < N - 1; ++k)
                        mat(k, N - 1) = mat(k, N - 2);
                }
            }

            cerr << t <<": dopo borrrdi\n";
/*            
            //discard calculation
            //#pragma omp parallel for reduction(+:diff) //num_threads(THD)
            for(i = 0; i < N*N; ++i)
                diff += mat[i] - temp[i];
                
            if(diff < epsilon)
                stop = true;
            else
                diff = 0;
            
            cerr << t <<": dopo epsilon\n";
*/
        }

        end_t = omp_get_wtime();
        exe_result[exe_i] = end_t - start_t;

        // SCOMMENTARE per effettuare statistiche
        if(exe_i == RUN - 1) { my_out << mat; }
        my_out << mat;  //COMMENTARE per le statistiche


        cerr << "exe_iteration: " << exe_i + 1 << " of " << RUN <<", diff: " <<diff <<'\n' ;
        
        //restore variables
        mat = backup;
        stop = false;
        diff = 0;

    }

    print_stats(exe_result, RUN);

    my_start.close(); my_out.close();

    delete[] exe_result;



    cerr << '\n';

    return 0;
}

/*
#pragma omp parallel firstprivate(submat) num_threads(THD)
            {
                const short t_ID = omp_get_thread_num();

                // Division in multiple matrices (chatGPT)
                const short block_row = t_ID / blocks_per_row;
                const short block_col = t_ID % blocks_per_row;
                const short r_on_mat = block_row * (B - 1), c_on_mat = block_col * (B - 1); // position first el. of the external block in the original matrix

                submat.copy_subMatrix(mat, r_on_mat, c_on_mat);

                for(short r = 1; r < B; ++r)
                    for(short c = 1; c < B; ++c)
                        //mat(r_on_mat + r, c_on_mat + c) = submat(r, c) + alpha * dt * ( submat(r + 1,c) + submat(r, c + 1) + submat(r - 1, c) + submat(r, c - 1) - 4*submat(r, c) );
            }
*/


// Commenti del debug


                //out_temp[t_ID] << block_row << " -- " << block_col << " -- " << y_0 << " -- " << x_0 << '\n';
                //out_temp[t_ID] << '\n' << t_ID << ": Prima:\n" << temp << '\n';

//if( (r_on_mat + r == HS_POS_1 && c_on_mat + c == HS_POS_1) || (r_on_mat + r == HS_POS_2 && c_on_mat + c == HS_POS_2) ) {out_temp[t_ID] << "\n continua \n "; continue; }

                        //out_temp[t_ID] << r <<" , " << c << ": "<< "mat(" <<r_on_mat + r <<", " << c_on_mat + c << ") = " << temp(r,c) << "+" << alpha << "*" <<dt << "* ( " << temp(r+1,c) <<"+" << temp(r,c+1) << "+" <<temp(r-1,c) <<"+" <<temp(r,c-1) <<"-" <<4 <<"*" <<temp(r,c) << ")";

                        //out_temp[t_ID] << " = " << mat(r_on_mat + r, c_on_mat + c) << '\n';

                        //out_temp[t_ID] << '\n' << t_ID << ": Dopo:\n" << mat;
