#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int rank, commsize;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request reqs[commsize * commsize * 2];
    for (int i = 0; i < 32; i++) {
        reqs[i] = MPI_REQUEST_NULL;
    }
    MPI_Status stats[commsize * commsize * 2];
    int count = 3;
    char *sendbuf = malloc(sizeof(*sendbuf) * count * commsize);
    char *recvbuf = malloc(sizeof(*recvbuf) * count * commsize);

    for (int i = 0; i < commsize; i++) {
        *(sendbuf + count * i) = 0x41 + rank;
        *(sendbuf + count * i + 1) = 0x30 + i; 
    }

    for (int i = 0; i < commsize; i++) {
        printf("Rank = %d, sendbuf[%d] = %c%c\n", rank, i, *(sendbuf + count * i), *(sendbuf + count * i + 1));
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < commsize; i++) {
        MPI_Isend(sendbuf + i * count, count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[i + rank * commsize]);
        MPI_Irecv(recvbuf + i * count, count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[commsize * commsize + i + rank * commsize]);
    }
    MPI_Waitall(commsize * commsize * 2, reqs, stats);
    if (rank == 0) {
        printf("Выполнено. Все процессы обменялись сообщениями. Смотри:\n\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < commsize; i++) {
        printf("Rank = %d, recvbuf[%d] = %c%c\n", rank, i, *(recvbuf + count * i), *(recvbuf + count * i + 1));
    }
    
    
    free(sendbuf);
    free(recvbuf);
    MPI_Finalize();
    return 0;
}