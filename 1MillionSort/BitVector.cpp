#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define SIZE (10000001) /* amount of bits */
#define ARRAY_SIZE(x) (x/8+(!!(x%8)))

char get_bit(char *array, int index);
void toggle_bit(char *array, int index);
void toggle_bit(char *array, int index) {
	array[index / 8] ^= 1 << (index % 8);
}

char get_bit(char *array, int index) {
	return 1 & (array[index / 8] >> (index % 8));
}

/*int main() {
	char bitvector[ARRAY_SIZE(SIZE)] = { 0 };

	FILE *file = fopen("input.txt", "r");

	int val;
	while (fscanf(file, "%d", &val) != EOF) {
		toggle_bit(bitvector, val);
	}

	for (int i = 1; i < SIZE; i++) {
		if (get_bit(bitvector, i))
			printf("%d ", i);
	}

	return 0;
}*/