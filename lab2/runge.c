#include <math.h>
#include <mpi.h>
#include <stdio.h>

double func(double x) { return x / (pow(sin(2 * x), 3)); }

const double eps = 1E-6;
const int n0 = pow(10, 7);

int main(int argc, char *argv[]) {
    const double a = 0.1;
    const double b = 0.5;

    int commsize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = n0, k;
    double sq[2], delta = 1;

    double t = MPI_Wtime();
    for (k = 0; delta > eps; n *= 2, k ^= 1) {
        double h = (b - a) / n;
        double s = 0.0;
        for (int i = rank; i < n; i += commsize) {
            s += func(a + h * (i + 0.5));
        }
        MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        sq[k] *= h;
        if (n > n0) {
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
        }
    }
    t = MPI_Wtime() - t;

    double tmax;
    MPI_Reduce(&t, &tmax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Result Pi: %.12f; Runge rule: EPS %e, n %d, time %lf, commsize "
               "%d\n",
               sq[k ^ 1], eps, n / 2, tmax, commsize);
        
        FILE * pFile;
        pFile = fopen ("runge.dat","a");
        fprintf (pFile, "%d\t%lf\n", commsize, tmax);
        fclose (pFile);       
    }

    MPI_Finalize();
    
}
