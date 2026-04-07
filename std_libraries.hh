//===----------------------------------------------------------------------===//
//
// Progetto Heat Dissipation
// Corso di Tecniche di Programmazione Avanzata, Università di Trento
// Autore: Paolo Rossi
// Data: 06/06/2025
//
//===----------------------------------------------------------------------===//

/** @file std_libraries.hh
 *  @brief Shared includes and numeric/statistics helpers for benchmarking.
 */

/** @brief Number of warm-up runs discarded from statistics. */
#define WARMUP 3

#include <omp.h>

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

/**
 * @brief Computes arithmetic mean of an array.
 * @param arr Pointer to first element.
 * @param size Number of elements.
 * @return Mean value.
 */
double mean(const double*, const int&);

/**
 * @brief Computes standard deviation of an array.
 * @param arr Pointer to first element.
 * @param mean Precomputed mean value.
 * @param size Number of elements.
 * @return Standard deviation.
 */
double sd(const double*, const double&, const int&);

/**
 * @brief Writes benchmark statistics to statistics.txt.
 * @param arr Execution times array.
 * @param size Number of runs.
 */
void print_stats(const double*, const int&);

/**
 * @brief Checks whether an integer is a power of two.
 * @param n Input integer.
 * @return True if @p n is a power of two, false otherwise.
 */
bool is_Power_Two(const int);

