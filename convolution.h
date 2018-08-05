

double randomSmall(bool neg);

int randomInt(int max);

bool randomBool(double t, double f);

int **getBox(int **map, int x0, int y0, int boxSize);

void freeBox(int **box, int size);

double **convOps(int **box, double **weight, int boxSize);

double convOpsSum(int **box, double **weight, int boxSize);