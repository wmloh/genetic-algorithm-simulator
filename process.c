#include "map.c"

int moveMouse(Map map, int dir) {
	int x = (map->mousePos)[0];
	int y = (map->mousePos)[1];
	if(dir == 0) {
		y--;
	} else if (dir == 1) {
		x++;
	} else if (dir == 2) {
		y++;
	} else {
		x--;
	}

	int val = (map->arr)[y][x];
	if(val == -1) {
		return 1;
	} else if (val == CHEESE) {
		return 2;
	}
	(map->arr)[(map->mousePos)[1]][(map->mousePos)[0]] = 0;
	(map->arr)[y][x] = MOUSE;
	(map->mousePos)[0] = x;
	(map->mousePos)[1] = y;
	return 0;
}

double **decidePrimary(Mouse m, Map map) {
	int **box = getBox(map->arr, (map->mousePos)[0], (map->mousePos)[1], 3);
	box[1][1] = 0;
	double **convBox = convDot(m->primaryW, box, 3);
	return convBox;
}

double **decidePeripheral(Mouse m, Map map) {
	int size = 2 * m->visionRange + 1;
	int **box = getBox(map->arr, (map->mousePos)[0], (map->mousePos)[1], size);
	double **convBox = convDot(m->peripheralW, box, size);
	return convBox;
}

double **decide(Mouse m, Map map) {
	double **primary = decidePrimary(m, map);
	double **peripheral = decidePeripheral(m, map);
	transferPeripheral(peripheral, 2 * m->visionRange + 1, primary, m->alpha);
	transferPrimary(primary);
	return primary;

}

