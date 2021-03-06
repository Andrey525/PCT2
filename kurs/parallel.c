#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int *array, int index_left, int index_right) {
    int temp;
    temp = array[index_left];
    array[index_left] = array[index_right];
    array[index_right] = temp;
}

int sort_relative_to_pivot(int *array, int index_left, int index_right) {
    int index_pivot = (index_left + index_right) / 2;
    while (index_right > index_left) {
        if (array[index_left] >= array[index_pivot] &&
            array[index_right] <= array[index_pivot]) {
            swap(array, index_left, index_right);
            if (index_left == index_pivot) {
                index_pivot = index_right;
                index_left++;
            } else if (index_right == index_pivot) {
                index_pivot = index_left;
                index_right--;
            } else {
                index_left++;
                index_right--;
            }
        } else {
            if (array[index_left] < array[index_pivot]) {
                index_left++;
            }
            if (array[index_right] > array[index_pivot]) {
                index_right--;
            }
        }
    }
    return index_pivot;
}

void quicksort(int *array, int index_left, int index_right) {

    if (index_right == index_left) {
        return;
    }
    int index_pivot = sort_relative_to_pivot(array, index_left, index_right);

    if (index_left != index_pivot) {
        quicksort(array, index_left, index_pivot - 1);
    }
    if (index_right != index_pivot) {
        quicksort(array, index_pivot + 1, index_right);
    }
}

int *merge(int *array1, int size1, int *array2, int size2) {
    int *result = malloc((size1 + size2) * sizeof(int));
    int i = 0;
    int j = 0;
    int k;
    for (k = 0; k < size1 + size2; k++) {
        if (i >= size1) {
            result[k] = array2[j];
            j++;
        } else if (j >= size2) {
            result[k] = array1[i];
            i++;
        } else if (array1[i] < array2[j]) {
            result[k] = array1[i];
            i++;
        } else {
            result[k] = array2[j];
            j++;
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    int commsize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (argc == 3) {
        int capacity = 1000000;
        int size = 0;
        int *array;
        int *local_array;
        int local_size;
        double t_read, t, t_merge, t_quicksort, t_exchange;
        FILE *fp;
        if (rank == 0) {
            fp = fopen(argv[1], "r");
            if (!fp) {
                printf("File open failed!\n");
                printf("Press enter to continue.\n");
                getchar();
                exit(-1);
            }
            t_read = MPI_Wtime();
            array = malloc(sizeof(int) * capacity);

            for (int i = 0; !feof(fp); i++) {
                if (size == capacity) {
                    capacity = capacity * 2;
                    array = realloc(array, sizeof(int) * capacity);
                }
                fscanf(fp, "%d", &array[i]);
                size++;
            }
            t_read = MPI_Wtime() - t_read;
            fclose(fp);
            printf("t_read = %lf\n", t_read);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        t = MPI_Wtime();
        t_exchange = MPI_Wtime();
        MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        local_size = (rank != commsize - 1)
                         ? (size / commsize)
                         : (size - ((size / commsize) * (commsize - 1)));
        local_array = calloc(local_size, sizeof(int));
        int *displs = malloc(sizeof(int) * commsize);
        int *scounts = malloc(sizeof(int) * commsize);
        for (int i = 0; i < commsize; i++) {
            scounts[i] = (i != commsize - 1)
                             ? (size / commsize)
                             : (size - ((size / commsize) * (commsize - 1)));
            displs[i] = (i > 0) ? displs[i - 1] + scounts[i - 1] : 0;
        }
        MPI_Scatterv(array, scounts, displs, MPI_INT, local_array, local_size,
                     MPI_INT, 0, MPI_COMM_WORLD);
        t_exchange = MPI_Wtime() - t_exchange;
        if (rank == 0) {
            free(array);
            array = NULL;
        }
        t_quicksort = MPI_Wtime();
        quicksort(local_array, 0, local_size - 1);
        t_quicksort = MPI_Wtime() - t_quicksort;
        t_merge = MPI_Wtime();
        int recv_size;
        int *recv_array;
        for (int step = 1; step < commsize; step = 2 * step) {
            if (rank % (2 * step) != 0) {
                MPI_Send(&local_size, 1, MPI_INT, rank - step, 0,
                         MPI_COMM_WORLD);
                MPI_Send(local_array, local_size, MPI_INT, rank - step, 0,
                         MPI_COMM_WORLD);
                break;
            }
            if (rank + step < commsize) {
                MPI_Recv(&recv_size, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
                recv_array = malloc(recv_size * sizeof(int));
                MPI_Recv(recv_array, recv_size, MPI_INT, rank + step, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                array = merge(local_array, local_size, recv_array, recv_size);
                free(local_array);
                free(recv_array);
                local_array = array;
                local_size = local_size + recv_size;
            }
        }
        t_merge = MPI_Wtime() - t_merge;
        t = MPI_Wtime() - t;
        double tmax, t_merge_max, t_quicksort_max, t_exchange_max;
        MPI_Reduce(&t, &tmax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&t_merge, &t_merge_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&t_quicksort, &t_quicksort_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&t_exchange, &t_exchange_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            printf("\nt_exchange_max = %lf\n", t_exchange_max);
            printf("\nt_quicksort_max = %lf\n", t_quicksort_max);
            printf("\nt_merge_max = %lf\n", t_merge_max);
            printf("\nt_quicksort_max + t_merge_max = %lf\n", t_quicksort_max + t_merge_max);
            printf("\ntmax = %lf\n", tmax);
            fp = fopen(argv[2], "w");
            if (!fp) {
                printf("File open failed!\n");
                printf("Press enter to continue.");
                getchar();
                return 0;
            }
            for (int i = 0; i < local_size; i++) {
                if (i < local_size - 1) {
                    fprintf(fp, "%d\n", local_array[i]);
                } else {
                    fprintf(fp, "%d", local_array[i]);
                }
            }
            fclose(fp);
        }
        free(local_array);
    } else {
        if (rank == 0) {
            printf("Use: mpiexec ./parallel [Name of source file] [Name of "
                   "result file]\n");
        }
    }
    MPI_Finalize();
    return 0;
}