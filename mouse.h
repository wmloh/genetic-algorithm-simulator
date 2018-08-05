struct mouse;

typedef struct mouse *Mouse;

Mouse initGenesis(void);

Mouse procreate(Mouse mParent);

void saveMouse(Mouse m, char *fileName);

Mouse loadMouse(char *fileName);

void exterminate(Mouse m);