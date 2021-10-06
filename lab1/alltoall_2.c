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
    MPI_Request reqs[commsize * 2];
    MPI_Status stats[commsize * 2];
    int count = 1024;
    char *sendbuf = malloc(sizeof(*sendbuf) * count * commsize);
    char *recvbuf = malloc(sizeof(*recvbuf) * count * commsize);

    double t = MPI_Wtime();

    #if 0
    int j = rank;
    do {
        MPI_Isend(sendbuf + j * count, count, MPI_CHAR, j, 0, MPI_COMM_WORLD,
                  &reqs[j]);
        MPI_Irecv(recvbuf + j * count, count, MPI_CHAR, j, 0, MPI_COMM_WORLD,
                  &reqs[commsize + j]);
        j = (j + 1) % commsize;
    } while (j != rank);
    #endif

    for (int i = rank; i < commsize + rank; i++) {
        MPI_Isend(sendbuf + (i % commsize) * count, count, MPI_CHAR, i % commsize, 0, MPI_COMM_WORLD,
                  &reqs[i % commsize]);
        MPI_Irecv(recvbuf + (i % commsize) * count, count, MPI_CHAR, i % commsize, 0, MPI_COMM_WORLD,
                  &reqs[commsize + (i % commsize)]);
    }
    MPI_Waitall(commsize * 2, reqs, stats);
    t = MPI_Wtime() - t;
    double tmax;
    MPI_Reduce(&t, &tmax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Tmax = %lf\n", tmax);
    }

    free(sendbuf);
    free(recvbuf);
    MPI_Finalize();
    return 0;
}
