#include "matrix.hh"

// argv[1]: matrix size; argv[2]: used threads; argv[3]: calculation steps; argv[4]: execution repetitions
int main(const int argc, const char **argv)
{
    // ==== Read matrix size & used threads ====

    if(argv[1] == nullptr) {
        cerr << "Error: matrix size not entered!\n";
        exit(-1);
    }

    // $$$ Check che THD sia potenza di 2!

    const short THD = (argv[2] == nullptr)? 2 : short( stoi(argv[2]) );

    cout << "\nThreads used: " << THD << '\n';

    if( (stoi(argv[1]) % THD) != 0 ) {
        cerr << "Error: size of matrix MUST be a multiple of " << THD << " threads!\n";
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
    const short N = (argv[1] == nullptr || stoi(argv[1]) < HS_POS_2 + 1)? 24 : short( stoi(argv[1]) );

    if(argv[1] == nullptr || stoi(argv[1]) < HS_POS_2 + 1)
    { cerr << "\nWarning: incorrect matrix size -> by default matrix set to 24x24.\n"; }

    Matrix mat(N);

    my_start << mat;

    // ==== Parameters ==== 

    const int RUN = 1;//50 + WARMUP;//(argv[3] == nullptr || stoi(argv[3]) < 100)? 100 + WARMUP : stoi(argv[3]) + WARMUP;
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
    
    const short blocks_per_row = 1 << (int)(log2(THD) / 2); // 2^(floor(log2(THD)/2))
    const short blocks_per_col = THD / blocks_per_row;
    const short B_row = N / blocks_per_row; // -> B_row = N_row_max
    const short B_col = N / blocks_per_col; // N/blocks_per_row

    // if(B_row > massimo )
        // assegna B_row in base a numero di THD
    // analogo a B_col
    

    //const short BLOCK_SIZE = 16;
//
    //const short blocks_per_row = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;
    //const short blocks_per_col = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;
    //const short tot_blocks = blocks_per_row * blocks_per_col;

    Matrix backup = mat;
    Matrix temp(N, true);

    // ==== Actualization algorithm ====
    
    // per il debug
    fstream out_temp[4];
    out_temp[0].open("temp0.txt", ios::out);
    out_temp[1].open("temp1.txt", ios::out);
    out_temp[2].open("temp2.txt", ios::out);
    out_temp[3].open("temp3.txt", ios::out);
    
    for(i = 0; i < 4; ++i)
        out_temp[i] << setw(3) << fixed << setprecision(2);

    for(int exe_i = 0; exe_i < RUN; ++exe_i)
    {
        start_t = omp_get_wtime();

        for(t = 0; t < STEP && !stop; ++t)   //cycle that flows through time
        {
            //1. copy of the temporary matrix
            #pragma omp parallel for simd schedule(simd:static, 16)
            for(int k = 0; k < N*N; ++k)
                temp[k] = mat[k];

            /*
            #pragma omp parallel num_threads(THD)
            {
                const short t_ID = omp_get_thread_num();
                short block_row, block_col;   //identificano il quadrante su cui opera t_ID
                short r_on_matrix, c_on_matrix; //indici sulla matrice dei primi elementi del blocco
                short start_r, end_r, start_c, end_c; //indice sulla matrice su cui opera t_ID

                if(STEP%100==0) {out_temp[t_ID] << "prima:\n" << mat << '\n';}
                for(short block_idx = t_ID; block_idx < tot_blocks; block_idx += THD)
                {
                    //if(STEP%10==0) {out_temp[t_ID] << "prima:\n" << mat << '\n';}

                    // distribuisce i thread LUNGO le righe!
                    block_row = block_idx / blocks_per_col;
                    block_col = block_idx % blocks_per_col;
                    r_on_matrix = block_row * BLOCK_SIZE;
                    c_on_matrix = block_col * BLOCK_SIZE;
                    
                    start_r = (r_on_matrix == 0)? 1 : r_on_matrix;
                    end_r = (r_on_matrix + BLOCK_SIZE == N)? N - 1 : r_on_matrix + BLOCK_SIZE;
                    start_c = (c_on_matrix == 0)? 1 : c_on_matrix;
                    end_c = (c_on_matrix + BLOCK_SIZE == N)? N - 1 : c_on_matrix + BLOCK_SIZE;
                    
                    if(STEP%100==0) {printf("%d: %d | %d | %d | %d | %d | %d | %d | %d\n", t_ID, block_row, block_col, r_on_matrix, c_on_matrix, start_r, end_r, start_c, end_c);}
                    
                    for(short r = start_r; r < end_r; ++r)
                        for(short c = start_c; c < end_c; ++c)
                            mat(r,c) = temp(r,c) + alpha * dt * (
                                temp(r + 1,c) + temp(r,c + 1) + temp(r - 1,c) + 4 * temp(r,c) 
                            );
                    
                    //if(STEP%10==0){out_temp[t_ID] << "dopo:\n" << mat << "\n\n";}
                }
                if(STEP%100==0){out_temp[t_ID] << "dopo:\n" << mat << "\n\n";}

            }*/

            
            #pragma omp parallel num_threads(THD)
            {
                const short t_ID = omp_get_thread_num();
                const short block_row = t_ID / blocks_per_col;
                const short block_col = t_ID % blocks_per_col;
                const short r_on_mat = block_row * B_row;
                const short c_on_mat = block_col * B_col;

                const short start_r = (r_on_mat == 0)? 1 : 0;
                const short end_r = (r_on_mat+B_row == N)? B_row - 1 : B_row;
                const short start_c = (c_on_mat == 0)? 1 : 0;
                const short end_c = (c_on_mat+B_col == N)? B_col - 1 : B_col;

                for(short r = start_r; r < end_r; ++r)
                    for(short c = start_c; c < end_c; ++c)
                        mat(r_on_mat + r, c_on_mat + c) = temp(r_on_mat + r, c_on_mat + c) + alpha * dt * (
                            temp(r_on_mat + r + 1, c_on_mat + c) +
                            temp(r_on_mat + r, c_on_mat + c + 1) +
                            temp(r_on_mat + r - 1, c_on_mat + c) +
                            temp(r_on_mat + r, c_on_mat + c - 1) -
                            4 * temp(r_on_mat + r, c_on_mat + c)
                        );
            }           
            

            //3. heat sources restoring
            mat(HS_POS_1, HS_POS_1) = HEAT_SOURCE_1;
            mat(HS_POS_2, HS_POS_2) = HEAT_SOURCE_2;
            
            //4. border actualization
            #pragma omp parallel sections   //ragionaci su se conviene simd
            {
                #pragma omp section //first row
                {
                    for(int k = 1; k < N - 1; ++k)
                        mat(0, k) = mat(1, k);
                }
                #pragma omp section //last row
                {
                    for(int k = 1; k < N - 1; ++k)
                        mat(N - 1, k) = mat(N - 2, k);
                }
                #pragma omp section //first column
                {
                    for(int k = 1; k < N - 1; ++k)
                        mat(k, 0) = mat(k, 1);
                }
                #pragma omp section //last column
                {
                    for(int k = 1; k < N - 1; ++k)
                        mat(k, N - 1) = mat(k, N - 2);
                }
            }

            //5. discard calculation
            #pragma omp parallel for reduction(+:diff) //simd schedule(simd:static, 8) -> sembra non convenire
            for(int k = 0; k < N*N; ++k)
                diff += mat[k] - temp[k];
                
            if(diff < epsilon)
                stop = true;
            else
                diff = 0;
            
        }

        end_t = omp_get_wtime();
        exe_result[exe_i] = end_t - start_t;

        // SCOMMENTARE per effettuare statistiche
        if(exe_i == RUN - 1) { my_out << mat; }
        //my_out << mat;  //COMMENTARE per le statistiche

        cerr << "exe_iteration: " << exe_i + 1 << " of " << RUN << " | time " << end_t - start_t <<" | diff: " <<diff <<'\n' ;
        
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

// + idx/subTHD * block_per_row
//idx%subTHD * block_per_row
