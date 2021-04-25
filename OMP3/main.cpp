#include <omp.h>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <iterator>
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

void sort(int* array, int size)
{
    bool isSorted = false;
    bool isOdd = true;
    int swap;

    while (!isSorted)
    {
        isSorted = true;

        for(int i = isOdd ? 0 : 1; i < size-1; i+=2)
        {
            if(array[i] > array[i+1])
            {
                isSorted = false;

                swap = array[i];
                array[i] = array[i+1];
                array[i+1] = swap;
            }
        }

        isOdd = !isOdd;
    }
}

void sort_parallel(int* array, int size)
{
    bool isSorted = false;
    bool isOdd = true;
    int swap;

    while (!isSorted)
    {
        isSorted = true;
		#pragma omp parallel num_threads(THREADS_NUM) private(swap) reduction(&&:isSorted)
		{
			#pragma omp for
                        for(int i = isOdd ? 0 : 1; i < size-1; i+=2)
			{
				if(array[i] > array[i+1])
				{
                                    isSorted = isSorted && false;
				    swap = array[i];
				    array[i] = array[i+1];
				    array[i+1] = swap;
				}
			}
		}
		
        isOdd = !isOdd;
    }
}


int main()
{
    for (int i = 0; i < 10; i++) {
        
        int size = 1000;
	double begin, end;
        int* A = int A[size];
	int* B = int B[size];
        
        random_array(A, size);
	for (int j = 0; j < size; j++)
            B[j] = A[j];
    
	    begin = omp_get_wtime();
	    sort(A, size);
	    end = omp_get_wtime();
	    time_sort = end - begin;
	    
            begin = omp_get_wtime();
	    sort_parallel(B, size);
	    end = omp_get_wtime();
	    time_sort_parallel = end - begin;
    }
    cout << time_sort << endl;
    cout << time_sort_parallel;
    
    return 0;
}
