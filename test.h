#define NROWS 10
#define NCOLS 11
#define NMV 8
#define NULL_MAP 0


#ifndef TEST_H
#define TEST_H
typedef struct __move__
{ short int    length;
  short int*   mv;
  short int*   dir;
  double* dist;
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
extern short int mv[NMV][2];
extern short int dir[NMV];
extern double dist[NMV];
extern move movement;

