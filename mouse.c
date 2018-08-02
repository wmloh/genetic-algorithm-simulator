#include <stdlib.h>
#include <string.h>
#include "convolution.c"

const int primaryInput = 8;
const double majorMutation = 0.01;

struct mouse {
	int gen;
	double *primaryW;
	double **peripheralW;
	int visionRange;
	double alpha; // significance of peripheral vision
};

typedef struct mouse *Mouse;

Mouse initGenesis(void) {
	Mouse m = malloc(sizeof(struct mouse));
	m->gen = 1;
	m->primaryW = malloc(sizeof(double) * primaryInput);
	m->peripheralW = malloc(sizeof(double *) * 3);
	m->visionRange = 1;
	m->alpha = 0.1;
	#pragma omp parallel for
	for(int i = 0; i < primaryInput; ++i) {
		(m->primaryW)[i] = randomSmall(true);
	}
	double *arr;
	for(int i = 0; i < 3; ++i) {
		arr = malloc(sizeof(double) * 3);
		for(int j = 0; j < 3; ++j) {
			arr[j] = 0;
		}
		(m->peripheralW)[i] = arr;
	}
	return m;
}

Mouse procreate(Mouse mParent) {
	Mouse mChild = malloc(sizeof(struct mouse));
	mChild->gen = mParent->gen + 1;
	mChild->primaryW = malloc(sizeof(double) * primaryInput);

	#pragma omp parallel for
	for(int i = 0; i < primaryInput; ++i) {
		(mChild->primaryW)[i] = (mParent->primaryW)[i] + randomSmall(true);
	}

	int units;
	//if(randomBool(majorMutation, ((double) 1) - majorMutation)) {
	if(true) {
		mChild->visionRange = mParent->visionRange + 1;
		int len = mChild->visionRange;
		int origLen = len - 1;

		units = 2 * len + 1;
		int origUnits = 2 * origLen + 1;
		mChild->peripheralW = malloc(sizeof(double *) * units);

		double *arr;
		arr = malloc(sizeof(double) * units);
		for(int i = 0; i < units; ++i) {
			arr[i] = randomSmall(true);
		}
		(mChild->peripheralW)[0] = arr;

		for(int i = 0; i < origUnits; ++i) {
			arr = malloc(sizeof(double) * units);
			arr[0] = randomSmall(true);
			for(int j = 0; j < origUnits; ++j) {
				arr[j + 1] = (mParent->peripheralW)[i][j];
			}
			arr[units-1] = randomSmall(true);
			(mChild->peripheralW)[i+1] = arr;
		}

		arr = malloc(sizeof(double) * units);
		for(int i = 0; i < units; ++i) {
			arr[i] = randomSmall(true);
		}
		(mChild->peripheralW)[units-1] = arr;

	} else {
		mChild->visionRange = mParent->visionRange;
		int len = mChild->visionRange;
		units = 2 * len + 1;
		
		mChild->peripheralW = malloc(sizeof(double *) * units);
		if(mChild->visionRange != 1) {
			for(int i = 0; i < len; ++i) {
				#pragma omp parallel for
				for(int j = 0; j < len; ++j) {
					(mChild->peripheralW)[i][j] = (mParent->peripheralW)[i][j] + randomSmall(true);
				}
			}
		} else {
			double *arr;
			for(int i = 0; i < 3; ++i) {
				arr = malloc(sizeof(double) * 3);
				for(int j = 0; j < 3; ++j) {
					arr[j] = 0;
				}
				(mChild->peripheralW)[i] = arr;
			}
		}
	}

	mChild->alpha = mParent->alpha + randomSmall(true) / 10;
	return mChild;
}

void saveMouse(Mouse m, char *fileName) {
	char src[25];
	strcpy(src, fileName);
	FILE *fp = fopen(strcat(src, ".txt"), "w");
	fprintf(fp, ">> gen\r\n");
	fprintf(fp, " %i\r\n", m->gen);
	fprintf(fp, "-----\r\n");

	fprintf(fp, ">> visionRange\r\n");
	fprintf(fp, " %i\r\n", m->visionRange);
	fprintf(fp, "-----\r\n");

	fprintf(fp, ">> alpha\r\n");
	fprintf(fp, " %f\r\n", m->alpha);
	fprintf(fp, "-----\r\n");

	fprintf(fp, ">> primaryW\r\n");
	for(int i = 0; i < primaryInput; ++i) {
		fprintf(fp, " %f\r\n", (m->primaryW)[i]);
	}
	fprintf(fp, "-----\r\n");

	fprintf(fp, ">> peripheralW\r\n");
	int len = 2 * m->visionRange + 1;
	fprintf(fp, " %i\r\n", len);
	for(int i = 0; i < len; ++i) {
		fprintf(fp, "*index:%i\r\n", i);
		for(int j = 0; j < len; ++j) {
			fprintf(fp, " %f\r\n", (m->peripheralW)[i][j]);
		}
	}
	fprintf(fp, "-----\r\nE");
	fclose(fp);
}

Mouse loadMouse(char *fileName) {
	char src[25];
	strcpy(src, fileName);
	FILE *fp = fopen(strcat(src, ".txt"), "r");

	char buffer[50];
	char c = fgetc(fp);
	int count = 0;

	Mouse m = malloc(sizeof(struct mouse));
	m->primaryW = malloc(sizeof(double) * primaryInput);
	int pcount = 0;

	while(c != 'E') {
		if(c == ' ') {
			char *s = fgets(buffer, 50, fp);
			char *pos;
			if((pos=strchr(s, '\n'))) *pos = '\0';

			if(count == 1) {
				int val = atoi(s);
				m->gen = val;
			} else if (count == 2) {
				int val = atoi(s);
				m->visionRange = val;
			} else if (count == 3) {
				double val;
				sscanf(s, "%lf", &val);
				m->alpha = val;
			} else if (count == 4) {
				double val;
				sscanf(s, "%lf", &val);
				(m->primaryW)[pcount] = val;
				++pcount;
			} else if (count == 5) {
				int len = atoi(s);
				m->peripheralW = malloc(sizeof(double *) * len);
				double val;
				for(int i = 0; i < len; ++i) {
					double *arr = malloc(sizeof(double) * len);
					fgets(buffer, 50, fp);
					for(int j = 0; j < len; ++j) {
						fgetc(fp);
						s = fgets(buffer, 50, fp);
						if((pos=strchr(s, '\n'))) *pos = '\0';
						sscanf(s, "%lf", &val);
						arr[j] = val;
					}
					(m->peripheralW)[i] = arr;
				}
			}
			
		} else if (c == '>') {
			count++;
			fgets(buffer, 50, fp);
		} else {
			fgets(buffer, 50, fp);
		} 
		c = fgetc(fp);
	}
	fclose(fp);
	return m;
}