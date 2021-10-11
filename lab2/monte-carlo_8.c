#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

const int n = pow(10, 8);

double getrand() { return (double)rand() / RAND_MAX; }

double funcMC(double x, double y) {
    return exp(pow(x + y, 2)); // Ω = {x ∈ (0, 1), y ∈ (0, 1 − x)}
}

int main(int argc, char **argv) {
    int commsize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(rank);
    double t = MPI_Wtime();
    double in = 0;
    double s = 0;
    for (int i = rank; i < n; i += commsize) {
        double x = getrand(); /* x in (0, 1) */
        double y = getrand(); /* y in (0, 1 - x) */
        if (x > 0 && x < 1 && y > 0 && y < (1 - x)) {
            in++;
            s += funcMC(x, y);
        }
    }
    t = MPI_Wtime() - t;
    double tmax;
    double global_s, global_in;
    MPI_Reduce(&t, &tmax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&s, &global_s, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&in, &global_in, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double v = global_in / n;
        double res = v * global_s / global_in;
        printf("Time %lf, res = %lf\n", tmax, res);
        FILE * pFile;
        pFile = fopen ("monte_carlo_8.dat","a");
        fprintf (pFile, "%d\t%lf\n", commsize, tmax);
        fclose (pFile);
    }
    MPI_Finalize();
    return 0;
}
