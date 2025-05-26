#include "matrix.hh"

// argv[1]: matrix size; argv[2]: calculation steps; argv[3]: execution repetitions
int main(const int argc, const char **argv)
{
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

    int i, j, m;   // counters

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

    double start_t, end_t;

    omp_set_num_threads(THD);

    // Variables to divide the matrix in multiple smaller matrices
    const short blocks_per_row = sqrt(THD);
    const short B = N / blocks_per_row; // block dimension
    
    //cerr << blocks_per_row << " -- " << B <<'\n';
    // ==== Actualization algorithm ====

    // per il debug
    fstream out_temp[4];
    out_temp[0].open("temp0.txt", ios::out);
    out_temp[1].open("temp1.txt", ios::out);
    out_temp[2].open("temp2.txt", ios::out);
    out_temp[3].open("temp3.txt", ios::out);

    out_temp[0] << setw(3) << fixed << setprecision(2);
    out_temp[1] << setw(3) << fixed << setprecision(2);
    out_temp[2] << setw(3) << fixed << setprecision(2);
    out_temp[3] << setw(3) << fixed << setprecision(2);

    Matrix backup = mat;

    for(int exe_i = 0; exe_i < 1/*RUN*/; ++exe_i)
    {
        start_t = omp_get_wtime();

        #pragma omp parallel
        {
            const short t_ID = omp_get_thread_num();

            short r, c, k;

            // Division in multiple matrices (chatGPT)
            // block_row e block_col dovrebbero valere sempre 1: per comprensione del codice gli ho messo un 1* davanti
            const short block_row = t_ID / blocks_per_row;
            const short block_col = t_ID % blocks_per_row;
            const short y_0 = block_row * B, x_0 = block_col * B;   // position first el. of the block in the original matrix

            // Temporary matrix: B+1 to include elements on the border (of the submatrix)
            Matrix temp(B+1, true);
            
            for(k = 0; k < STEP; ++k)
            {
                temp.copy_subMatrix(mat, y_0, block_row, x_0, block_col);

                short r_on_mat = y_0 - 1 * block_row;
                short c_on_mat = x_0 - 1 * block_col;
                for(r = 1; r < B; ++r)
                {
                    for(c = 1; c < B; ++c) 
                    {
                        mat(r_on_mat + r, c_on_mat + c) = temp(r,c) + alpha * dt * ( temp(r+1,c) + temp(r,c+1) + temp(r-1,c) + temp(r,c-1) - 4*temp(r,c) );
                    }
                }
                // Ottimizzazione: l'aggiornamento lo effettuerò solo nel thread a cui compete la sotto matrice
                mat(HS_POS_1, HS_POS_1) = HEAT_SOURCE_1;
	            mat(HS_POS_2, HS_POS_2) = HEAT_SOURCE_2;

                //Impone la sincronizzazione per scandire il tempo tra i thread
                #pragma omp barrier
            }

        }

        end_t = omp_get_wtime();
        exe_result[exe_i] = end_t - start_t;

        // SCOMMENTARE per effettuare statistiche
        //if(exe_i == RUN - 1) { my_out << mat; }
        my_out << mat;  //COMMENTARE per le statistiche

        // restore variables
        mat = backup;
        cerr << "exe_iteration: " << exe_i + 1 << " of " << RUN <<'\n';
    }

    print_stats(exe_result, RUN);

    my_start.close(); my_out.close();

    delete[] exe_result;

    cerr << '\n';

    return 0;
}


// Commenti del debug


                //out_temp[t_ID] << block_row << " -- " << block_col << " -- " << y_0 << " -- " << x_0 << '\n';
                //out_temp[t_ID] << '\n' << t_ID << ": Prima:\n" << temp << '\n';

//if( (r_on_mat + r == HS_POS_1 && c_on_mat + c == HS_POS_1) || (r_on_mat + r == HS_POS_2 && c_on_mat + c == HS_POS_2) ) {out_temp[t_ID] << "\n continua \n "; continue; }

                        //out_temp[t_ID] << r <<" , " << c << ": "<< "mat(" <<r_on_mat + r <<", " << c_on_mat + c << ") = " << temp(r,c) << "+" << alpha << "*" <<dt << "* ( " << temp(r+1,c) <<"+" << temp(r,c+1) << "+" <<temp(r-1,c) <<"+" <<temp(r,c-1) <<"-" <<4 <<"*" <<temp(r,c) << ")";

                        //out_temp[t_ID] << " = " << mat(r_on_mat + r, c_on_mat + c) << '\n';

                        //out_temp[t_ID] << '\n' << t_ID << ": Dopo:\n" << mat;
