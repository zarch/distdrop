#include "list.h"
#define NCOLS2 11        /* number of columns */
#define NMV 8

#ifndef DROP_H
#define DROP_H
typedef struct __move__
{
    short int mv[2];
    short int dir;
    double    dist;
} move;

#endif  /* DROP_H */


/* global variables declaration */
extern move movements[NMV];

/* global functions declaration */
extern list *get_not_null ( short int (*)[NCOLS2] );

extern int print_array_short ( short int (*)[NCOLS2] ) ;
extern int print_dir ( short int (*)[NCOLS2] ) ;
extern int print_array_double ( double (*)[NCOLS2] ) ;
extern int print_array_float ( float (*)[NCOLS2] ) ;


/*                   nrows, ncols  nmv, null_map, null_neig */
extern int set_globals (int, int, int, int ) ;
                 
extern int execute  ( move *,            /* list of movements */
                      list *,            /* list of points */
                      short (*)[NCOLS2], /* road */
                      short (*)[NCOLS2], /* domain */
                      float (*)[NCOLS2], /* elevation */
                      double (*)[NCOLS2],/* rdist */
                      short (*)[NCOLS2], /* rdir */
                      short (*)[NCOLS2], /* not_used */
                      float (*)[NCOLS2], /* drop_up */
                      float (*)[NCOLS2]  /* drop_dw */ );

extern int distdrop ( move *,            /* list of movements */
                      short (*)[NCOLS2], /* road */
                      short (*)[NCOLS2], /* domain */
                      float (*)[NCOLS2], /* elevation */
                      double (*)[NCOLS2],/* rdist */
                      short (*)[NCOLS2], /* rdid */
                      short (*)[NCOLS2], /* not_used */
                      float (*)[NCOLS2], /* drop_up */
                      float (*)[NCOLS2]  /* drop_dw */ );

