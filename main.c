#include "engine.c"

int main() {

	srand(time(NULL));
	
	Mouse m1 = loadMouse("mousetest");
	
	Map map = initMap(10, m1, 5, 5, 8, 8);
	applyMap("map", map, 10);
	padMap(map);
	mapPrint(map);
	
	testPhase(m1, map, 10, true, "testPhaseMap");
}