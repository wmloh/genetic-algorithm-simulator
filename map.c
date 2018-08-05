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
};

typedef struct mapStruct *Map;

Map initMap(int size, Mouse m, int mouseX, int mouseY, int cheeseX, int cheeseY) {
	Map map = malloc(sizeof(struct mapStruct));
	int **arr = malloc(sizeof(int *) * size);
	for(int i = 0; i < size; ++i) {
		arr[i] = calloc(size, sizeof(int));
	}
	map->arr = arr;
	(map->arr)[mouseY][mouseX] = MOUSE;
	(map->arr)[cheeseY][cheeseX] = CHEESE;
	map->size = size;
	map->m = m;
	map->mousePos = malloc(sizeof(int) * 2);
	map->cheesePos = malloc(sizeof(int) * 2);
	(map->mousePos)[0] = mouseX;
	(map->mousePos)[1] = mouseY;
	(map->cheesePos)[0] = cheeseX;
	(map->cheesePos)[1] = cheeseY;
	return map;
}

void applyMap(char *fileName, Map map, int size) {
	FILE *fp = fopen(fileName, "r");
	char buffer[size + 2];
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