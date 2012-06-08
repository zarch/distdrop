#include <stdio.h>
#include <math.h>

#include "move.h"

#define M_SQRT2 1.4142135623730950f  /* sqrt(2) */
#define M_SQRT5 2.2360679774997898f  /* sqrt(5) */
/*
 *
 * |  |11|  |12|  |  |   Order in which neighbors
 * |10| 1| 2| 3|13|  |    are visited (Knight move).
 * |  | 4|  | 6|  |  |
 * |17| 7| 8| 9|14|  |
 * |  |16|  |15|  |  |
 *
 *
 */
move knight_movements[16] =
{
    //  mv  |dir|dist
    {{ 1,  1},  1, M_SQRT2}, //  1
    {{ 1,  0},  2, 1},       //  2
    {{ 1, -1},  3, M_SQRT2}, //  3
    {{ 0,  1},  4, 1},       //  4
    {{ 0, -1},  6, 1},       //  6
    {{-1,  1},  7, M_SQRT2}, //  7
    {{-1,  0},  8, 1},       //  8
    {{-1, -1},  9, M_SQRT5}, //  9
    {{-1, -1}, 10, M_SQRT5}, // 10
    {{-1, -1}, 11, M_SQRT5}, // 11
    {{-1, -1}, 12, M_SQRT5}, // 12
    {{-1, -1}, 13, M_SQRT5}, // 13
    {{-1, -1}, 14, M_SQRT5}, // 14
    {{-1, -1}, 15, M_SQRT5}, // 15
    {{-1, -1}, 16, M_SQRT5}, // 16
    {{-1, -1}, 17, M_SQRT5}, // 17
};

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
move normal_movements[8] =
{
    //  mv  |dir|dist
    {{ 1,  1}, 1, M_SQRT2}, // 1
    {{ 1,  0}, 2, 1},       // 2
    {{ 1, -1}, 3, M_SQRT2}, // 3
    {{ 0,  1}, 4, 1},       // 4
    {{ 0, -1}, 6, 1},       // 6
    {{-1,  1}, 7, M_SQRT2}, // 7
    {{-1,  0}, 8, 1},       // 8
    {{-1, -1}, 9, M_SQRT2}, // 9
};


move *get_mv( struct Cell_head *window, int knight )
{
    /* Find north-south, east_west and diagonal factors */
    float EW_fac = 1.0;
    float NS_fac = window->ns_res / window->ew_res;
    float DIAG_fac = (double)sqrt((double)(NS_fac * NS_fac + EW_fac * EW_fac));
    float V_DIAG_fac = (double)sqrt((double)(4 * NS_fac * NS_fac +
                                                 EW_fac * EW_fac));
    float H_DIAG_fac = (double)sqrt((double)(    NS_fac * NS_fac +
                                             4 * EW_fac * EW_fac));

    /*
    EW_fac /= 2.0;
    NS_fac /= 2.0;
    DIAG_fac /= 2.0;
    V_DIAG_fac /= 4.0;
    H_DIAG_fac /= 4.0;*/

    move *movements;

    if (knight){
        movements = &knight_movements;
    }
    else {
        movements = &normal_movements;
    }
    movements[0].dist = DIAG_fac * window->ew_res; // 1
    movements[1].dist = NS_fac * window->ew_res;   // 2
    movements[2].dist = DIAG_fac * window->ew_res; // 3
    movements[3].dist = EW_fac * window->ew_res;   // 4
    movements[4].dist = EW_fac * window->ew_res;   // 6
    movements[5].dist = DIAG_fac * window->ew_res; // 7
    movements[6].dist = NS_fac * window->ew_res;   // 8
    movements[7].dist = DIAG_fac * window->ew_res; // 9

    if (knight){
        movements[ 8].dist = H_DIAG_fac * window->ew_res; // 10
        movements[ 9].dist = V_DIAG_fac * window->ew_res; // 11
        movements[10].dist = V_DIAG_fac * window->ew_res; // 12
        movements[11].dist = H_DIAG_fac * window->ew_res; // 13
        movements[12].dist = H_DIAG_fac * window->ew_res; // 14
        movements[13].dist = V_DIAG_fac * window->ew_res; // 15
        movements[14].dist = V_DIAG_fac * window->ew_res; // 16
        movements[15].dist = H_DIAG_fac * window->ew_res; // 17
    }

    return movements;
}

int get_neighbours ( int px, int py, move *movements, int **neighbours,
                     int nrows, int ncols )
{
    //
    // Function that return a pointer to a neighbours array
    //
    for ( int row = 0; row < ( int ) sizeof ( movements ); row++ ){
        // compute virtual row and col
        int vrow = px + movements[row].mv[0];
        int vcol = py + movements[row].mv[1];
        // Are neighbours in the region?
        if ( vrow<0  || vcol<0 || vrow>=nrows || vcol>=ncols ){
            // set neighbour value to NULL
            neighbours[row][0] = NULL_NEIG;
            neighbours[row][1] = NULL_NEIG;
        }
        else{
            // set neighbour value
            neighbours[row][0] = vrow;
            neighbours[row][1] = vcol;
        }
    }
    return 0;
}
