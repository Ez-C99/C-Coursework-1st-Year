/*
  Completed the following:
  Advanced version

  Written by: <Ezra Chamba 10175465>
  Date: <28/04/2019>
*/


#include <stdio.h>
#include <stdlib.h>     //For use in random function
#include <math.h>       //For use in all mathematical calculations
#include <time.h>       //For use in setting pseudo random seed
#include <string.h>
#include <stdbool.h>    //For use of boolean data type

#define NUMROWS	    9       //Map size y
#define NUMCOLS     9       //Map size x
#define NUMWALKS    1000	//Number of random walks


/*Structures*/
typedef struct _results {
    double probres[NUMROWS][NUMCOLS];
    double meanres[NUMROWS][NUMCOLS];
    double sdres[NUMROWS][NUMCOLS];
}results;

typedef struct _cellPos{        /*Storage for cell position*/
    int col;
    int row;
}cellPos;

/*Functions*/
void randomStep(cellPos *tracker);                  //Calculates random step
int status(const cellPos *tracker, const char map[NUMROWS][NUMCOLS]);           // return the status of the next step
void printResults(double table[NUMROWS][NUMCOLS]);
double standardDeviation(int *walksum, double meanres, int stepsum);
void traverse(results *data, const char area[NUMROWS][NUMCOLS]);

const cellPos navdir[8] = {     /*Random step movement coordinates. Going clockwise from 0 to 7 with
 * 0 N and 7 as NW*/
        {0, -1},
        {1, -1},
        {1, 0},
        {1, 1},
        {0, 1},
        {-1,1},
        {-1,0},
        {-1,-1}};

int main() {

    srand(time(NULL));      //Initialise the seed for the random generator

    results output;
    for(int y =0; y < NUMROWS; y++)
    {
        for(int x = 0; x < NUMCOLS; x++)
        {
            output.probres[y][x] = 0;
            output.meanres[y][x] = 0;
            output.sdres[y][x] = 0;
        }
    }

    //Msp definition
    const char map[NUMROWS][NUMCOLS] = {{'B', 'W', 'W', 'B', 'B', 'W', 'B', 'W', 'W'},
                                        {'B', 'L', 'L', 'V', 'L', 'L', 'L', 'L', 'B'},
                                        {'W', 'L', 'L', 'L', 'L', 'D', 'L', 'L', 'B'},
                                        {'B', 'L', 'L', 'D', 'L', 'L', 'L', 'L', 'W'},
                                        {'B', 'L', 'D', 'L', 'L', 'L', 'L', 'L', 'W'},
                                        {'W', 'L', 'L', 'L', 'L', 'L', 'V', 'L', 'B'},
                                        {'W', 'V', 'L', 'L', 'L', 'L', 'L', 'L', 'W'},
                                        {'W', 'L', 'L', 'L', 'D', 'L', 'L', 'L', 'W'},
                                        {'B', 'B', 'W', 'B', 'W', 'B', 'B', 'W', 'B'},};

    traverse(&output, map);


    //* Print results:
    printf("\n\nPercentage probability of getting off Jurassic Island\n");
    printResults(output.probres);
    printf("Mean path length when escaping\n");
    printResults(output.meanres);
    printf("Standard deviation of path length when escaping\n");
    printResults(output.sdres);


    return 0;
}

void randomStep(cellPos *tracker){
    const int nextcell = rand()%8;
    const cellPos *dir = &navdir[nextcell];
    tracker->col += dir->col;
    tracker->row += dir->row;
}

int status(const cellPos *tracker, const char map[NUMROWS][NUMCOLS]) {
    int code = 2;               //Exit case
    const int row = tracker->row, col = tracker->col;
    if ((row >= 0 && row < NUMROWS) && (col >= 0 && col < NUMCOLS)) {
        if ((col > 0 && col < NUMCOLS)) {
            if ((map[row][col] == 'B') || (map[row][col] == 'L')) {
                code = 1;           //Continue traversal case
            } else {
                code = 2;           //Dead case
            }
        }
        return code;
    }
}


void printResults(double table[NUMROWS][NUMCOLS]) {
    for (int a = 0; a < NUMROWS; a++) {
        for (int b = 0; b < NUMCOLS; b++) {
            printf("%.2lf\t", table[a][b]);
        }
        printf("\n");
    }
}

double standardDeviation(int *walksum, double meanres, int stepsum) {
    double deviation = 0;
    for (int a = 0; a < stepsum; a++) {
        deviation += pow(walksum[a] - meanres, 2);
    }
    return sqrt(deviation / stepsum);
}

void traverse(results *data, const char area[NUMROWS][NUMCOLS]) {
    /* Perform random walks and calculate results: */
    for (int e = 0; e < NUMROWS; e++) {
        for (int f = 0; f < NUMCOLS; f++) {                         //Scan through all cells on map to iterate on each
            if ((area[e][f] == 'B') || (area[e][f] == 'L')) {                         //Continuation condition for cell
                int sum = 0;
                int steptotal = 0;
                int walksum[NUMWALKS];

                for (int i = 0; i < NUMWALKS; i++) {
                    cellPos tracker = {e, f};
                    walksum[sum] = 0;
                    int alive = 1; //status(&tracker, map);
                    for (int e = 0; (e < 10000) && (alive == 1); e++, walksum[sum]++) {
                        sum++;
                        randomStep(&tracker);
                        alive = status(&tracker, area);
                    }
                    if (alive == 2) {
                        steptotal += walksum[sum++];
                    }
                }
                if (sum > 0) {
                    for (int y = 0; y < NUMROWS; y++) {
                        for (int x = 0; x < NUMROWS; x++) {
                            data->probres[e][f] = ((double) sum / NUMWALKS) * 100;
                            data->meanres[e][f] = (double) steptotal / sum;
                            data->sdres[e][f] = standardDeviation(walksum, data->meanres[e][f], sum);
                        }
                    }
                }
            }
        }
    }
}

