#include <omp.h>
#include <cassert>
#include <cmath>
#include <chrono>
#include <iostream>
using namespace std;

#define THREADS_NUM 2

void random_array(int* array, int size) 
{
    for(int i = 0; i < size; i++) 
    {
    	array[i] = rand();
    }
}

int main() {

    int size = 100;
    int* A = new int[size * size];
    int* B = new int[size * size];

    random_array(A, size * size);
    random_array(B, size * size);

    //tape splitting
    int* result1 = new int[size * size];
    #pragma omp parallel for num_threads(THREADS_NUM) shared(A, B, result1)         
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result1[i*size + j] = 0;
            for (int k = 0; k < size; k++)
                result1[i*size + j] += A[i*size + k] * B[k*size + j];
        }
    }
    return result1;

    //block splitting
    int* result2 = new int[size * size];
    int block_size = size / THREADS_NUM;

    #pragma omp parallel for schedule(dynamic, block_size) collapse(2) shared(A, B, result2)
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            result2[i*size + j] = 0;
            for(int k=0; k < size; k++) {
                result2[i*size + j] += A[i*size + k] * B[k * size + j];
            }
        }
    }
    return result2;

    return 0;
}
