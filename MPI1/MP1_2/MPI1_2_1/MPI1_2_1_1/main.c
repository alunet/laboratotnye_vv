#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	int id, numprocs, data;
	data = 0;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);	

	int to_id = id + 1;
	int from_id = id - 1;
	if (to_id >= numprocs) to_id = 0;
	if (from_id < 0) from_id = numprocs - 1;
	
	if (id == 0) {
		data++;
		printf("Proc %d sending data to proc %d\n", id, to_id);
		MPI_Send(&data, 1, MPI_INT, to_id, 0, MPI_COMM_WORLD);
		printf("Proc %d sent data: %d \n", id, data);

		printf("Proc %d receiving data from proc %d\n", id, from_id);
		MPI_Recv (&data, 1, MPI_INT, from_id, 0, MPI_COMM_WORLD, &status);
		printf("Proc %d received data: %d \n", id, data);
	} else {
		printf("Proc %d receiving data from proc %d\n", id, from_id);
                MPI_Recv (&data, 1, MPI_INT, from_id, 0, MPI_COMM_WORLD, &status);
                printf("Proc %d received data: %d \n", id, data);
	
		data++;
	
		printf("Proc %d sending data to proc %d\n", id, to_id);
                MPI_Send(&data, 1, MPI_INT, to_id, 0, MPI_COMM_WORLD);
                printf("Proc %d sent data: %d \n", id, data);
	}
	
	MPI_Finalize();
	
        return 0;
}

