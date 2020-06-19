#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include "logic.h"

bool eqGrid(char **grid, char** answerGrid, int nrows, int ncols){
    bool ans = true;
    for(int i=0; i<nrows; i++){
        for(int j=0; j<ncols; j++){
            if(grid[i][j] != answerGrid[i][j]){
                return false;
            }
        }
    }
    return ans;
}

int eq(int n, char **grid, char** answerGrid, int nrows, int ncols){ 
    n++;

    if(!eqGrid(grid, answerGrid, nrows, ncols)){
        printf("Test %d fails", n);
        display(grid, nrows, ncols);
        printf("\nnot same as\n");
        display(answerGrid, nrows, ncols);
        exit(1);
    }
    printf("Test %d passed\n", n);
  
    return n;
}

void zeroGrid(char **grid, int nrows, int ncols){
    for (int i=0; i<nrows; i++){
        for (int j=0; j<ncols; j++){
            grid[i][j] = 0;
        }
    }
}

/*  tests for 2x2 grid*/
int test2x2(int n){
    int nrows = 2, ncols = 2;

    //setting up test grids
    char **grid;
    allocateGrid(&grid, nrows, ncols);
    zeroGrid(grid, nrows, ncols); //set all to zero
    char **answerGrid;
    allocateGrid(&answerGrid, nrows, ncols);
    zeroGrid(answerGrid, nrows, ncols);

    /* Checking program carries out below correctly 
        1 1     =>  1 1  => 1 1  => all cells stay alive for all further evolutions
        1 0         1 1     1 1     (grid remains the same for all further evolutions)
    */
    //setting inital grid
    grid[0][0] = 1; grid[0][1] = 1;
    grid[1][0] = 1; 
    //setting answer
    answerGrid[0][0] = 1; answerGrid[0][1] = 1;
    answerGrid[1][0] = 1; answerGrid[1][1] = 1;
    //Tests #1 - #3
    evolve(grid,nrows,ncols, 1, false); //evolve grid once 
    n = eq(n, grid, answerGrid, nrows, ncols); //check if grid was evolved correctly 
    evolve(grid,nrows,ncols, 1, false); //evolve grid once 
    n = eq(n, grid, answerGrid, nrows, ncols); //check grid remained the same 
    evolve(grid,nrows,ncols, 25, false); //evolve grid 25 times
    n = eq(n, grid, answerGrid, nrows, ncols); //check grid remained the same 


    /* Checking program carries out below correctly 
        1 0     =>  0 0  => 0 0  => all cells stay dead for all further evolutions
        0 1         0 0     0 0     (grid remains the same for all further evolutions)
    */
    //setting inital grid
    zeroGrid(grid, nrows, ncols);
    //setting answer
    zeroGrid(answerGrid, nrows, ncols);
    //Tests #4 - #6
    evolve(grid,nrows,ncols, 1, false); //evolve grid once 
    n = eq(n, grid, answerGrid, nrows, ncols); //check if grid was evolved correctly 
    evolve(grid,nrows,ncols, 1, false); //evolve grid once 
    n = eq(n, grid, answerGrid, nrows, ncols); //check grid remained the same 
    evolve(grid,nrows,ncols, 10, false); //evolve grid 10 times
    n = eq(n, grid, answerGrid, nrows, ncols); //check grid remained the same 


    //deallocate the arrays
    for (int i=0; i<nrows; i++){
        free(grid[i]),free(answerGrid[i]);
    }
    free(grid), free(answerGrid);  

    return n;
}

/*  tests for 3x3 grid*/
int test3x3(int n){
    int nrows = 3, ncols = 3;

    //setting up test grids
    char **grid;
    allocateGrid(&grid, nrows, ncols);
    zeroGrid(grid, nrows, ncols);
    char **answerGrid;
    allocateGrid(&answerGrid, nrows, ncols);
    zeroGrid(answerGrid, nrows, ncols);

    /* Checking program carries out below correctly 
        1 1 0    =>  0 1 0  =>  0 0 0   => all cells stay dead for all further evolutions
        0 0 1        0 1 0      0 0 0     (grid remains the same for all further evolutions)
        0 0 0        0 0 0      0 0 0
    */
    //setting inital grid
    grid[0][0] = 1; grid[0][1] = 1;
    grid[1][2] = 1; 
    //setting answer (for first evolution)
    answerGrid[0][1] = 1;
    answerGrid[1][1] = 1;
    //Tests #7 
    evolve(grid,nrows,ncols, 1, false); //evolve grid once 
    n = eq(n, grid, answerGrid, nrows, ncols); //check if grid was evolved correctly 
    // Test #8
    zeroGrid(answerGrid, nrows, ncols); // updating answer for next evolution
    evolve(grid,nrows,ncols, 1, false); //evolve grid once 
    n = eq(n, grid, answerGrid, nrows, ncols); //check grid evolved correctly
    // Test #9
    evolve(grid,nrows,ncols, 100, false); //evolve grid 100 times
    n = eq(n, grid, answerGrid, nrows, ncols); //check grid remained the same 


    /* Checking program carries out below correctly 
        0 0 0   =>  0 1 0  =>  0 0 0   
        1 1 1       0 1 0      1 1 1     
        0 0 0       0 1 0      0 0 0
    */
    //setting inital grid
    grid[1][0] = 1; grid[1][1] = 1; grid[1][2] = 1;
    //setting answer (for first evolution)
    answerGrid[0][1] = 1;
    answerGrid[1][1] = 1;
    answerGrid[2][1] = 1;
    //Tests #10
    evolve(grid,nrows,ncols, 1, false); //evolve grid once 
    n = eq(n, grid, answerGrid, nrows, ncols); //check if grid was evolved correctly 
    //updating answer (for next evoltion)
    zeroGrid(answerGrid, nrows, ncols);
    answerGrid[1][0] = 1; answerGrid[1][1] = 1; answerGrid[1][2] = 1;
    //Test #11
    evolve(grid,nrows,ncols, 1, false); //evolve grid once 
    n = eq(n, grid, answerGrid, nrows, ncols); //check grid evolved correctly

    //deallocate the arrays
    for (int i=0; i<nrows; i++){
        free(grid[i]),free(answerGrid[i]);
    }
    free(grid), free(answerGrid);  

    return n;
}

 int test4x3(int n){
    int nrows = 4, ncols = 3;

    //setting up test grids
    char **grid;
    allocateGrid(&grid, nrows, ncols);
    zeroGrid(grid, nrows, ncols);
    char **answerGrid;
    allocateGrid(&answerGrid, nrows, ncols);
    zeroGrid(answerGrid, nrows, ncols);

    /* Checking program carries out below correctly 
        1 1 1    =>  1 0 1  =>  0 0 0   => all cells stay dead for all further evolutions
        1 1 1        0 0 0      0 0 0     (grid remains the same for all further evolutions)
        1 1 1        0 0 0      0 0 0
        1 1 1        1 0 1      0 0 0 
    */
    //setting inital grid
    for(int i=0; i<nrows; i++){
        for(int j=0; j<ncols; j++){
            grid[i][j] = 1;
        }
    }
    //setting answer (for first evolution)
    answerGrid[0][0] = 1; answerGrid[0][2] = 1;
    answerGrid[3][0] = 1; answerGrid[3][2] = 1;
    //Tests #12 
    evolve(grid,nrows,ncols, 1, false); //evolve grid once 
    n = eq(n, grid, answerGrid, nrows, ncols); //check if grid was evolved correctly 
    // Test #13
    zeroGrid(answerGrid, nrows, ncols); //updating answer for next evolution
    evolve(grid,nrows,ncols, 1, false); //evolve grid once 
    n = eq(n, grid, answerGrid, nrows, ncols); //check grid evolved correctly
    // Test #14
    evolve(grid,nrows,ncols, 30, false); //evolve grid 30 times
    n = eq(n, grid, answerGrid, nrows, ncols); //check grid remained the same 

    //deallocate the arrays
    for (int i=0; i<nrows; i++){
        free(grid[i]),free(answerGrid[i]);
    }
    free(grid), free(answerGrid);  

     return n;
 }

 int test3x4(int n){
    int nrows = 3, ncols = 4;

    //setting up test grids
    char **grid;
    allocateGrid(&grid, nrows, ncols);
    zeroGrid(grid, nrows, ncols);
    char **answerGrid;
    allocateGrid(&answerGrid, nrows, ncols);
    zeroGrid(answerGrid, nrows, ncols);

    /* Checking program carries out below correctly 
        1 1 1 0    =>  1 1 1 0  =>  1 0 1 0  =>  0 0 0 0
        1 0 0 1        1 0 1 0      1 0 1 0      0 0 0 0 
        0 0 0 0        0 0 0 0      0 0 0 0      0 0 0 0
    */
    //setting inital grid
    grid[0][0] = 1; grid[0][1] = 1; grid[0][2] = 1;
    grid[1][0] = 1; grid[1][3] = 1;
    //setting answer (for first evolution)
    answerGrid[0][0] = 1; answerGrid[0][1] = 1; answerGrid[0][2] = 1;
    answerGrid[1][0] = 1; answerGrid[1][2] = 1;
    //Tests #15 
    evolve(grid,nrows,ncols, 1, false); //evolve grid once 
    n = eq(n, grid, answerGrid, nrows, ncols); //check if grid was evolved correctly 
    // Test #16
    answerGrid[0][1] = 0; //update answer
    evolve(grid,nrows,ncols, 1, false); //evolve grid once
    n = eq(n, grid, answerGrid, nrows, ncols); //check if grid was evolved correctly
    // Test #17
    zeroGrid(answerGrid, nrows, ncols); //update answer
    evolve(grid,nrows,ncols, 1, false); //evolve grid once
    n = eq(n, grid, answerGrid, nrows, ncols); //check if grid was evolved correctly

    //deallocate the arrays
    for (int i=0; i<nrows; i++){
        free(grid[i]),free(answerGrid[i]);
    }
    free(grid), free(answerGrid);  

     return n;
 }



void test(){
    int n = 0;
    
    //Tests #1 - #6?  (2x2 tests)
    n = test2x2(n);
    //Tests #7 - 11?  (3x3 tests)
    n = test3x3(n);
    /* non-square Tests */
    //Tests #12 - #14?  (4x3 tests)
    n = test4x3(n);
    //Tests #15 - 17?   (3x4 tests)
    n = test3x4(n);

    printf("Tests passed: %d\n", n); 
}

