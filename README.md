# Genetic Algorithm: Mouse and Cheese
## Dependencies:
* Cygwin
## Description:

### Mouse
The mouse's main objective is to locate and go to the location of the cheese, which grants it high reward and ends the simulation.
The genesis mouse (first mouse during initialization) will only have a vision range of 1. With the vision as inputs to the mouse's
decision process-making, it will choose to move up, down, left or right. If it collides with an obstacle, it will die.

To simulate a genetic algorithm that converges to an optimum, mice can reproduce using the function ```procreate``` where resultant
mice contains traces of attributes (mathematical parameters) as their parent.

### Map
The map is essentially a multidimensional array, which resembles a square map. Using the ```applyMap``` function, users/developers
can manually configure the map on a .txt file to define obstacles and cheese.

### Engine
