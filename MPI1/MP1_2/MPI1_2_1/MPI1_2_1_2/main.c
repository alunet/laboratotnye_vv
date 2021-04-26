#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	int id, numprocs, data, iter_num;
	data = -1;
        iter_num = 3;
        MPI_Request request;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);	

	int to_id = id + 1;
	int from_id = id - 1;
	if (to_id >= numprocs) to_id = 0;
	if (from_id < 0) from_id = numprocs - 1;
	
    if (data == -1) data = id;

    for (int i = 0; i < iter_num; i++) {
        printf("Proc %d sending data \"%d\" to proc %d\n", id, data, to_id);
        MPI_Isend(&data, 1, MPI_INT, to_id, 0, MPI_COMM_WORLD, &request);

        MPI_Recv (&data, 1, MPI_INT, from_id, 0, MPI_COMM_WORLD, &status);
        printf("Proc %d received data: %d \n", id, data);
        
        MPI_Wait(&request, &status);
    }

	MPI_Finalize();
	
        return 0;
}

