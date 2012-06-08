#include <grass/gis.h>

#define TYPE_SHORT 1
#define TYPE_INT 2
#define TYPE_FLOAT 3
//#define TYPE_DOUBLE 4
#define NULL_NEIG -9999

typedef struct __move__
{
    int mv[2];
    int dir;
    float dist;
} move;

/* global variables declaration */
/* extern move movements[NMV]; */
extern move *get_mv(struct Cell_head *, int );
extern int get_neighbours ( int, int, move *, int **, int, int );
