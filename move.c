#include <stdio.h>
#include "move.h"

#define M_SQRT2 1.41421356237309504880f  /* sqrt(2) */
#define NULL_NEIG 9999

/*
 *
 * DEFINE MOVEMENTS
 *
 * Movement & Direction
 * -1 0 1
 * |1|2|3| -1
 * |4| |6|  0
 * |7|8|9|  1
 */
move movements[NMV] =
{
    //  mv  |dir|dist
    {{ 1,  1}, 1, M_SQRT2},
    {{ 1,  0}, 2, 1},
    {{ 1, -1}, 3, M_SQRT2},
    {{ 0,  1}, 4, 1},
    {{ 0, -1}, 6, 1},
    {{-1,  1}, 7, M_SQRT2},
    {{-1,  0}, 8, 1},
    {{-1, -1}, 9, M_SQRT2},
};

int get_neighbours ( int *px, int *py, move *movements, int **neighbours,
                            int *nrows, int *ncols )
{
    //
    // Function that return a pointer to a neighbours array
    //

    for ( int row = 0; row < ( int )sizeof ( movements ); row++ ){
        // compute virtual row and col
        int vrow = *px + movements[row].mv[0];
        int vcol = *py + movements[row].mv[1];
        // Are neighbours in the region?
        if ( vrow<0  || vcol<0 || vrow>=*nrows || vcol>=*ncols ){
            // set neighbour value to NULL
            neighbours[row][0] = NULL_NEIG;
            neighbours[row][1] = NULL_NEIG;
        }
        else{
            // set neighbour value
            // I don't use vrow, because I use cache
            // transform row from -1, 0, 1 to 0, 1, 2
            neighbours[row][0] = movements[row].mv[0] + 1;
            neighbours[row][1] = vcol;
        }
        //printf ( "nx:%d ny:%d\n", neighbours[row][0], neighbours[row][1]);
    }
    return 0;
}
