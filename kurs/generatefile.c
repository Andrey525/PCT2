#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 *	First argument - Name of file
 *  Second argument - Size
 */

int main(int argc, char *argv[]) {
    if (argc == 3) {
        FILE *fp = fopen(argv[1], "w");
        if (!fp) {
            printf("File open failed!\n");
            printf("Press enter to continue.");
            getchar();
            return 0;
        } else {
            printf("File for generating random numbers opened successfully!\n");
        }
        int size = atoi(argv[2]);
        srand(time(NULL));
        for (int i = 0; i < size; i++) {
            fprintf(fp, "%d\n", rand() % 10000);
        }
        fclose(fp);
        printf("Generation competed!\n");
    } else {
        printf("Use, for example: ./generatefile [Name of file] 100\n");
        return 1;
    }

    return 0;
}