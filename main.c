#include "engine.c"

int main() {

	srand(time(NULL));
	
	Mouse m1 = loadMouse("mouse2");
	Mouse m2 = procreate(m1);
	Map map = initMap(5, m2, 2, 2, 4, 4);
	applyMap("map.txt", map, 5);
	mapPrint(map);
	
	int **box = getBox(map->arr, 2, 2, 3);
	boxPrint(box, 3);

	double **p1 = decidePrimary(m2, map);
	double **p2 = decidePeripheral(m2, map);
	double **result = decide(m2, map);
	boxPrintDble(p1, 3);
	boxPrintDble(p2, 5);
	boxPrintDble(result, 3);
}