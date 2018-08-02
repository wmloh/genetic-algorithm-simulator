#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void mapPrint(int **map, int size) {

	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < size; ++j) {
			int x = map[i][j];
			if(x == -1) {
				printf("%i",map[i][j]);
			} else {
				printf(" %i",map[i][j]);
			}
			
		}
		printf("\n");
	}
	printf("\n");
	
}

void mapPrintDble(double **map, int size) {
	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < size; ++j) {
			double x = map[i][j];
			if(x == 0) {
				printf(" %f",map[i][j]);
			} else {
				printf("%f",map[i][j]);
			}
			
		}
		printf("\n");
	}
	printf("\n");
}