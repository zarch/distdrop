#ifndef DROP_H
#define DROP_H

#include "queue_alg.h"
#define NMV 8

#define TYPE_SHORT 1
#define TYPE_INT 2
#define TYPE_FLOAT 3
#define TYPE_DOUBLE 4

typedef struct __move__
{
    short int mv[2];
    short int dir;
    float    dist;
} move;

/* global variables declaration */
extern move movements[NMV];

/* global functions declaration */
extern int print_dir(short int **, int, int) ;
extern int print_array (void*, int, int, int) ;
extern queue *get_row_not_null( queue **, int );

//                             road     domain   nrows ncols
extern float **get_input_map ( short**, short**, int*, int* );

extern int *get_neighbours ( int *,  //px,
                             int *,  //py,
                             move *, //movements,
                             int **,  //neighbours[NMV][2],
                             int *,  //nrows,
                             int *); //ncols )


extern int distdrop ( move   *,  // movements,
                      float  **, //rdist,
                      float  **, //elevation,
                      float  **, //rdrop_up,
                      float  **, //rdrop_dw,
                      short  **, //rdir,
                      int,       // nrows,
                      int);      // ncols


extern int distdrop_queue ( move   *,  // movements,
                      float  **, //rdist,
                      float  **, //elevation,
                      float  **, //rdrop_up,
                      float  **, //rdrop_dw,
                      short  **, //rdir,
                      int,       // nrows,
                      int);      // ncols

#endif  /* DROP_H */
