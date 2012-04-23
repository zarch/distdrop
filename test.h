#define NROWS 10        /* number of rows */
#define NCOLS 11        /* number of columns */
#define NMV 8           /* number of possible movement */
#define NULL_MAP 0      /* value of null value in the map */
#define NULL_NEIG -9999 /* value of null value in the map */


#ifndef TEST_H
#define TEST_H
typedef struct __move__
{
    short int mv[2];
    short int dir;
    double    dist;
} move;

#endif  /* TEST_H */


/* global variables declaration */

extern short int road[NROWS][NCOLS];
extern short int domain[NROWS][NCOLS];
extern float elevation[NROWS][NCOLS];
extern double rdist[NROWS][NCOLS];
extern short int rdir[NROWS][NCOLS];
extern float rdrop_up[NROWS][NCOLS];
extern float rdrop_dw[NROWS][NCOLS];
extern short int not_used[NROWS][NCOLS];
extern move movements[NMV];


