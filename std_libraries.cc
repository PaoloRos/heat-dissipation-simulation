//===----------------------------------------------------------------------===//
//
// Progetto Heat Dissipation
// Corso di Tecniche di Programmazione Avanzata, Università di Trento
// Autore: Paolo Rossi
// Data: 06/06/2025
//
//===----------------------------------------------------------------------===//

/** @file std_libraries.cc
 *  @brief Implementation of statistics and validation helpers.
 */

#include "std_libraries.hh"

/** @brief Arithmetic mean implementation. */
double mean(const double* arr, const int& size)
{
    double mean = 0;
    for(int i = 0; i < size; ++i)
        mean += arr[i];
    return mean/size;
}

/** @brief Standard deviation implementation. */
double sd(const double* arr, const double& mean, const int& size)
{
    double sd = 0;
    for(int i = 0; i < size; ++i)
        sd += (arr[i] - mean)*(arr[i] - mean);
    return sqrt(sd/size);
}

/** @brief Statistics output implementation. */
void print_stats(const double* arr, const int& size)
{
    fstream my_out;
    my_out.open("statistics.txt", ios::out);

    double mn = mean(arr + WARMUP - 1, size - WARMUP);

    my_out << "---------------- Statistics ----------------\n";
    my_out << "Calculated on " << size - WARMUP << " iteration\n";
    my_out << "Mean: " << mn << " (s)\n";
    my_out << "Standard deviation: " << sd(arr + WARMUP - 1, mn, size - WARMUP) << " (s)\n";
    my_out << "--------------------------------------------\n\n";

    for(int i = 0; i < size; ++i){
        if(i < WARMUP)
            my_out << "Warm up value -> not considered in the statistic: ";
        my_out << arr[i] << '\n';
    }

    my_out.close();
}

/** @brief Power-of-two predicate implementation. */
bool is_Power_Two(const int n)
{ return (n > 0) && ( (n & (n-1)) == 0 ); }

