#include "matrix.hh"

int main()
{
    const short N = 1024;
    
    const int CACHE = 128000; //128Kbyte

    cout << "\nVariabili allocate: ";
    cout << (1 + 2 + 4 + 4 + 2)*sizeof(short) + (1 + 1 + 1)*sizeof(short) + (1 + 1)*sizeof(short);
    cout << "byte\n";

    cout << "Oggetti allocati (membri + padding): ";
    cout << 2 * sizeof(Matrix) << " byte\n";

    int diff = CACHE - (1 + 2 + 4 + 4 + 2)*sizeof(short) + (1 + 1 + 1)*sizeof(short) + (1 + 1)*sizeof(short) - 2 * sizeof(Matrix);

    cout << "---------\nSpazio rimanente: \n" << diff << '\n';
    cout << '\n';

    for(int i = 1; i < 200; i*=2)
        cout << i << ' ';
    
    return 0;
}

// Algoritmo di calcolo dimensione
// Matrice_classe(membri + 6byte padding) + celle_mem_dinamiche + tolleranza_metodi
// sizeof(Matrix) + N*N*sizeof(double) + tolleranza_metodi

//La sezione parallela più onerosa è 2. Attualizzazione -> se quella è entro i limiti => tutti gli altri sono entro questi limiti

/*
#pragma omp parallel num_threads(THD)
{
    const short t_ID = omp_get_thread_num(); -> 1S
    short block_row, block_col; -> 2S
    unsigned short r_start, c_start, r_end, c_end; -> 4S
    unsigned short start_r, start_c, end_r, end_c; -> 4S
    short r, c; -> 2S
    for(short block_idx = t_ID; block_idx < total_blocks; block_idx += THD) -> 1S + 1S + 1S
    {
        block_row = block_idx / blocks_per_col; -> 1S
        block_col = block_idx % blocks_per_col;
        r_start = block_row * B_row; -> 1S
        c_start = block_col * B_col; -> 1S
        r_end = (r_start + B_row < N)? r_start + B_row : N; -> 1S
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

*/