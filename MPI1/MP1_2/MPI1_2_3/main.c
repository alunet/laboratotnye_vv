#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rang, nbproc, token, dest,sum;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rang);
    MPI_Comm_size (MPI_COMM_WORLD, &nbproc);
    
    token = 1;
    sum = 0;

    if (sum==0) {
        dest = 0;
        do{        
            MPI_Send (&token, 1, MPI_INT ,dest ,0 , MPI_COMM_WORLD ) ;
            dest = dest+1;
            sum =1;
          }while (dest<nbproc); 
    } else { 
        MPI_Recv  (&token, 1, MPI_INT ,dest,0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE) ;
        do{
            MPI_Send (&token, 1, MPI_INT ,dest ,0 , MPI_COMM_WORLD ) ;
            dest = dest+1;
          } while (dest<nbproc);
    }
    
    MPI_Finalize();
    
    return 0;
}

