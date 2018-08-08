#include "engine.c"

struct strain {
	int numMouse;
	Map map;
	Mouse *mice;
	double *scores;
};

typedef struct strain *Strain;

Strain initStrain(int numMouse, int mapSize, char *mapFileName, int mouseX, int mouseY, int cheeseX, int cheeseY) {
	Strain s = malloc(sizeof(struct strain));
	s->numMouse = numMouse;
	Map map = initMap(mapSize, mouseX, mouseY, cheeseX, cheeseY);
	applyMap(mapFileName, map, mapSize);
	padMap(map);
	s->map = map;
	s->scores = malloc(numMouse * sizeof(double));
	s->mice = malloc(numMouse * sizeof(Mouse));
	for(int i = 0; i < numMouse; ++i) {
		(s->mice)[i] = initGenesis(mapSize);
		(s->scores)[i] = 0;
	}
	return s;
}

void swap(double *ele1, double *ele2) {
	double temp = *ele2;
	*ele2 = *ele1;
	*ele1 = temp;
}

void swapMouse(Mouse *m1, Mouse *m2) {
	Mouse temp = *m2;
	*m2 = *m1;
	*m1 = temp;
}

void quickSortRange(Mouse *mice, double *scores, int first, int last) {
	if(last <= first) return;

	double pivot = scores[first];
	int pos = last;

	for(int i = last; i > first; --i) {
		if(scores[i] < pivot) {
			swap(&scores[pos], &scores[i]);
			swapMouse(&mice[pos], &mice[i]);
			--pos;
		}
	}
	swap(&scores[first], &scores[pos]);
	swapMouse(&mice[first], &mice[pos]);
	quickSortRange(mice, scores, first, pos-1);
	quickSortRange(mice, scores, pos+1, last);
}

void twinQuickSort(Mouse *mice, double *scores, int len) {
	quickSortRange(mice, scores, 0, len-1);
}

void naturalSelect(Mouse *m, int size, int deg, int upperBound) {
	int s;
	double generatedP;
	for(int i = 0; i < size; ++i) {
		generatedP = (pow(2, pow(((double) i) / upperBound, deg)) - 1) * 100;
		s = rand() % 100;

		if(s <= generatedP) {
			exterminate(m[i]);
			m[i] = NULL;
		}
	}
}

Mouse initSimulation(Strain s, int gen, int threshold, int thresIncrement, int thresInterval) {
	int numMouse = s->numMouse;
	for(int i = 0; i < gen; ++i) {

		initPhase(s->mice, numMouse, s->map, threshold, s->scores);
		twinQuickSort(s->mice, s->scores, numMouse);
		
		if(i != gen - 1) {
			naturalSelect(s->mice, numMouse, 4, numMouse-1);
			int index = 0;
			
			for(int j = numMouse - 1; j >= 0; --j) {
				if(!(s->mice)[j]) {
					(s->mice)[j] = procreate((s->mice)[index], s->map->size);

					index++;
					while(!(s->mice)[index]) {
						index++;
					}
				}
			}
		}
		if(i % thresInterval == 0 && i != 0) {
			threshold += thresIncrement;
		}		
	}
	return (s->mice)[0];
}

void freeStrain(Strain s) {
	int len = s->numMouse;
	for(int i = 0; i < len; ++i) {
		exterminate((s->mice)[i]);
	}
	free(s->mice);
	freeMap(s->map);
	free(s->scores);
	free(s);
}