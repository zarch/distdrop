#include "move.h"

#define M_SQRT2 1.41421356237309504880f  /* sqrt(2) */

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