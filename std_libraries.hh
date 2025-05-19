#include <omp.h>

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

double mean(const double* arr, const int size)
{
    double mean = 0;
    for(int i = 0; i < size; ++i)
        mean += arr[i];
    return mean/size;
}