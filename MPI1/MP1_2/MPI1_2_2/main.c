#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rang, nbproc, token, dest;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rang);
    MPI_Comm_size (MPI_COMM_WORLD, &nbproc);

    if (rang == 0) {
        token = 1;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&token, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        dest = token;
        token = 1;
        MPI_Send(&token, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);

    } else {
        token = rang;
        MPI_Send(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        token = 0;
    }

    MPI_Finalize();

    return 0;
}