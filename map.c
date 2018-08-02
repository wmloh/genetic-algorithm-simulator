#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int **initMap(int size) {
	int **map = malloc(sizeof(int *) * size);
	for(int i = 0; i < size; ++i) {
		map[i] = calloc(size, sizeof(int));
	}
	return map;
}

void applyMap(char *fileName, int **map, int size) {
	FILE *fp = fopen(fileName, "r");
	for(int i = 0; i < size; ++i) {
		char buffer[size + 2];
		char *s = fgets(buffer, 20, fp);
		#pragma omp parallel for
		for(int j = 0; j < size; ++j) {
			char c = s[j];
			if(c == '1') {
				map[i][j] = -1;
			} else {
				map[i][j] = ((int) c) - 48;
			}
		}
	}
	fclose(fp);
}

void padMap(int **map, int size) {
	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < size; ++j) {
			if(i == 0 || i == size - 1 || j == 0 || j == size - 1) {
				map[i][j] = -1;
			}
		}
	}
}
/*
int main() {
	int size = 8;
	int **map = initMap(size);
	padMap(map, size);
	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < size; ++j) {
			int x = map[i][j];
			if(x == 0) {
				printf(" %i",map[i][j]);
			} else {
				printf("%i",map[i][j]);
			}
			
		}
		printf("\n");
	}

}
*/