#define NROWS 10
#define NCOLS 11
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
extern short int road;
extern short int domain;
extern float elevation;
extern double rdist;
extern short int rdir;
extern float rdrop_up;
extern float rdrop_dw;
extern short int not_used;
extern short int mv;
extern short int dir;
extern double dist;
extern move movement;

