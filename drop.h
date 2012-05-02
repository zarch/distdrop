#ifndef DROP_H
#define DROP_H

#include "list.h"
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
extern list *get_not_null ( short int **, int, int);

extern int print_dir(short int **, int, int) ;
extern int print_array (void*, int, int, int) ;
int up_neighbours ( int px, int py, move *movements, int nrows, int ncols);

/* nrows, ncols  nmv, null_map, null_neig */
extern int set_globals (int, int, int, int ) ;
                 
extern int execute  ( move *,            /* list of movements */
                      list *,            /* list of points */
                      short **, /* road */
                      short **, /* domain */
                      float **, /* elevation */
                      float **,/* rdist */
                      short **, /* rdir */
                      short **, /* not_used */
                      float **, /* drop_up */
                      float **,  /* drop_dw */
                      int, /* number of rows */
                      int /* number of cols */);

extern int distdrop ( move *,            /* list of movements */
                      short **, /* road */
                      short **, /* domain */
                      float **, /* elevation */
                      float **,/* rdist */
                      short **, /* rdid */
                      short **, /* not_used */
                      float **, /* drop_up */
                      float **,  /* drop_dw */
                      int, /* number of rows */
                      int /* number of cols */);

#endif  /* DROP_H */
