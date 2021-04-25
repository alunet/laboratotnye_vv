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


void GridSize(int proc_num, int &row_num, int &column_num) {
    int square_root = ceil(sqrt(proc_num));
    row_num = proc_num;
    int dif = row_num - square_root;
    for (int i = 1; i <= square_root; i++) { 
        if (proc_num % i == 0) {
            int dif_i = proc_num / i - square_root;
            if (dif_i < dif) {
                row_num = proc_num / i;
                dif = dif_i;
            }
        }
    }
    column_num = proc_num / row_num;        
}

int main() {

    int size = 100;
    int* A = new int[size * size];
    int* b = new int[size];

    random_array(A, size * size);
    random_array(b, size);

    //rows splitting
    int* result1 = new int[size];
    #pragma omp parallel for num_threads(THREADS_NUM) shared(A, b, result1)     
    for (int i = 0; i < size; i++) {
        result1[i] = 0;
        for (int j = 0; j < size; j++) {
            result1[i] += A[i * size + j] * b[j];
        }
    }
    return result1;

    //columns splitting
    int* result2 = new int[size];   
    for (int i = 0; i < size; i++) {
        int temp_result2 = 0;
        #pragma omp parallel for num_threads(THREADS_NUM) shared(A, b, result2) reduction(+ : temp_result2)
        for (int j = 0; j < size; j++) {
            temp_result2 += A[i * size + j] * b[j];
        }
        result2[i] = temp_result2;
    }
    return result2;

    //blocks splitting
    int procs4row, procs4column;
    GridSize(THREADS_NUM, procs4row, procs4column);
    assert(procs4row * procs4column == THREADS_NUM);
    int block_size_x = size / procs4row;
    int block_size_y = size / procs4column;

    int* result3 = new int[size];
    #pragma omp parallel num_threads(THREADS_NUM) shared(A, b, result3) 
    {   
        int proc_rank = omp_get_thread_num();
        int i_shift = (proc_rank / procs4column) * block_size_x;
        int j_shift = (proc_rank % procs4column) * block_size_y;
        
        int* temp_result3 = new int[size];
        for (int i = 0; i < size; i++) {
            temp_result3[i] = 0;
        }

        for (int i = 0; i < block_size_x; i++) {
            int temp_result3_i = 0;
            for (int j = 0; j < block_size_y; j++) {
                temp_result3_i += A[(i + i_shift) * size + j + j_shift] * b[j + j_shift];
            }
            temp_result3[i + i_shift] = temp_result3_i;
        }

        #pragma omp critical
        for (int i = 0; i < size; i++) {
            result3[i] += temp_result3[i];
        } 

        delete [] temp_result3;
    }
    return result3;

    return 0;
}
