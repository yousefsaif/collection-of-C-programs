#ifndef LOGIC_H_
#define LOGIC_H_

#include <stdbool.h>


//get the number of *Live* neighbours for a particular cell
int numNeighbours(char **grid, int row, int col, int rows, int cols);

//evolve the game by number of evolutions (displaying after each evolution if 'show' is true)
void evolve(char **grid, int nrows, int ncols, int evolutions, bool show);

//Display the grid that thats passed in, Printing state of game of life
void display(char **grid, int nrows, int ncols);

// allocate the array
void allocateGrid(char ***grid, int nrows, int ncols);

//Populate the grid with live/dead cells randomly
void populateGrid(char **grid, int nrows, int ncols);


#endif