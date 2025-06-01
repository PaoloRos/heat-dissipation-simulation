#define WARMUP 3   //to discard the first executions due to warm up

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

