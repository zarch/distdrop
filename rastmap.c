#include <fcntl.h>

#include "rastmap.h"

int allocate_buf(cell_map *map, void *buf)
{
    if(map->type == CELL_TYPE)
        buf = (CELL *) Rast_allocate_buf ( map->type );
    if(map->type == FCELL_TYPE)
        buf = (FCELL *) Rast_allocate_buf ( map->type );
    if(map->type == DCELL_TYPE)
        buf = (DCELL *) Rast_allocate_buf ( map->type );

    return 0;
}

int check_fd(cell_map *map)
{
    map->fd = Rast_open_old ( map->name, map->mapset );

    return 0;
}


int manage_segments(seg_map *seg)
{
    seg->nseg = ( ( seg->nrows + seg->srows - 1 ) / seg->srows ) *
                ( ( seg->ncols + seg->scols - 1 ) / seg->scols );

    /* calculate total number of segments */
    if ( seg->maxmem > 0 )
         seg->segments_in_memory = ( seg->maxmem * seg->nseg ) / 100;
    /* maxmem = 0 */
    else
        seg->segments_in_memory = 4 * ( seg->nrows / seg->srows +
                                        seg->ncols / seg->scols + 2 );

    if ( seg->segments_in_memory == 0 )
        seg->segments_in_memory = 1;

    return 0;
}


int init_seg_map(cell_map *map, seg_map *seg)
{
    int size;

    if(map->type == CELL_TYPE)
        size = sizeof(CELL);
    if(map->type == FCELL_TYPE)
        size = sizeof(FCELL);
    if(map->type == DCELL_TYPE)
        size = sizeof(DCELL);

    map->temp_file = G_tempfile();

    G_message("Create tempfile %s for map <%s>\n", map->temp_file, map->name);

    map->temp_fd = creat ( map->temp_file, 0600 );
    G_message("fd tempfile %d\n", map->temp_fd);
    if ( segment_format ( map->temp_fd,
                          seg->nrows, seg->ncols,
                          seg->srows, seg->scols,
                          size) != 1 )
        G_fatal_error ( "Unable to create temporary file" );

    close ( map->temp_fd );

    map->temp_fd = open ( map->temp_file, 2 );
    if ( segment_init ( &map->seg, map->temp_fd, seg->segments_in_memory ) != 1 )
        G_fatal_error ( "Unable to create temporary file" );
    return 0;
}

int copy_segment(cell_map *map, short int direction){

    int row;
    void *buf = NULL;
    struct Cell_head window;

    /* The computation window */
    G_get_window(&window);

    if(map->type == CELL_TYPE)
        buf = (CELL *) Rast_allocate_buf ( map->type );
    if(map->type == FCELL_TYPE)
        buf = (FCELL *) Rast_allocate_buf ( map->type );
    if(map->type == DCELL_TYPE)
        buf = (DCELL *) Rast_allocate_buf ( map->type );

    /* Flush segments from memory to disk */
    segment_flush( &map->seg);

    /* Copy map to segment */
    if(direction == 0) {
        G_message("Copy map <%s> content to segment file", map->name);
        for(row = 0; row < window.rows; row++) {
            Rast_get_row(map->fd, buf, row, map->type);
            segment_put_row(&map->seg, buf, row);
        }
    }

    /* Copy segment to map */
    if(direction == 1) {
        G_message("Copy segment content to map <%s>", map->name);
        for(row = 0; row < window.rows; row++) {
            segment_get_row(&map->seg, buf, row);
            Rast_put_row(map->fd, buf, map->type);
        }
    }

    if(buf)
        G_free(buf);

    return 1;
}