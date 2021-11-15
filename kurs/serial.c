#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

void quicksort(int *array, int index_left, int index_right, int index_pivot) {

	if (index_right == index_left) {
		return;
	}

	printf("index_left = %d index_right = %d\n", index_left, index_right);

	int temp;
	int index_left_for_next = index_left;
	int index_right_for_next = index_right;

	while (index_right > index_left) {
		if (array[index_left] >= array[index_pivot] && array[index_right] <= array[index_pivot]) {
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

		for (int i = 0; i < SIZE; i++) {
			printf("%d ", array[i]);
		}
		printf("\n pivot = %d\n", index_pivot);

	}
	printf("index_left_for_next = %d index_right_for_next = %d\n", index_left_for_next, index_right_for_next);
	if (index_left_for_next != index_pivot) {
		quicksort(array, index_left_for_next, index_pivot - 1, (index_left_for_next + index_pivot - 1) / 2);	
	}
	
	if (index_right_for_next != index_pivot) {
		quicksort(array, index_pivot + 1, index_right_for_next, (index_pivot + 1 + index_right_for_next) / 2);	
	}
	
}

int main(int argc, char *argv[]) {

	int array[SIZE];
	for (int i = 0; i < SIZE; i++) {
		array[i] = rand() % 100;
	}
	int index_pivot = 1;
	int index_left = 0;
	int index_right = SIZE - 1;
	
	quicksort(array, index_left, index_right, index_pivot);

	return 0;
}