#include <math.h>
#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

double randomSmall(bool neg) {
	if(neg) {
		bool b = rand() & 1;
		if(b) {
			return -((double) rand()) / RAND_MAX;
		}
	}
	return ((double) rand()) / RAND_MAX;
}

int randomInt(int max) {
	int num = rand() % max;
	return num;
}

bool randomBool(double t, double f) {
	double num = randomSmall(false);
	if(num <= t) {
		return true;
	}
	return false;
}

int **getBox(int **map, int x0, int y0, int boxSize) {
	int **box = malloc(sizeof(int *) * boxSize);
	for(int i = 0; i < boxSize; ++i) {
		int *arr = malloc(sizeof(int *) * boxSize);
		#pragma omp parallel for
		for(int j = 0; j < boxSize; ++j) {
			arr[j] = map[i + x0][j + y0];
		}
		box[i] = arr;
	}
	return box;
}

void freeBox(int **box, int size) {
	#pragma omp parallel for
	for(int i = 0; i < size; ++i) {
		free(box[i]);
	}
	free(box);
}

double **convOps(int **box, double **weight, int boxSize) {
	double **outputBox = malloc(sizeof(double *) * boxSize);
	for(int i = 0; i < boxSize; ++i) {
		double *arr = malloc(sizeof(double) * boxSize);
		#pragma omp parallel for
		for(int j = 0; j < boxSize; ++j) {
			arr[j] = box[i][j] * weight[i][j];
		}
		outputBox[i] = arr;
	}
	return outputBox;
}

double convOpsSum(int **box, double **weight, int boxSize) {
	double sum = 0;
	for(int i = 0; i < boxSize; ++i) {
		#pragma omp parallel for
		for(int j = 0; j < boxSize; ++j) {
			sum += weight[i][j] * box[i][j];
		}
	}
	return sum;
}