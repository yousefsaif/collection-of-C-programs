#include "logic.h"
#include <stdio.h>
#include <stdlib.h>
#include "time.h"


int numNeighbours(char **grid, int row, int col, int nrows, int ncols){
    int n = 0;
 
    //count number of live neighbours above(if not on top row)
    if(row>0){ 
        if(col>0){ 
            n += grid[row-1][col-1]; //diagnol up left
        }

        n += grid[row-1][col]; //directly up

        if(col<ncols-1){ 
            n += grid[row-1][col+1]; // diagbnol up right
        }
    }
  
    //count number of live cells on left (if not furthest left column)
    if(col>0){
        n += grid[row][col-1]; //left
    }
    //count number of live cells on right (if not furthest right column)
    if(col < ncols-1){
        n += grid[row][col+1]; //right
    }

    //count number of live cells below (if not on bottom row)
    if(row<nrows-1){
        if(col>0){
            n += grid[row+1][col-1]; //diagnol down left
        }

        n += grid[row+1][col];   // directly down

        if(col<ncols-1){
            n += grid[row+1][col+1]; // diagnol down right
        }
    }

    return n;
}

void evolve(char **grid, int nrows, int ncols, int evolutions, bool show){
    //create and allocate for gridCopy
    char **gridCopy;
    allocateGrid(&gridCopy, nrows, ncols);

    //handle all evolutions
    for(int e = 0; e<evolutions; e++){
        //set gridCopy to be same as grid
        for(int i=0; i<nrows; i++){
            for(int j=0; j<ncols; j++){
                gridCopy[i][j] = grid[i][j];
            }
        }

        //carry out one evlolution of game of life rules (changing state of cells if necessary) 
        for(int i=0; i<nrows; i++){
            for(int j=0; j<ncols; j++){
                int numNeighbour = numNeighbours(gridCopy,i, j, nrows, ncols);                                
                if(gridCopy[i][j] == 1){
                    if(numNeighbour < 2 || numNeighbour > 3){
                        grid[i][j] = 0;
                    }
                }
                else{ //if cell is dead
                    if(numNeighbour == 3){
                        grid[i][j] = 1;
                    }
                }
            }
        }
        if(show){ //check if each evolution should be displayed
            printf("evolution: %d", e+1);
            display(grid, nrows, ncols); //display after one whole evolution 
            printf("\n");
        }
    }
     
    //deallocate gridCopy
    for (int i=0; i<nrows; i++){
        free(gridCopy[i]);
    }
    free(gridCopy);
}

void display(char **grid, int nrows, int ncols){
    for (int i=0; i<nrows; i++){
        printf("\n");
        for (int j=0; j<ncols; j++){
            printf("%d", grid[i][j]);
        }
    }
    printf("\n");
}


void allocateGrid(char ***grid, int nrows, int ncols){
    // allocate the array
    *grid = malloc( sizeof(char *) * nrows);

    if (*grid == NULL){
        printf("ERROR1: out of memory\n");
        exit(1);
    }

    for (int i=0; i<nrows; i++){
        (*grid)[i] = malloc( sizeof(char) * ncols);
        if ((*grid)[i] == NULL){
            printf("ERROR2: out of memory\n");
            exit(1);
        }
    }
}


void populateGrid(char **grid, int nrows, int ncols){
    srand ( time(NULL) ); //seed so that rand() is unique on different runs
    for (int i=0; i<nrows; i++){
        for (int j=0; j<ncols; j++){
            grid[i][j] = rand() % 2;
        }
    }
}

