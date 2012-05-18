#ifndef DROP_H
#define DROP_H

#include <grass/rowio.h>

#define NMV 8

#define TYPE_SHORT 1
#define TYPE_INT 2
#define TYPE_FLOAT 3


/* MAP */
typedef struct __cell_map__
{
    char *name;
    int fd;
    RASTER_MAP_TYPE type;
    ROWIO io;
    CELL *buf;
    CELL **cache;
} cell_map;

typedef struct __fcell_map__
{
    char *name;
    int fd;
    RASTER_MAP_TYPE type;
    ROWIO io;
    FCELL *buf;
    FCELL **cache;
} fcell_map;




/* global functions declaration */
//                             road     domain   nrows ncols
//extern int **get_input_map ( cell_map *, cell_map *, fcell_map *, int*, int* );
//extern int update_float_cache ( int *, int *, int *, fcell_map * );
extern int getrow(int, void *, int, int);

#endif  /* DROP_H */