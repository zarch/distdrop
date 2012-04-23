#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "test.h"

/* global */
int i_prev, i_row, i_next, neighbours[NMV][2];
short *p_row[NCOLS], *c_row[NCOLS], *n_row[NCOLS];



list * get_not_null ( short int ( *map ) [NCOLS] )
{
    /*
     * Return a pointer to a list of couple (x, y) with not null values
     *
     *
     * Look here for Array pointers and functions:
     *
     * https://gist.github.com/2419722
     *
     */
    unsigned int row, col;
    printf ( "Create an empty list\n" );
    list *not_null = create_empty_list();

    /* Analyse each cell in the map */
    for ( row = 0; row < NROWS; row++ )
    {
        /* G_percent ( row, window.rows, 10 ); */
        for ( col = 0; col < NCOLS; col++ )
        {
            /* printf("pixel: %d,%d; map value: %d\n", \
             *        row, col, map[row][col]); */
            if ( map[row][col] != NULL_MAP ) /* Rast_is_null_value( ) */
            {
                /* add to a list */
                printf("not_null, row: %d, col: %d", row, col);
                add_point_to_order_list ( row, col, not_null );
            }
        }
    }
    return not_null;
}

int print_array_short( short int ( *map ) [NCOLS] )
{
    unsigned int row, col;
    for ( row = 0; row < NROWS; row++ )
    {
        for ( col = 0; col < NCOLS; col++ )
        {
            printf(" %d", map[row][col]);
        }
        printf("\n");
    }
    return 0;
}

int print_array_double( double ( *map ) [NCOLS] )
{
    unsigned int row, col;
    for ( row = 0; row < NROWS; row++ )
    {
        for ( col = 0; col < NCOLS; col++ )
        {
            printf(" %.2f", map[row][col]);
        }
        printf("\n");
    }
    return 0;
}

int print_array_float( float ( *map ) [NCOLS] )
{
    unsigned int row, col;
    for ( row = 0; row < NROWS; row++ )
    {
        for ( col = 0; col < NCOLS; col++ )
        {
            printf(" %.2f", map[row][col]);
        }
        printf("\n");
    }
    return 0;
}


int up_neighbours ( int px, int py )
{
    /*
     * Function that return a pointer to a neighbours array
     *
     */
    int row;
    /* printf ( "pixel: row=%d col=%d\n", px, py); */
    for ( row = 0; row < sizeof(movements) / sizeof(movements[0]); row++ )
    {
        neighbours[row][0] = px + movements[row].mv[0];
        neighbours[row][1] = py + movements[row].mv[1];
        /* Are neighbours in the region? */
        if ( neighbours[row][0]<0     || neighbours[row][1]<0 ||
                neighbours[row][0]>=NROWS || neighbours[row][1]>=NCOLS )
        {
            /*set neighbour value to NULL */
            neighbours[row][0] = NULL_NEIG;
            neighbours[row][1] = NULL_NEIG;
        }
        /* printf ( "nx:%d ny:%d\n", neighbours[row][0], neighbours[row][1]); */
    }
    return 1;
}

int up_row(int i, short ( *map ) [NCOLS])
{
    /* update index */
    i_prev = i_row - 1;
    i_row  = i;
    i_next = i_row + 1;
    /* load rows  */
    p_row[NCOLS] = map[i_prev];
    c_row[NCOLS]   = map[i_row];
    n_row[NCOLS] = map[i_next];
    return 0;
}

int prove( short ( *map ) [NCOLS] )
{
    short col;
    short *ndomain;
    ndomain = domain[1];
    double *ndist;
    ndist = rdist[1];
    printf ( "DOMAIN\n");
    for ( col = 0; col < NCOLS; col++ )
    {
        printf ( "col = %d; val = %d\n", col, ndomain[col]);
    }
    printf ( "DIST\n");
    for ( col = 0; col < NCOLS; col++ )
    {
        printf ( "col = %d; val = %f\n", col, ndist[col]);
    }
    return 0;
}


int execute ( list *points )
{
    int i, n, nx, ny, px, py, n_last, p_last;
    short value;
    elem *e, *p;
    /* define row maps for neighbours and pixel*/
    short *ndomain, *nroad, *ndir, *nnot_used;
    short *pdomain, *proad, *pdir, *pnot_used;
    float *nelev, *pelev, drop;
    float *ndrop_up, *ndrop_dw, *pdrop_up, *pdrop_dw;
    double *ndist, *pdist, new_dist;

    list *origin_list = create_empty_list();

    n_last = -999;
    p_last = -999;
    for ( i = 0, e = points->first; i < points->length; i++ )
    {
        /* over write row and col of the pixel */
        px = e->point.row;
        py = e->point.col;
        printf ( "--------------------------------------------------------\n");
        printf ( "pixel: px=%d py=%d\n", px, py);
        /* check if pixel row change */
        if (px != p_last)
            {
                p_last = px;
                /* using grass function `get_row`
                * load row of different maps */
                proad = road[p_last];
                pdomain = domain[p_last];
                pdist = rdist[p_last];
                pdir = rdir[p_last];
                pnot_used = not_used[p_last];
                pelev = elevation[p_last];
                pdrop_up = rdrop_up[p_last];
                pdrop_dw = rdrop_dw[p_last];
            }
        /* update global variable of pixel: `neighbours` */
        pnot_used[py] = 0;
        up_neighbours ( px, py );
        for ( n = 0; n < NMV ; n++ )
        {
            nx = neighbours[n][0];
            ny = neighbours[n][1];
            /* Check if neighbours are valid*/
            if (nx != NULL_NEIG)
            {
                /* check if is a new row */
                if (nx != n_last)
                {
                    n_last = nx;
                    /* using grass function `get_row`
                     * load row of different maps */
                    nroad = road[n_last];
                    ndomain = domain[n_last];
                    ndist = rdist[n_last];
                    ndir = rdir[n_last];
                    nnot_used = not_used[n_last];
                    nelev = elevation[p_last];
                    ndrop_up = rdrop_up[n_last];
                    ndrop_dw = rdrop_dw[n_last];
                }
                
                /* check if we are inside the domain */
                if ((ndomain[ny] != NULL_MAP) && (nnot_used[ny] != 0))
                {
                    printf ( "===================================\n");
                    printf ( "nixel: nx=%d ny=%d\n", nx, ny);
                    printf ( "ndomain[ny]: %d\n", ndomain[ny]);
                    printf ( "ndist[ny]: %f\n", ndist[ny]);
                    printf ( "nnot_used[ny]: %d\n", nnot_used[ny]);
                    if (ndist[ny] != NULL_MAP)
                    {
                        /* distance already compute check the smaller one */
                        printf ( "pdist[py]: %f, mv= %f\n", pdist[py], movements[n].dist);
                        new_dist = pdist[py] + movements[n].dist;
                        printf ( "ew_dist: %f => %f\n", new_dist, pdist[py] + movements[n].dist );
                        if (ndist[ny] > new_dist)
                        {
                            /* assign the smaller one */
                            ndist[ny] = new_dist;
                            ndir[ny] = movements[n].dir;
                            nnot_used[ny] = 1;
                            add_point_to_order_list(nx, ny, origin_list);
                            drop = nelev[ny] - pelev[py];
                            printf ( "drop: %f; new_dist: %f\n", drop, ndist[ny]);
                            /* check if drop is positive or negative */
                            if (drop >= 0)
                            {
                                ndrop_up[ny] = pdrop_up[py] + drop;
                                ndrop_dw[ny] = pdrop_dw[py];
                            }
                            else
                            {
                                ndrop_up[ny] = pdrop_up[py];
                                ndrop_dw[ny] = pdrop_dw[py] + drop;
                            }
                        }
                    }
                    else
                    {
                        /* compute distance */
                        ndist[ny] = pdist[py] + movements[n].dist;
                        ndir[ny]   = movements[n].dir;
                        drop = nelev[ny] - pelev[py];
                        printf ( "drop: %f; new_dist: %f\n", drop, ndist[ny]);
                        /* check if drop is positive or negative */
                        if (drop >= 0)
                        {
                            ndrop_up[ny] = pdrop_up[py] + drop;
                            ndrop_dw[ny] = pdrop_dw[py];
                        }
                        else
                        {
                            ndrop_up[ny] = pdrop_up[py];
                            ndrop_dw[ny] = pdrop_dw[py] + drop;
                        }
                        /* check if is already compute */
                        if (nnot_used[ny])
                        {
                            add_point_to_order_list(nx, ny, origin_list);
                        }
                    }
                }
            }
            /* */
        }
        printf ( "FINISH ELEMENT\n");
        p = e;
        e = e->next;
        free(p);
        printf ( "fatto...\n");
    }
    if (origin_list->length != 0 )
    {
        printf ( "#######################################################\n");
        printf ( "FINISH LIST\n");
        execute( origin_list );
    }
    return 0;
}
