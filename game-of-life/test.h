#ifndef TEST_H_   
#define TEST_H_

#include <stdbool.h>

//check if grid and answerGrid are the same
bool eqGrid(char **grid, char** answerGrid, int nrows, int ncols);

//Increment test number(int n) if grid is same as answerGrid, exiting and printing both grids if answer is incorrect
int eq(int n, char **grid, char** answerGrid, int nrows, int ncols);

//set all grid values to zero
void zeroGrid(char **grid, int nrows, int ncols);

//run tests for 2x2 grids
int test2x2(int n);

//run tests for 3x3 grids
int test3x3(int n);

//run tests for non-sqaure grids, 3x4 and 4x3
int test4x3(int n);
int test3x4(int n);

/* run all tests */
void test();



#endif 