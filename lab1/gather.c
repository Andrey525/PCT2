#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, commsize;
    int root = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int k = 1; k <= 2; k++) {
        int count = 1 * pow(1024, k);
        char *buf;
        if (rank == root) {
            buf = malloc(sizeof(*buf) * count * commsize);
        } else {
            buf = malloc(sizeof(*buf) * count);
        }

        double t = MPI_Wtime();
        if (rank == root) {
            for (int i = 0; i < commsize; i++) {
                if (i == root) {
                    continue;
                }
                MPI_Recv(buf + i * count, count, MPI_CHAR, i, 0, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
            }
        } else {
            MPI_Send(buf, count, MPI_CHAR, root, 0, MPI_COMM_WORLD);
        }
        t = MPI_Wtime() - t;
        double tmax;
        MPI_Reduce(&t, &tmax, 1, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);
        if (rank == root) {
            printf("Tmax = %lf\n", tmax);
        }

        free(buf);
    }
    MPI_Finalize();
    return 0;
}