#include "matrix.hh"

int main()
{
    Matrix mat(1024);
    cout << "Size of:\n";
    
    cout << "1: " << sizeof(double**) + sizeof(short);

    cout << '\n';
    
    return 0;
}

// Algoritmo di calcolo dimensione
// Matrice_classe(membri + 6byte padding) + celle_mem_dinamiche + tolleranza_metodi
// sizeof(Matrix) + N*N*sizeof(double) + tolleranza_metodi

//La sezione parallela più onerosa è 2. Attualizzazione -> se quella è entro i limiti => tutti gli altri sono entro questi limiti