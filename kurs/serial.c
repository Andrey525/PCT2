#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quicksort(int *array, int index_left, int index_right) {

    if (index_right == index_left) {
        return;
    }

    int temp;
    int index_left_for_next = index_left;
    int index_right_for_next = index_right;
    int index_pivot = (index_left + index_right) / 2;

    while (index_right > index_left) {
        if (array[index_left] >= array[index_pivot] &&
            array[index_right] <= array[index_pivot]) {
            temp = array[index_left];
            array[index_left] = array[index_right];
            array[index_right] = temp;
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

    if (index_left_for_next != index_pivot) {
        quicksort(array, index_left_for_next, index_pivot - 1);
    }
    if (index_right_for_next != index_pivot) {
        quicksort(array, index_pivot + 1, index_right_for_next);
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

        int capacity = 10;
        int i, size;
        int *array = malloc(sizeof(int) * capacity);
        for (i = 0; !feof(fp); i++) {
            if (i == capacity) {
                capacity = capacity * 2;
                array = realloc(array, sizeof(int) * capacity);
            }
            fscanf(fp, "%d", &array[i]);
        }
        fclose(fp);
        printf("File content read successfully!\n");
        size = i - 1;

        printf("Starting to sort!\n");
        quicksort(array, 0, size - 1);
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
        for (i = 0; i < size; i++) {
            fprintf(fp, "%d\n", array[i]);
        }
        fclose(fp);
        printf("Sorted content write to %s successfully!\n", argv[2]);

        free(array);
    } else {
        printf("Use: ./serial [Name of source file] [Name of result file]\n");
    }

    return 0;
}