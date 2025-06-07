//===----------------------------------------------------------------------===//
//
// Progetto Heat Dissipation
// Corso di Tecniche di Programmazione Avanzata, Università di Trento
// Autore: Paolo Rossi
// Data: 06/06/2025
//
//===----------------------------------------------------------------------===//

#define WARMUP 3    //per scartare le prime misure di tempo

#include <omp.h>

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

double mean(const double*, const int&);

double sd(const double*, const double&, const int&);

void print_stats(const double*, const int&);

bool is_Power_Two(const int);

