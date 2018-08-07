#include "visual.c"

double distMetric(Map map) {
	int x0 = (map->mousePos)[0];
	int y0 = (map->mousePos)[1];
	int x1 = (map->cheesePos)[0];
	int y1 = (map->cheesePos)[1];

	return sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
}

void initPhase(Mouse *mArray, int num, int gen) {
	
}

void testPhase(Mouse m, Map map, int threshold, bool print, char *fileName) {

	int score = 0;
	double dist = 0;
	char src[25];
	if(fileName) {
		
		strcpy(src, fileName);
		FILE *fp = fopen(strcat(src, ".txt"), "w");
		fclose(fp);
	}

	while(true) {
		if(print) {
			mapPrint(map);
			dist = distMetric(map);
			printf("Distance: %lf\n", dist);
		}
		if (fileName) {
			saveMap(map, fileName);
		}
		int move = decide(m, map);
		int result = moveMouse(m, map, move);
		if(result == 1) {
			printf("Collision\n");
			break;
		} else if (result == 2) {
			printf("Found cheese!\n");
			break;
		}
		score++;
		
		
		if(score >= threshold) {
			printf("Exceeded threshold\n");
			break;
		}
	}
	

	FILE *fp = fopen(src, "a");
	fprintf(fp, "E");
	fclose(fp);
	
	return;
}