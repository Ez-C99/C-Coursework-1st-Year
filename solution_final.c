/*
  Completed the following:
  Advanced version

  Written by: <Ezra Chamba>
  Date: <28/04/2019>
*/


#include <stdio.h>
#include <stdlib.h>     //For use in random function
#include <math.h>       //For use in all mathematical calculations
#include <time.h>       //For use in setting pseudo random seed
#include <stdbool.h>    //For use of boolean data type

#define NUMROWS	    9       //Map size y
#define NUMCOLS     9       //Map size x
#define NUMWALKS    1000	//Number of random walks

/*Structures*/
typedef struct _results {
    double probresults[NUMROWS][NUMCOLS];
    double meanresults[NUMROWS][NUMCOLS];
    double sdresults[NUMROWS][NUMCOLS];
}results;

typedef struct _cellPos{        /*Storage for cell position*/
    int col;
    int row;
}cellPos;


/*Functions*/
void randomStep(cellPos *tracker);                                              //Chooses a random direction to step in
int status(const cellPos *tracker, const char statmap[NUMROWS][NUMCOLS]);       /*Looks at what cell you're on and
 * decides how to progress based on it*/
void printResults(double table[NUMROWS][NUMCOLS]);                  // Prints results in table format
void traverse(results* Output, const char travelmap[NUMROWS][NUMCOLS]);  /*Travels along the map, calling the right
 * functions accordingly*/
double calProbability(int completedwalks);                      /*Calculate the probability of successfully exiting
 * the map*/
double standardDeviation(const int *valuerange, const double avg, const int value);     /*Calculate the standard
 * deviation of the each cell to exit*/


const cellPos navdir[8] = { /*Random step move coordinates. Clockwise from N to NW. 0=N and 7=NW*/
        {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1,1}, {-1,0}, {-1,-1}};
//         N        NE      E       SE      S       SW      W       NW


int main(void){

    srand(time(NULL));           //Initialise the seed for the random generator

    /*Instantiate the structure tha will hold the results*/
    results Output;
    for (int y = 0; y < NUMROWS; y++){
        for (int x = 0; x < NUMCOLS; x++){
            Output.probresults[y][x] = 0;
            Output.meanresults[y][x] = 0;
            Output.sdresults[y][x] = 0;
        }
    }

    //Msp definition
    const char map[NUMROWS][NUMCOLS] = {{'B', 'W', 'W', 'B', 'B', 'W', 'B', 'W', 'W'},
                                        {'B', 'L', 'L', 'V', 'L', 'L', 'L', 'L', 'D'},
                                        {'W', 'L', 'L', 'L', 'L', 'D', 'L', 'L', 'D'},
                                        {'B', 'L', 'L', 'D', 'L', 'L', 'L', 'L', 'W'},
                                        {'B', 'L', 'D', 'L', 'L', 'L', 'L', 'L', 'W'},
                                        {'W', 'L', 'L', 'L', 'L', 'L', 'V', 'L', 'D'},
                                        {'W', 'V', 'L', 'L', 'L', 'L', 'L', 'L', 'W'},
                                        {'W', 'L', 'L', 'L', 'D', 'L', 'L', 'L', 'W'},
                                        {'B', 'B', 'W', 'B', 'W', 'B', 'B', 'W', 'L'},};


    /*Call the traversal function*/
    traverse(&Output, map);

    /*Print results*/
    printf("\n\nPercentage probability of getting off Jurassic Island\n");
    printResults(Output.probresults);
    printf("Mean path length when escaping\n");
    printResults(Output.meanresults);
    printf("Standard deviation of path length when escaping\n");
    printResults(Output.sdresults);

    return 0;
}


void randomStep(cellPos *tracker){      /*According to random step coordinates, choose one at random and perform the
 * calculation on the tracker structure, giving it the new coordinates*/
    const int nextcell = rand()%8;              //Random number generator between 0 and 7 from seed function earlier
    const cellPos *dir = &navdir[nextcell];
    tracker->col += dir->col;                   //New column
    tracker->row += dir->row;                   //New row
}


int status(const cellPos *tracker, const char statmap[NUMROWS][NUMCOLS]){
    const int col = tracker->col, row = tracker->row;
    int sig = 2;
    if ((row < NUMROWS && row > 0) && (col < NUMCOLS && col > 0)){
        if ((statmap[row][col] == 'B') || (statmap[row][col])){
            sig = 1;            //
        }else{
            sig = 0;            //Death condition
        }
    }
    return sig;
}


void printResults(double table[NUMROWS][NUMCOLS]){
    for (int e = 0; e < NUMROWS; e++) {
        for (int f = 0; f < NUMCOLS; f++) {
            printf("%-4.2lf\t", table[e][f]);
        }
        printf("\n");
    }
}


void traverse(results* Output, const char travelmap[NUMROWS][NUMCOLS]){
    for (int a = 0; a < NUMROWS; a++){                                      //Scan through map
        for (int b = 0; b < NUMCOLS; b++){
            if ((travelmap[a][b] == 'B') || (travelmap[a][b] == 'L')){ /*L and B are the only cells you can move from
 * when you start on them, otherwise you're dead on the other cells*/
                int cwarray[NUMWALKS];          //Store the step count of all completed walks
                int allsteps = 0;               //Step count for a completed walk
                int completedwalks = 0;         //Completed walks

                for (int c = 0; c < NUMWALKS; c++){     //Iterate as many times as steps to be taken
                    int signifier = 1;
                    cellPos tracker = {b, a};           /*Tracker structure. It was initialised in x,y format so needs
 * to be {b,a} instead of {a,b}. Otherwise calculations and results will be inverted*/
                    cwarray[completedwalks] = 0;
                    for (int d = 0;(signifier == 1) && (d < 3000000); cwarray[completedwalks]++){
                        randomStep(&tracker);
                        signifier = status(&tracker, travelmap);
                    }
                    if (signifier == 1){
                        printf("No exit path found");
                    } else if (signifier == 2){
                        allsteps += cwarray[completedwalks++];
                    }
                }
                if (completedwalks > 0){
                    Output->probresults[a][b] = calProbability(completedwalks);
                    Output->meanresults[a][b] = (double)allsteps / completedwalks;
                    Output->sdresults[a][b] = standardDeviation(cwarray, Output->meanresults[a][b], completedwalks);
                }
            }
        }
    }
}


double calProbability(int completedwalks){
    int probanswer = (double)completedwalks / NUMWALKS * 100;
    return probanswer;
}


double standardDeviation(const int *valuerange, const double avg, const int value){
    double deviation = 0;
    for (int g = 0; g < value; g++){
        deviation += pow(valuerange[g] - avg, 2);
    }
    double sdanswer = sqrt(deviation)/100;
    return sdanswer;
}



