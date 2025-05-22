#include "std_libraries.hh"

double mean(const double* arr, const int& size)
{
    double mean = 0;
    for(int i = 0; i < size; ++i)
        mean += arr[i];
    return mean/size;
}

double sd(const double* arr, const double& mean, const int& size)
{
    double sd = 0;
    for(int i = 0; i < size; ++i)
        sd += (arr[i] - mean)*(arr[i] - mean);
    return sqrt(sd/size);
}

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

