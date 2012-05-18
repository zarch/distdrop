#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <grass/gis.h>
#include <grass/raster.h>
#include <grass/glocale.h>
#include <grass/rowio.h>

#include "glob.h"
#include "cache.h"


// DEFINE GLOBAL VARIABLES
#define M_SQRT2        1.41421356237309504880  /* sqrt(2) */
#define NULL_MAP 0
#define NULL_NEIG -999
#define INPUT_DOMAIN 10000.0f
#define INPUT_NOT_DOMAIN -1.0f
#define INPUT_ROAD 0.0f
#define DIR_ROAD -1

/*
 * Possible improvements
 * =======================
 *
 * - use a stack for the row;
 * - use a cache with more than 3 rows;
 *
 */
int getrow ( int fd, void *buf, int row, int len, int direction )
{
    if ( direction > 0 )
        lseek ( fd, ( off_t ) row * len, 0 );
    else
        lseek ( fd, ( off_t ) ( nrows - row - 1 ) * len, 0 );
    if ( read ( fd, ( DCELL * ) buf, len ) != len )
        G_fatal_error ( _ ( "Error reading temporary file" ) );
    return 1;
}

int update_float_cache ( int *row, int *last_row, int *nrows, fcell_map *map )
{
    // TODO: save cache[0] into grass and free
    if ( *row - *last_row == 1 )
    {
        if ( *row > 1 && *row < *nrows-1 )
        {
            //
            // we are in the middle of the map
            //
            //write the first cache row to the HD
            Rowio_put (map->io, map->cache[0], *row);
            // free the memory
            Rowio_release(map->cache[0]);
            // change to the next row, already in the memory, just change the pointer
            map->cache[0] = map->cache[1];
            map->cache[1] = map->cache[2];
            // read the new row
            map->cache[2] = ( FCELL * ) Rowio_get ( map->io, *row );
        }
        else if ( *row == 0 )
        {
            map->cache[1] = ( FCELL * ) Rowio_get ( map->io, 0 );
            map->cache[0] = map->cache[1];
            map->cache[2] = ( FCELL * ) Rowio_get ( map->io, *row+1 );
        }
        else if ( *row == 1 )
        {
            map->cache[1] = map->cache[2];
            map->cache[2] = ( FCELL * ) Rowio_get ( map->io, *row );
        }
        else
        {
            Rowio_put (map->io, map->cache[0], *row);
            // free the memory
            G_free(map->cache[0]);
            map->cache[0] = map->cache[1];
            map->cache[1] = ( FCELL * ) Rowio_get ( map->io, *row );
            map->cache[2] = map->cache[1];
        }
    }
    else
    {
        // we must save all the cache and re-read it
        Rowio_put (map->io, map->cache[0], *row-1);
        Rowio_put (map->io, map->cache[0], *row);
        Rowio_put (map->io, map->cache[0], *row+1);
        // read and load map row to the new cache
        map->cache[0] = *row == 0? ( FCELL * ) Rowio_get ( map->io, *row ) : ( FCELL * ) Rowio_get ( map->io, *row-1 );
        map->cache[1] = ( FCELL * ) Rowio_get ( map->io, *row );
        map->cache[2] = *row == *nrows ? ( FCELL * ) Rowio_get ( map->io, *row ) : ( FCELL * ) Rowio_get ( map->io, *row-1 );
    }
    return 0;
}

