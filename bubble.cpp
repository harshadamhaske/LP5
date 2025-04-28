#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <omp.h>
using namespace std;

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// Parallel Bubble Sort using OpenMP
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        int start = i % 2;
        #pragma omp parallel for
        for (int j = start; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

int main() {
    const int SIZE = 10000;
    vector<int> arr1(SIZE), arr2;

    for (int i = 0; i < SIZE; i++)
        arr1[i] = rand() % 10000;
    arr2 = arr1;

    double start, end;

    start = omp_get_wtime();
    sequentialBubbleSort(arr1);
    end = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << end - start << " sec" << endl;

    start = omp_get_wtime();
    parallelBubbleSort(arr2);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << end - start << " sec" << endl;

    cin.get();
    return 0;
}
