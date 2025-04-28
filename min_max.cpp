#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>  // for rand()
#include <ctime>    // for measuring time

using namespace std;

void generateRandomArray(vector<int>& arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1000; // Random numbers between 0 and 999
    }
}

int main() {
    int size = 1000000; // Size of the array
    vector<int> arr(size);

    // Generate random data
    generateRandomArray(arr, size);

    // Variables to store results
    int minVal, maxVal, sumVal;
    double avgVal;

    // Measure the time taken by parallel reduction operations
    clock_t start = clock();

    #pragma omp parallel
    {
        // Parallel Reduction for Min, Max, Sum
        #pragma omp parallel for reduction(min : minVal) reduction(max : maxVal) reduction(+: sumVal)
        for (int i = 0; i < size; i++) {
            minVal = arr[i];  // Initialize min to the first element
            maxVal = arr[i];  // Initialize max to the first element
            sumVal += arr[i]; // Add current element to sum
        }
    }

    avgVal = static_cast<double>(sumVal) / size; // Calculate average

    clock_t end = clock();

    // Output the results
    cout << "Min Value: " << minVal << endl;
    cout << "Max Value: " << maxVal << endl;
    cout << "Sum Value: " << sumVal << endl;
    cout << "Average Value: " << avgVal << endl;
    cout << "Time Taken: " << double(end - start) / CLOCKS_PER_SEC << " seconds" << endl;

    cin.get();
    return 0;
}
