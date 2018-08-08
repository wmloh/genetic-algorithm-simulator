#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "mouse.c"

#define CHEESE 9
#define MOUSE 5

struct mapStruct {
	int **arr;
	int size;
	Mouse m;
	int *mousePos;
	int *cheesePos;
	const int * const resetPos;
};

typedef struct mapStruct *Map;

Map initMap(int size, int mouseX, int mouseY, int cheeseX, int cheeseY) {
	Map map = malloc(sizeof(struct mapStruct));
	map->size = size;
	int **arr = malloc(sizeof(int *) * size);
	for(int i = 0; i < size; ++i) {
		arr[i] = calloc(size, sizeof(int));
	}
	map->arr = arr;
	(map->arr)[mouseY][mouseX] = MOUSE;
	(map->arr)[cheeseY][cheeseX] = CHEESE;

	map->mousePos = malloc(sizeof(int) * 2);
	map->cheesePos = malloc(sizeof(int) * 2);
	*(int **)&(map->resetPos) = malloc(sizeof(int) * 2);
	(map->mousePos)[0] = mouseX;
	(map->mousePos)[1] = mouseY;
	(map->cheesePos)[0] = cheeseX;
	(map->cheesePos)[1] = cheeseY;
	
	*(int *)&(map->resetPos)[0] = mouseX;
	*(int *)&(map->resetPos)[1] = mouseY;
	
	return map;
}

void newMap(char *fileName, int size) {
	char src[25];
	strcpy(src, fileName);
	FILE *fp = fopen(strcat(src, ".txt"), "w");

	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < size; ++j) {
			fprintf(fp, "0");
		}
		fprintf(fp, "\r\n");
	}
	fclose(fp);
}

void resetMap(Map map) {
	(map->arr)[(map->mousePos)[1]][(map->mousePos)[0]] = 0;
	(map->arr)[(map->resetPos)[1]][(map->resetPos)[0]] = MOUSE;
	(map->mousePos)[0] = map->resetPos[0];
	(map->mousePos)[1] = map->resetPos[1];
}

void applyMap(char *fileName, Map map, int size) {
	char src[25];
	strcpy(src, fileName);
	
	FILE *fp = fopen(strcat(src, ".txt"), "r");
	char buffer[size + 3];
	for(int i = 0; i < size; ++i) {
		
		char *s = fgets(buffer, size+3, fp);
		char c;
		#pragma omp parallel for
		for(int j = 0; j < size; ++j) {
			c = s[j];
			if(c == '1') {
				(map->arr)[i][j] = -1;
			} else {
				(map->arr)[i][j] = ((int) c) - 48;
			}
		}
	}
	(map->arr)[(map->mousePos)[1]][(map->mousePos)[0]] = MOUSE;
	(map->arr)[(map->cheesePos)[1]][(map->cheesePos)[0]] = CHEESE;
	fclose(fp);
}

void saveMap(Map map, char *fileName) {
	char src[25];
	strcpy(src, fileName);
	FILE *fp = fopen(strcat(src, ".txt"), "a");

	int size = map->size;

	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < size; ++j) {
			int x = (map->arr)[i][j];
			if(x >= 0) {
				fprintf(fp, " %i", x);
			} else {
				fprintf(fp, " %i", -x);
			}
			
		}
		fprintf(fp, "\r\n");
	}
	fprintf(fp, "\r\n");
	fclose(fp);
}

void padMap(Map map) {
	int size = map->size;
	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < size; ++j) {
			if(i == 0 || i == size - 1 || j == 0 || j == size - 1) {
				(map->arr)[i][j] = -1;
			}
		}
	}
}



void freeMap(Map map) {
	free(map->mousePos);
	free(map->cheesePos);
	int len = map->size;
	for(int i = 0; i < len; ++i) {
		free((map->arr)[i]);
	}
	free(map->arr);
	free(map);
}