#include "map.c"
#include "visual.c"
#include "mouse.c"

int main() {

	Mouse m = initGenesis();
	Mouse m2 = procreate(m);
	saveMouse(m2, "mouse2");
	Mouse m3 = loadMouse("mouse2");
	saveMouse(m3, "mouse3");

}