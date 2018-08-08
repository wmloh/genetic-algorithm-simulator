#include "gene.c"

int main() {
	
	srand(time(NULL));
	/*
	int size = 10;
	Mouse m1 = loadMouse("mousetest2", size);
	//Mouse m1 = initGenesis(size);
	Map map = initMap(size, 5, 5, 8, 8);
	applyMap("map", map, size);
	padMap(map);
	mapPrint(map);
	
	testPhase(m1, map, 15, true, "testPhaseMap");
	*/
	
	int numMouse = 5;
	int gen = 3;
	Strain r = initStrain(numMouse, 10, "map", 5, 5, 8, 8);
	Mouse m = initSimulation(r, gen, 5, 5, 10);
	saveMouse(m, "trainedMouse");
	
	/*
	int size = 10;
	Mouse m1 = initGenesis(size);
	Map map = initMap(size, 5, 5, 8, 8);
	applyMap("map", map, size);
	padMap(map);
	mapPrint(map);

	Mouse m2 = procreate(m1, size);
	*/
	
}