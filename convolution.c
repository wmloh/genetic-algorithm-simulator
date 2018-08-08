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

int randomChoice(double p1, double p2, double p3, double p4) {
	double num = ((double) rand()) / RAND_MAX;
	if(num <= p1) {
		return 0;
	} else if (num <= p1 + p2) {
		return 1;
	} else if (num <= p1 + p2 + p3) {
		return 2;
	}
	return 3;
}

int max(double *arr, int size, double *val) {
	int x = 0;
	double maximum = arr[0];
	for(int i = 1; i < size; ++i) {
		if(arr[i] > maximum) {
			maximum = arr[i];
			x = i;
		}
	}
	if(val) {
		*val = maximum;
	}
	return x;
}

int **getBox(int **map, int x, int y, int boxSize) {
	int offset = (boxSize - 1) / 2;
	int x0 = x - offset;
	int y0 = y - offset;
	int **box = malloc(sizeof(int *) * boxSize);
	for(int i = 0; i < boxSize; ++i) {
		int *arr = malloc(sizeof(int) * boxSize);
		#pragma omp parallel for
		for(int j = 0; j < boxSize; ++j) {
			arr[j] = map[i + y0][j + x0];
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

void freeBoxDble(double **box, int size) {
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

double **convAdd(double **box1, double **box2, int size) {
	double **output = malloc(sizeof(double *) * size);
	for(int i = 0; i < size; ++i) {
		output[i] = malloc(sizeof(double) * size);
		for(int j = 0; j < size; ++j) {
			output[i][j] = box1[i][j] + box2[i][j];
		}
	}
	return output;
}

double **convDot(double **box1, int **box2, int size) {
	double **output = malloc(sizeof(double *) * size);
	for(int i = 0; i < size; ++i) {
		output[i] = malloc(sizeof(double ) * size);
		for(int j = 0; j < size; ++j) {
			output[i][j] = box1[i][j] * box2[i][j];
		}
	}
	return output;
}

double **convScalar(int **box, double c, int size) {
	double **output = malloc(sizeof(double *) * size);
	for(int i = 0; i < size; ++i) {
		output[i] = malloc(sizeof(double) * size);
		for(int j = 0; j < size; ++j) {
			output[i][j] = box[i][j] * c;
		}
	}
	return output;
}

void softmax(double *box) {
	double sum = 0;
	#pragma omp parallel for
	for(int i = 0; i < 4; ++i) {
		box[i] = exp(box[i]);
		sum += box[i];
	}

	#pragma omp parallel for
	for(int i = 0; i < 4; ++i) {
		box[i] /= sum;
	}
}

void transferPrimary(double **box, double beta) {
	box[0][1] += beta * (box[0][0] + box[0][2]);
	box[1][0] += beta * (box[0][0] + box[2][0]);
	box[2][1] += beta * (box[2][0] + box[2][2]);
	box[1][2] += beta * (box[0][2] + box[2][2]);
	box[0][0] = 0;
	box[0][2] = 0;
	box[2][0] = 0;
	box[2][2] = 0;
}

void transferPeripheral(double **peri, int size, double **primary, double alpha) {
	int center = (size - 1) / 2;
	int x0 = center - 1;
	int x1 = center + 1;
	for(int i = 0; i < size; ++i) {
		#pragma omp parallel for
		for(int j = 0; j < size; ++j) {
			if (i == j && j < x0) {
				primary[0][0] += alpha * peri[j][i];
			} else if (i + j == size - 1 && j < x0) {
				primary[2][0] += alpha * peri[j][i];
			} else if (i == j && j > x1) {
				primary[2][2] += alpha * peri[j][i];
			} else if (i + j == size - 1 && j > x1) {
				primary[0][2] += alpha * peri[j][i];
			} else if (j > i && i + j < size - 1 && i < x0) {
				primary[0][1] += alpha * peri[j][i];
			} else if (i > j && i + j > size - 1 && i > x1) {
				primary[2][1] += alpha * peri[j][i];
			} else if (j > i && i + j > size - 1 && j > x1) {
				primary[1][2] += alpha * peri[j][i];
			} else if (i > j && i + j < size - 1 && j < x0) {
				primary[1][0] += alpha * peri[j][i];
			}
		}
	}
}

