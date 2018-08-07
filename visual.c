#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.c"
#include <unistd.h>

void boxPrint(int **box, int size) {
	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < size; ++j) {
			int x = box[i][j];
			if(x < 0) {
				printf("%i", x);
			} else {
				printf(" %i", x);
			}

		}
		printf("\n");
	}
	printf("\n");
}

void boxPrintDble(double **box, int size) {
	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < size; ++j) {
			double x = box[i][j];
			if(x < 0) {
				printf("%f", x);
			} else {
				printf(" %f", x);
			}
		}
		printf("\n");
	}
	printf("\n");
}

void mapPrint(Map map) {
	boxPrint(map->arr, map->size);
}

void rowPrint(double *row, int size) {
	for(int i = 0; i < size; ++i) {
		double x = row[i];
		if(x < 0) {
			printf("%f", x);
		} else {
			printf(" %f", x);
		}
	}
	printf("\n");
}