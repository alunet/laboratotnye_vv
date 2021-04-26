#include "mpi.h"
#include <math.h>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <iostream>
using namespace std;

void random_array(int* array, int size) 
{
    for(int i = 0; i < size; i++) 
    {
    	array[i] = rand();
    }
}

void merge(int* array, int left, int middle, int right) {
    int size_left = middle - left + 1;
    int size_right = right - middle;

    int* half_l = new int[size_left]; 
    int* half_r = new int[size_right];

    for (int i = 0; i < size_left; i++)
        half_l[i] = array[left + i];
    for (int i = 0; i < size_right; i++)
        half_r[i] = array[middle + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < size_left && j < size_right) {
        if (half_l[i] <= half_r[j]) {
            array[k] = half_l[i];
            i++;
        }
        else {
            array[k] = half_r[j];
            j++;
        }
        k++;
    }
 
    while (i < size_left) {
        array[k] = half_l[i];
        i++;
        k++;
    }
 
    while (j < size_right) {
        array[k] = half_r[j];
        j++;
        k++;
    }

    delete [] half_l;
    delete [] half_r;
}

void mergeArrays(int* array_left, int* array_right, int size_left, int size_right, int* array) {
    int i = 0, j = 0, k = 0;

    while (i < size_left && j < size_right) {
        if (array_left[i] <= array_right[j]) {
            array[k] = array_left[i];
            i++;
        }
        else {
            array[k] = array_right[j];
            j++;
        }
        k++;
    }
 
    while (i < size_left) {
        array[k] = array_left[i];
        i++;
        k++;
    }
 
    while (j < size_right) {
        array[k] = array_right[j];
        j++;
        k++;
    }
}

void sort(int* array, int l, int r){
    if (l>=r) {
        return;
    }
    int m = l + (r - l) / 2;
    mergeSort(array, l, m);
    mergeSort(array, m + 1, r);
    merge(array, l, m, r);
}

int main(int argc, char *argv[]) {
    int size = 10000;
    double begin, end;

    int id, numprocs, data, received;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);	
    
    for (int k = 0; k < 5; k++) {
        int* A = new int[size];
        int* B = new int[size];
         
        random_array(A, size);
	for (int j = 0; j < size; j++)
            B[j] = A[j];
        
        if (id == 0) { //sorting
            begin = MPI_Wtime();
            sort(B, 0, size - 1);
	    end = MPI_Wtime();
	    time_sort = end - begin;   
        }

        if (id == 0) {
            begin = MPI_Wtime();
        }
        //Create subarrays
        int size_chunk = ceil(size / numprocs);
        int* chunk = new int[size_chunk];

        //Send subarrays to processes 
        MPI_Scatter(A, size_chunk, MPI_INT, chunk, size_chunk, MPI_INT, 0, MPI_COMM_WORLD);
        if ((id == numprocs - 1) && (size % size_chunk != 0)) {
            size_chunk = size % size_chunk;
        }

        //Sort subarrays parallel
        sort(chunk, 0, size_chunk - 1);
    
        //Calculate the number of levels for merging
        int max_height = ceil(log2(numprocs));

        //Merge subarrays parallel
        int height = 0;
        bool is_right_part = false;
        while (height < max_height && !is_right_part) {
            int step = pow(2, height);
            if (id % step == 0) {
                if ((id / step) % 2 == 0) { 
                    if (id + step > numprocs - 1) {
                        height++;
                        continue;
                    }
                    int size_another_chunk;
                    MPI_Recv(&size_another_chunk, 1, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);;
                    //Get right subarray
                    int* another_chunk = new int[size_another_chunk];
                    MPI_Recv(another_chunk, size, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
                    //Merge two subarrays
                    int* merge_array = new int[size_chunk + size_another_chunk];
                    mergeArrays(chunk, another_chunk, size_chunk, size_another_chunk, merge_array);

                    delete [] chunk;
                    delete [] another_chunk;
                    chunk = merge_array;
                    size_chunk = size_chunk + size_another_chunk;
                    height++;

                    if (height == max_height) {
                        delete [] A;
                        A = chunk;
                    }

                } else { 
                    MPI_Send(&size_chunk, 1, MPI_INT, id - step, 0, MPI_COMM_WORLD);
                    MPI_Send(chunk, size_chunk, MPI_INT, id - step, 0, MPI_COMM_WORLD);
                    delete [] chunk;
                    is_right_part = true;
                }
            }
        }

        if (id == 0) {
            end = MPI_Wtime();
	    time_sort_parallel = end - begin;
        }
    }
    cout << time_sort << endl;
    cout << time_sort_parallel;

    MPI_Finalize();
    
    return 0;
}