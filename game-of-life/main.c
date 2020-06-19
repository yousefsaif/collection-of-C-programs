#include <stdio.h>
#include <stdlib.h>
#include "logic.h"
#include "test.h"


int main(int argc, char **argv){
    if(argc == 1){ //run tests
        printf("Running tests..\n");
        test();
    }
    else if(argc >= 2 && argc <=4){ //run based on user input 
        int nrows, ncols; //number of rows and columns
        int nevolutions = 10; //number of evolutions for game, set to 10 by defualt

        //set number of rows,cols and evolutions from user input
        nrows = atoi(argv[1]);
        if(argc == 2)
            ncols = nrows; //square grid
        else //argc == 3 or 4 
            ncols = atoi(argv[2]);
        
        if(argc == 4)
            nevolutions = atoi(argv[3]); //update number of evolutions to user input
        
        // check if grid size is valid
        if( nrows <= 0 || ncols <= 0){
            fprintf(stderr, "can't have grid size less that 1, try using a positive integer\n");
            exit(1);
        }
        if(nevolutions < 0){
            fprintf(stderr, "only use positive integers\n");
            exit(1);
        }
        printf("nrows=%d   ncols=%d  evolutions=%d\n", nrows, ncols, nevolutions);


        //create and print inital grid
        char **grid;
        allocateGrid(&grid, nrows, ncols);
        populateGrid(grid, nrows, ncols); //set cell states at random
        printf("\ninital grid");
        display(grid, nrows, ncols);
        printf("\n");

        /* Evolve and display here, maybe set to 10 rounds as defualt */
        evolve(grid, nrows, ncols, nevolutions, true);
       
        /* deallocate the array */
        for (int i=0; i<nrows; i++){
            free(grid[i]);
        }
        free(grid);
    }
    else{ //proivde user with instructions on how to use program
        fprintf(stderr, "Use: ./main or ./main X  or ./main X Y or ./main X Y Z\n");
        fprintf(stderr, "Where X and Y are positive integers representing numbers of rows and cols respectively\n");
        fprintf(stderr, "And Z represents the number of evolutions you want\n");
        fprintf(stderr, "Using only X(./main X) creates a square grid\n");
        exit(1);
    }
    
    return 0;
}