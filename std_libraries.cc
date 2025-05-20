#include "std_libraries.hh"

double mean(const double* arr, const int& size)
{
    double mean = 0;
    for(int i = 0; i < size; ++i)
    {
        cout << arr[i] <<endl;
        mean += arr[i];
    }
    return mean/size;
}

void print_stats(const double* arr, const int& size)
{
    fstream my_out;
    my_out.open("statistics.txt", ios::out);

    my_out << "---------------- Statistics ----------------\n";
    my_out << "Mean: " << mean(arr, size) << '\n';
    my_out << "Standard deviation: " << '\n';
    my_out << "--------------------------------------------\n\n";

    for(int i = 0; i < size; ++i)
        my_out << arr[i] << '\n';

    my_out.close();
}
