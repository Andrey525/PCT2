#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

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

int main(int argc, char *argv[]) {
    if (argc == 3) {
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            printf("File open failed!\n");
            printf("Press enter to continue.");
            getchar();
            return 0;
        } else {
            printf("File for reading opened successfully!\n");
        }
        double t_read;
        t_read = wtime();
        int capacity = 10;
        int size = 0;
        int *array = malloc(sizeof(int) * capacity);
        for (int i = 0; !feof(fp); i++) {
            if (i == capacity) {
                capacity = capacity * 2;
                array = realloc(array, sizeof(int) * capacity);
            }
            fscanf(fp, "%d", &array[i]);
            size++;
        }
        fclose(fp);
        t_read = wtime() - t_read;
        printf("t_read = %lf\n", t_read);
        printf("File content read successfully!\n");

        printf("Starting to sort!\n");
        double t;
        t = wtime();
        quicksort(array, 0, size - 1);
        t = wtime() - t;
        printf("Quicksort completed!\n");

        fp = fopen(argv[2], "w");
        if (!fp) {
            printf("File open failed!\n");
            printf("Press enter to continue.");
            getchar();
            return 0;
        } else {
            printf("File for writing opened successfully!\n");
        }
        for (int i = 0; i < size; i++) {
            if (i < size - 1) {
                fprintf(fp, "%d\n", array[i]);
            } else {
                fprintf(fp, "%d", array[i]);
            }
        }
        fclose(fp);
        printf("Sorted content write to %s successfully!\n", argv[2]);
        printf("Time of sorting = %lf\n", t);
        free(array);
    } else {
        printf("Use: ./serial [Name of source file] [Name of result file]\n");
    }

    return 0;
}