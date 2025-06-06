//===----------------------------------------------------------------------===//
//
// Progetto Heat Dissipation
// Corso di Tecniche di Programmazione Avanzata, Università di Trento
// Autore: Paolo Rossi
// Data: 06/06/2025
//
//===----------------------------------------------------------------------===//

#define HEAT_SOURCE_1 5
#define HEAT_SOURCE_2 3
#define HS_POS_1 5
#define HS_POS_2 20

#include "std_libraries.hh"

// Matrice quadrata con gli elementi disposti in un array monodimensionale
class Matrix
{
    public:
    
        // Inizializza una matrice di dimensione 'size' a valori nulli. Se 'null' == false, allora gli elementi in posizione (5,5) e (20,20) hanno valori di default
        Matrix(short size, bool zero = false);

        Matrix(const Matrix&);

        ~Matrix();

        int get_size() const;

        void get_ID(const short r, const short c = -1);

        // Ottimizza la copia degli elementi di una matrice: l'accesso diretto al puntatore senza passare per l'overoading di '[]' diminuisce i tempi
        void copy_in_parallel(const Matrix&, const int chunk_size);

        Matrix& operator=(const Matrix&);

        // Accesso all'elemento per coordinate (riga, colonna)
        double& operator()(const int, const int) const;

        // Accesso all'elemento per indice lineare
        double& operator[](const int) const;

        //double operator-(const Matrix&) const; -> verifica che non serva

        friend ostream& operator<<(ostream&, const Matrix&);

    private:
        double* el;
        short N;
};

ostream& operator<<(ostream&, const Matrix&);

inline int Matrix::get_size() const { return this->N; }

inline double& Matrix::operator()(const int r, const int c) const { return this->el[r*this->N + c]; }

inline double& Matrix::operator[](const int idx) const { return this->el[idx]; }

