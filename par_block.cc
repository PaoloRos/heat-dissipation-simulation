#include "matrix.hh"

// argv[1]: matrix size; argv[2]: used threads; argv[3]: calculation steps; argv[4]: execution repetitions
int main(const int argc, const char **argv)
{
    cerr << '\n';

    int i, t;   //counters

    // ==== Read matrix size & used threads ====

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

    try
    {
        if(argv[2] == nullptr)
            throw invalid_argument("Error: number of threads NOT entered!\n\n");
        if(!is_Power_Two( stoi(argv[2]) ))
            throw out_of_range("Error: number of threads must be a power of 2!\n\n");
        
        i = stoi(argv[2]);
    }
    catch(invalid_argument& e) { cerr << e.what(); exit(-1); }
    catch(out_of_range& e) { cerr << e.what(); exit(-1); }
    
    const short THD = short(i);
    cout << "Threads used: " << THD << "\n\n";

    // Ho tolto il controllo se dimensione matrix è multipla di THD: non serve per come ho strutturaro il codice (credo)
    // tanto le prove le effettuero sempre su numeri pari --> verifica se devo mettere il check

    // ==== I/O stream opening ====

    fstream my_start, my_out;
    my_start.open("starting_mat.txt", ios::out);
    my_out.open("par_output.txt", ios::out);

    fstream my_csv;
    my_csv.open("par_time.csv", ios::out);

    if(my_start.fail() || my_out.fail() || my_csv.fail()) {
        cerr << "Error in opening files.\n";
        return -1;
    }

    // setting to format %3.2f
    my_start << setw(3) << fixed << setprecision(2);
    my_out << setw(3) << fixed << setprecision(2);

    // per il debug
    fstream out_temp[4];
    out_temp[0].open("temp0.txt", ios::out);
    out_temp[1].open("temp1.txt", ios::out);
    out_temp[2].open("temp2.txt", ios::out);
    out_temp[3].open("temp3.txt", ios::out);
    
    for(i = 0; i < 4; ++i)
        out_temp[i] << setw(3) << fixed << setprecision(2);

    // ==== Matrix generation ====

    Matrix mat(N);
    Matrix backup = mat;
    Matrix temp(N, true);

    my_start << mat;
    
    // ==== Parameters ==== 

    const int RUN = (argv[4] == nullptr || stoi(argv[4]) < 50)? 50 + WARMUP : stoi(argv[4]) + WARMUP;
    double* exe_result = new double[RUN];

    const int STEP = (argv[3] == nullptr || stoi(argv[3]) < 1000)? 1000 : stoi(argv[3]);
    //if(argv[3] == nullptr || stoi(argv[3]) < 1000)
    //{ cerr << "\nWarning: incorrect calculation steps -> by default set to 1000.\n"; }

    const double alpha = 0.5;   // thermal coefficient
    const double dt = 0.1;  // time step
    const double epsilon = 0.0001;  // minimum discard
    bool stop = false;
    double diff = 0;

    double start_t, end_t;

    short MAX_SIZE = 64;
    short blocks_per_row, blocks_per_col;
    short B_row, B_col;

    //così distribuisco il lavoro se sono entro i N=64
    if(N*N / THD > MAX_SIZE*MAX_SIZE)
    {
        // Calcola il numero di blocchi per riga/colonna
        blocks_per_row = (N + MAX_SIZE - 1) / MAX_SIZE;
        blocks_per_col = (N + MAX_SIZE - 1) / MAX_SIZE;
        B_row = B_col = MAX_SIZE;
    }
    else
    {
        blocks_per_row = 1 << (int)(log2(THD) / 2); // 2^(floor(log2(THD)/2))
        blocks_per_col = THD / blocks_per_row;
        B_row = N / blocks_per_row, B_col = N / blocks_per_col;
    }

    const short total_blocks = blocks_per_row * blocks_per_col;

    // ==== Actualization algorithm ====

    for(int exe_i = 0; exe_i < RUN; ++exe_i)
    {
        start_t = omp_get_wtime();

        for(t = 0; t < STEP && !stop; ++t)   //cycle that flows through time
        {
            //1. copy of the i matrix
            temp.copy_in_parallel(mat); //faster than use the operator overload

            //2. Matrix body actualization
            #pragma omp parallel num_threads(THD)
            {
                const short t_ID = omp_get_thread_num();

                short block_row, block_col; //identify the quadrant on whic t_ID operates
                unsigned short r_start, c_start, r_end, c_end;  //global index of the first and last element on which the current thread operates
                unsigned short start_r, start_c, end_r, end_c;   //quadrant index of the first and last element, but in global coordinates (spiega meglio)

                short r, c;

                for(short block_idx = t_ID; block_idx < total_blocks; block_idx += THD) 
                {
                    block_row = block_idx / blocks_per_col;
                    block_col = block_idx % blocks_per_col;

                    r_start = block_row * B_row;
                    c_start = block_col * B_col;
                    r_end = (r_start + B_row < N)? r_start + B_row : N; //prevents segmentation fault
                    c_end = (c_start + B_col < N)? c_start + B_col : N;
                    
                    start_r = (r_start == 0) ? 1 : r_start;
                    end_r = (r_end == N) ? N - 1 : r_end;
                    start_c = (c_start == 0) ? 1 : c_start;
                    end_c = (c_end == N) ? N - 1 : c_end;

                    for(r = start_r; r < end_r; ++r)
                        for(c = start_c; c < end_c; ++c)
                            mat(r, c) = temp(r, c) + alpha * dt * ( temp(r + 1, c) + temp(r, c + 1) + temp(r - 1, c) + temp(r, c - 1) - 4 * temp(r, c)
                            );
                }
            }

            //3. heat sources restoring
            mat(HS_POS_1, HS_POS_1) = HEAT_SOURCE_1;
            mat(HS_POS_2, HS_POS_2) = HEAT_SOURCE_2;
            
            //4. border actualization
            #pragma omp parallel sections
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
            #pragma omp parallel for simd schedule(static) reduction(+:diff)
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

    my_start.close(); my_out.close(); my_csv.close();

    delete[] exe_result;

    //*/

    cerr << '\n';

    return 0;
}

/*

    const short blocks_per_row = 1 << (int)(log2(THD) / 2); // 2^(floor(log2(THD)/2))
    const short blocks_per_col = THD / blocks_per_row;
    const short B_row = 8;//N / blocks_per_row; // -> B_row = N_row_max
    const short B_col = 8;//N / blocks_per_col; // N/blocks_per_row
    const short tot_blocks = blocks_per_row * blocks_per_col
     if(B_row > massimo )
  // assegna B_row in base a numero di THD
     analogo a B_col
    const short BLOCK_SIZE = 16;

    const short blocks_per_row = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;
    const short blocks_per_col = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;
    const short tot_blocks = blocks_per_row * blocks_per_col;



#pragma omp parallel num_threads(THD)
            {
                const short t_ID = omp_get_thread_num();
                short block_row, block_col;   //identificano il quadrante su cui opera t_ID
                short r_on_matrix, c_on_matrix; //indici sulla matrice dei primi elementi del blocco
                short start_r, end_r, start_c, end_c; //indice sulla matrice su cui opera t_ID

                //if(STEP%100==0) {out_temp[t_ID] << "prima:\n" << mat << '\n';}
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

            }
*/

