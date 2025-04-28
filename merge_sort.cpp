#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <omp.h>
using namespace std;

// Merge function
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort using OpenMP
void parallelMergeSort(vector<int>& arr, int left, int right, int depth = 0) {
    if (left < right) {
        int mid = (left + right) / 2;
        if (depth < 4) {
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, left, mid, depth + 1);
                #pragma omp section
                parallelMergeSort(arr, mid + 1, right, depth + 1);
            }
        } else {
            sequentialMergeSort(arr, left, mid);
            sequentialMergeSort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
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
    sequentialMergeSort(arr1, 0, SIZE - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << end - start << " sec" << endl;

    start = omp_get_wtime();
    parallelMergeSort(arr2, 0, SIZE - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << end - start << " sec" << endl;

    cin.get();
    return 0;
}
