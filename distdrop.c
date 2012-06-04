#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <grass/gis.h>
#include <grass/raster.h>
#include <grass/glocale.h>
#include <grass/segment.h>

#include "distdrop.h"


#define NULL_MAP -9999

queue **prepare_input ( cell_map *road, cell_map *domain,
                        cell_map *dist, cell_map *dir,
                        cell_map *up, cell_map *dw,
                        seg_map *segment_info)
{

    G_message( _ ("Get the file descriptor of the input maps\n" ) );
    /* Rast_open_old - returns file destriptor (>0) */
    road->fd = Rast_open_old ( road->name, "" );
    domain->fd = Rast_open_old ( domain->name, "" );

    int nrows = segment_info->nrows;
    int ncols = segment_info->ncols;

    manage_segments( segment_info );

    /* Allocate input buffer */
    allocate_buf(road);
    allocate_buf(domain);

    queue **road_queue = create_empty_array_of_queue( nrows );

    G_message( _( "Creating segment files for output maps...\n" ) );

    /* Create segmented format files for cost layer and output layer */
    dist->fd = Rast_open_new ( dist->name, dist->type );
    dir->fd = Rast_open_new ( dir->name, dir->type );
    up->fd = Rast_open_new ( up->name, up->type );
    dw->fd = Rast_open_new ( dw->name, dw->type );

    /* Initialize the segments */
    init_seg_map(dist, segment_info);
    init_seg_map(dir, segment_info);
    init_seg_map(up, segment_info);
    init_seg_map(dw, segment_info);

    /*
     *
     * Merge, road and domain, and initialize the output
     *
     */
    G_message( _( "Start to merge road and computational domain\n" ) );
    int row, col;

    float distdom = DISTDOMAIN, dropdom = DROPDOMAIN;
    float distroad = DISTROAD, droproad = DROPROAD;
    int dirdom = DIRDOMAIN, dirroad = DIRROAD;


    for ( row = 0; row < nrows; row++ ){
        G_percent ( row, nrows, 2 );
        Rast_get_row ( road->fd, road->buf, row, road->type  );
        Rast_get_row ( domain->fd, domain->buf, row, domain->type  );

        for ( col = 0; col < ncols; col++ ){

            void *val = NULL;
            FCELL null_map_f = NULL_MAP;
            CELL null_map_c = NULL_MAP;

            if(domain->type == CELL_TYPE)
                val = (void *)&domain->cbuf[col];
            if(domain->type == FCELL_TYPE)
                val = (void *)&domain->fbuf[col];
            if(domain->type == DCELL_TYPE)
                val = (void *)&domain->dbuf[col];


            if ( Rast_is_null_value( val, domain->type) == 0 ){
            //if (&domain->buf[col] == 1 ){
                // Domain cell is not null
                segment_put ( &dist->seg, &distdom, row, col );
                segment_put ( &dir->seg, &dirdom, row, col );
                segment_put ( &up->seg, &dropdom, row, col );
                segment_put ( &dw->seg, &dropdom, row, col );
            }
            else {
                segment_put ( &dist->seg, &null_map_f, row, col );
                segment_put ( &dir->seg, &null_map_c, row, col );
                segment_put ( &up->seg, &null_map_f, row, col );
                segment_put ( &dw->seg, &null_map_f, row, col );            ;
            }

            if(road->type == CELL_TYPE)
                val = (void *)&road->cbuf[col];
            if(road->type == FCELL_TYPE)
                val = (void *)&road->fbuf[col];
            if(road->type == DCELL_TYPE)
                val = (void *)&road->dbuf[col];

            if ( Rast_is_null_value( val, road->type) == 0 ){
                // Road cell is not null
                segment_put ( &dist->seg, &distroad, row, col );
                segment_put ( &dir->seg, &dirroad, row, col );
                segment_put ( &up->seg, &droproad, row, col );
                segment_put ( &dw->seg, &droproad, row, col );
                array_append(row, col, road_queue);
            }
        }
        printf( " \n" );
    }

    G_message( _( "Close the input maps\n" ) );
    /* closing input raster maps */
    Rast_close ( road->fd );
    Rast_close ( domain->fd );


    G_message( _( "Write segments to the hard disk\n" ) );
    /* Flush segments from memory to disk */
    segment_flush( &dist->seg);
    segment_flush( &dir->seg);
    segment_flush( &up->seg);
    segment_flush( &dw->seg);

    return road_queue;
}


int queue_pixel_core ( move *movements, queue **redo_rows,
                       cell_map *dist, cell_map *elev,
                       cell_map *dir, cell_map *up, cell_map *dw,
                       seg_map *segment_info, int row, int *count)
{
    // pass
    printf(' ');
}

/*{
    elem *el = pop( redo_rows[row] ), *prev;
    int row_canged = 0;
    int *neighbours[8][2] = 0;

    // if el, update cache
    if (el != NULL)
    {
        dist_cache = update_float_cache ( &row, last_row, &nrows, dist_cache, rdist );
        elev_cache = update_float_cache ( &row, last_row, &nrows, elev_cache, elevation );
        dir_cache = update_short_cache ( &row, last_row, &nrows, dir_cache, rdir );
        up_cache = update_float_cache ( &row, last_row, &nrows, up_cache, rdrop_up );
        dw_cache = update_float_cache ( &row, last_row, &nrows, dw_cache, rdrop_dw );
        *last_row = row;
    }

    // check if there is pixel to do in the row
    while (el != NULL)
    {
        int col = el->point.col;
        if ( dist_cache[1][col] >= 0 )
        {
            // check if is road, and set direction to -1
            dir_cache[1][col] = dist_cache[1][col] == 0 ? DIR_ROAD: dir_cache[1][col];
            // get cell neighbours
            get_neighbours ( &row, &col, movements, neighbours, &nrows, &ncols );
            for ( int n = 0; n < NMV ; n++ )
            {
                // TODO: after check if there are performce consegunece to declaire here or not
                int nx = neighbours[n][0];
                int ny = neighbours[n][1];

                //Check if neighbours are inside the region AND the domain
                if ( nx != NULL_NEIG && dist_cache[nx][ny]>0 )
                {
                    float new_dist = dist_cache[1][col] + movements[n].dist;
                    if ( dist_cache[nx][ny] > new_dist )
                    {
                        // assign the smaller one
                        dist_cache[nx][ny] = new_dist;
                        dir_cache[nx][ny] = movements[n].dir;
                        //check if drop is positive or negative
                        float drop = elev_cache[nx][ny] - elev_cache[1][col];
                        if ( drop >= 0 )
                        {
                            up_cache[nx][ny] = up_cache[1][col] + drop;
                            dw_cache[nx][ny] = dw_cache[1][col];
                        }
                        else
                        {
                            up_cache[nx][ny] = up_cache[1][col];
                            dw_cache[nx][ny] = dw_cache[1][col] + drop;
                        }
                        array_append(row + nx - 1, ny, redo_rows);
                        *count += 1;
                        row_canged += 1; // debug only
                    }
                }
            }
        }
        prev = el;
        free(prev);
        el = pop( redo_rows[row] );
    }
    if (row_canged)
    {
        printf("--------------------------------\n");
        printf("dist_cache, row: %d\n", row);
        print_array(dist_cache, TYPE_FLOAT, 3, 11);
    }
}

*/





int queue_pixel ( queue **redo_rows,
                  cell_map *elev, cell_map *dist,
                  cell_map  *dir, cell_map *up, cell_map *dw,
                  seg_map *segment_info, move *movements )
{
    int count = 0;//,  nx, ny;

    printf ( "\n\n↓\n" );
    for ( int i = 0; i < segment_info->nrows; i++ ){
        queue_pixel_core ( movements, redo_rows, dist, elev, dir,
                          up, dw, segment_info,
                          i, &count);
    }
    if (count > 0)
    {
        count = 0;
        printf ( "\n\n↑\n" );
        for ( int i = segment_info->nrows-1; i >= 0; i-- ){
            queue_pixel_core ( movements, redo_rows, dist, elev, dir,
                          up, dw, segment_info,
                          i, &count);
        }
    }
    printf("===========================================\n");
    return count;
}


int distdrop ( cell_map *elev,
               cell_map *dist, cell_map *dir,
               cell_map *up, cell_map *dw,
               seg_map *segment_info, move *movements,
               queue **redo_rows)
{
    int all_done = 1;

    while ( all_done ){ // if all_done != 0? continue: break
        all_done = queue_pixel ( redo_rows,  elev, dist,
                                 dir, up, dw, segment_info,
                                 movements);
    }
    return 0;
}


