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

static int **new_int_map (int nrows, int ncols, int *set_number )
{
    int    **new_map = ( int**    ) malloc ( nrows * sizeof ( int *    ) );
    if ( new_map == NULL ){
        fprintf ( stderr, "Unable allocate memory" );
        exit ( 1 );
    }

    for ( int i = 0; i < nrows; i++ ){
        //new_map[i] = ( short* ) calloc ( *ncols, sizeof ( short ) );
        new_map[i] = ( int* ) malloc ( ncols * sizeof ( int ) );
        if ( new_map[i] == NULL ){
            fprintf ( stderr, "Unable allocate memory" );
            exit ( 1 );
        }
        if ( set_number != NULL ){
            for ( int j = 0; j < ncols; j++ ){
                new_map[i] = set_number;
            }
        }
    }
    return new_map;
}

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
                int seg_numb = get_segment_number(row, col, segment_info);
                array_append(seg_numb, row, col, road_queue);
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


static int queue_pixel_core ( move *movements, queue **redo_segments,
                              cell_map *dist, cell_map *elev,
                              cell_map *dir, cell_map *up, cell_map *dw,
                              seg_map *segment_info, int seg, int *count,
                              int **neighbours )
{
    FCELL dist_cell = 0, dist_cell_neig = 0, up_cell = 0, dw_cell = 0;
    FCELL elev_cell = 0, elev_cell_neig = 0;
    //CELL dir_cell = 0;

    elem *el = pop( redo_segments[seg] ), *prev;
    //int *neighbours[NMV] = {{0, 0},{0, 0},};

    G_debug ( 4, "Segment number: %d\n", seg );

    // check if there is pixel to do in the row
    while (el != NULL)
    {
        int row = el->point.row;
        int col = el->point.col;
        segment_get( &dist->seg, &dist_cell, row, col );

        if ( dist_cell >= 0 )
        {
            // get cell neighbours
            get_neighbours ( row, col, movements, neighbours,
                             segment_info->nrows, segment_info->ncols );

            for ( int n = 0; n < NMV ; n++ )
            {
                // TODO: after check if there are performce consegunece to declaire here or not
                int nx = neighbours[n][0];
                int ny = neighbours[n][1];

                //Check if neighbours are inside the region AND the domain
                if ( nx != NULL_NEIG){
                    // get distance value
                    segment_get( &dist->seg, &dist_cell_neig, nx, ny );
                    //Check if distance is in the domain
                    if (dist_cell_neig>0 )
                    {
                        float new_dist = dist_cell + movements[n].dist;
                        if ( dist_cell_neig > new_dist )
                        {
                            // assign the smaller one
                            segment_put ( &dist->seg, &new_dist, nx, ny );
                            segment_put ( &dir->seg, &movements[n].dir, nx, ny );
                            //check if drop is positive or negative
                            segment_get( &elev->seg, &elev_cell, row, col );
                            segment_get( &elev->seg, &elev_cell_neig, nx, ny );
                            float drop = elev_cell_neig - elev_cell;
                            if ( drop >= 0 )
                            {
                                segment_get( &up->seg, &up_cell, row, col );
                                up_cell += drop;
                                segment_put ( &up->seg, &up_cell, nx, ny );
                                //up_cache[nx][ny] = up_cache[1][col] + drop;
                                segment_get( &dw->seg, &dw_cell, row, col );
                                segment_put ( &dw->seg, &dw_cell, nx, ny );
                                //dw_cache[nx][ny] = dw_cache[1][col];
                            }
                            else
                            {
                                segment_get( &up->seg, &up_cell, row, col );
                                segment_put ( &up->seg, &up_cell, nx, ny );
                                //up_cache[nx][ny] = up_cache[1][col];
                                segment_get( &dw->seg, &dw_cell, row, col );
                                dw_cell += drop;
                                segment_put ( &dw->seg, &dw_cell, nx, ny );
                                //dw_cache[nx][ny] = dw_cache[1][col] + drop;
                            }
                            int seg_numb = get_segment_number(nx, ny, segment_info);
                            array_append(seg_numb, nx, ny, redo_segments);
                            *count += 1;
                        }
                    }
                }
            }
        }
        prev = el;
        free(prev);
        el = pop( redo_segments[seg] );
    }
    return 0;
}


static int queue_pixel ( queue **redo_segments,
                  cell_map *elev, cell_map *dist,
                  cell_map  *dir, cell_map *up, cell_map *dw,
                  seg_map *segment_info, move *movements, int **neighbours )
{
    int count = 0;//,  nx, ny;

    printf ( "\n\n↓\n" );
    for ( int i = 0; i < segment_info->nseg; i++ ){
        queue_pixel_core ( movements, redo_segments, dist, elev, dir,
                          up, dw, segment_info,
                          i, &count, neighbours);
    }
    if (count > 0)
    {
        count = 0;
        printf ( "\n\n↑\n" );
        for ( int i = segment_info->nseg-1; i >= 0; i-- ){
            queue_pixel_core ( movements, redo_segments, dist, elev, dir,
                          up, dw, segment_info,
                          i, &count, neighbours);
        }
    }
    printf("===========================================\n");
    return count;
}


int distdrop ( cell_map *elev,
               cell_map *dist, cell_map *dir,
               cell_map *up, cell_map *dw,
               seg_map *segment_info, move *movements,
               queue **redo_segments)
{
    int all_done = 1;

    int **neighbours = new_int_map ( ( int ) sizeof ( movements ),
                                     ( int ) sizeof ( movements[0] ), NULL );

    elev->fd = Rast_open_old ( elev->name, "" );
    init_seg_map(elev, segment_info);

    while ( all_done ){ // if all_done != 0? continue: break
        all_done = queue_pixel ( redo_segments,  elev, dist,
                                 dir, up, dw, segment_info,
                                 movements, neighbours);
    }
    return 0;
}

int print_dir ( cell_map *map, seg_map *seg )
{
    FILE *fp;
    fp = fopen(map->name, "w");
    fprintf(fp, "# -*- coding: utf-8 -*-\n");
    CELL dir_cell = 0;
    for ( int row = 0; row < seg->nrows; row++ )
    {
        //printf("%d: ", row);
        for ( int col = 0; col < seg->ncols; col++ )
        {
            segment_get( &map->seg, &dir_cell, row, col );
            switch ( dir_cell )
            {
                // utf8 symbols: ⬅ ⬆ ⬇ ⬈ ⬉ ⬊ ⬋ ⬚ ← ↑ → ↓ ↗ ↖ ↘ ↙ ⬛ ⬣
                case NULL_MAP: fprintf ( fp, " ⬚" ); break;
                case 1: fprintf ( fp, " ↖" ); break;
                case 2: fprintf ( fp, " ↑" ); break;
                case 3: fprintf ( fp, " ↗" ); break;
                case 4: fprintf ( fp, " ←" ); break;
                case 5: fprintf ( fp, " ⬣" ); break;
                case 6: fprintf ( fp, " →" ); break;
                case 7: fprintf ( fp, " ↙" ); break;
                case 8: fprintf ( fp, " ↓" ); break;
                case 9: fprintf ( fp, " ↘" ); break;
            }
        }
        fprintf ( fp, "\n" );
    }
    fclose(fp);
    return 0;
}

