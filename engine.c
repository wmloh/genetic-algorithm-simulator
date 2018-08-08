#include "visual.c"

#define DISTFACTOR 0.9

double distMetric(Map map) {
	int x0 = (map->mousePos)[0];
	int y0 = (map->mousePos)[1];
	int x1 = (map->cheesePos)[0];
	int y1 = (map->cheesePos)[1];

	return sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
}

double scoreMetric(int steps, double dist) {
	double distScore = pow(DISTFACTOR, dist);
	return (2 * steps * distScore) / (steps + distScore);
}

bool initPhase(Mouse *mArray, int num, Map map, int threshold, double *scores) {
	int steps;
	double dist;
	Mouse m;
	bool gotCheese = false;

	for(int i = 0; i < num; ++i) {
		m = mArray[i];
		steps = 0;
		dist = 0;
		int moveOutcome;
		while(true) {
			moveOutcome = moveMouse(m, map, decide(m, map));
			if(moveOutcome == 1) {
				break;
			} else if (moveOutcome == 2) {
				gotCheese = true;
				break;
			}
			steps++;
			if(steps >= threshold) {
				break;
			}
		}
		
		if(moveOutcome != 2) {
			dist = distMetric(map);	
		}
		scores[i] = scoreMetric(steps, dist);
		resetMap(map);
		refreshMemory(m, map->size);
	}

	return gotCheese;
}

double testPhase(Mouse m, Map map, int threshold, bool print, char *fileName) {

	int steps = 0;
	double dist = 0;
	char src[25];
	if(fileName) {
		strcpy(src, fileName);
		FILE *fp = fopen(strcat(src, ".txt"), "w");
		fclose(fp);
	}

	while(true) {
		dist = distMetric(map);
		if(print) {
			mapPrint(map);
			printf("Distance: %f\n", dist);
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
			dist = 0;
			printf("Found cheese!\n");
			break;
		}
		
		steps++;
		
		
		if(steps >= threshold) {
			printf("Exceeded threshold\n");
			break;
		}
	}
	
	double score = scoreMetric(steps, dist);
	if(print) {
		printf("Score: %f\n", score);	
	}
	return score;
}