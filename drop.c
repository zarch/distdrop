#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "drop.h"
# define M_SQRT2        1.41421356237309504880  /* sqrt(2) */


#define NULL_MAP 0
#define NULL_NEIG 0

/*
 *
 * DEFINE MOVEMENTS
 *
 */
/* Movement & Direction
 -1 0 1
 |1|2|3| -1
 |4| |6|  0
 |7|8|9|  1
*/
move movements[NMV] = {
    /*  mv  |dir|dist          */
    {{ 1,  1}, 1, M_SQRT2},
    {{ 1,  0}, 2, 1},
    {{ 1, -1}, 3, M_SQRT2},
    {{ 0,  1}, 4, 1},
    {{ 0, -1}, 6, 1},
    {{-1,  1}, 7, M_SQRT2},
    {{-1,  0}, 8, 1},
    {{-1, -1}, 9, M_SQRT2},
};

int neighbours[NMV][2];

list * get_not_null ( short int **map, int nrows, int ncols)
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
    /* printf ( "Create an empty list\n" ); */
    list *not_null = create_empty_list();

    /* Analyse each cell in the map */
    for ( row = 0; row < nrows; row++ )
    {
        /* G_percent ( row, window.rows, 10 ); */
        for ( col = 0; col < ncols; col++ )
        {
            /* printf("pixel: %d,%d; map value: %d\n", \
             *        row, col, map[row][col]); */
            if ( map[row][col] != NULL_MAP ) /* Rast_is_null_value( ) */
            {
                /* add to a list */
                /* printf("not_null, row: %d, col: %d", row, col); */
                add_point_to_order_list ( row, col, not_null );
            }
        }
    }
    return not_null;
}

int print_dir( short int **map, int nrows, int ncols)
{
    unsigned int row, col;
    for ( row = 0; row < nrows; row++ )
    {
        for ( col = 0; col < ncols; col++ )
        {
            switch(map[row][col])
            {
                case 0: printf(" ░ "); break;
                case 1: printf(" ^\\"); break;
                case 2: printf(" ↑ "); break;
                case 3: printf(" /^"); break;
                case 4: printf(" ← "); break;
                case 6: printf(" → "); break;
                case 7: printf(" _/"); break;
                case 8: printf(" ↓ "); break;
                case 9: printf(" \\_"); break;
                case -1: printf(" ○ "); break;
            }
        }
        printf("\n");
    }
    return 0;
}

int print_array(void *map, int type, int nrows, int ncols)
{
    short **sa = (short **)map;
    int **ia = (int **)map;
    float **fa = (float **)map;
    double **da = (double **)map;

    unsigned int row, col;

    for ( row = 0; row < nrows; row++ )
    {
        for ( col = 0; col < ncols; col++ )
        {
            if(type == TYPE_SHORT)
                printf(" %i", sa[row][col]);
            if(type == TYPE_INT)
                printf(" %i", ia[row][col]);
            if(type == TYPE_FLOAT)
                printf(" %.2f", fa[row][col]);
            if(type == TYPE_DOUBLE)
                printf(" %.2f", da[row][col]);
        }
        printf("\n");
    }
    return 0;
}


int up_neighbours ( int px, int py, move *movements, int nrows, int ncols)
{
    /*
     * Function that return a pointer to a neighbours array
     *
     */
    int row;
    /* printf ( "px:%d py:%d\n", px, py); */
    for ( row = 0; row < sizeof(movements); row++ )
    {
        neighbours[row][0] = px + movements[row].mv[0];
        neighbours[row][1] = py + movements[row].mv[1];
        /* Are neighbours in the region? */
        if ( neighbours[row][0]<0     || neighbours[row][1]<0 ||
                neighbours[row][0]>=nrows || neighbours[row][1]>=ncols )
        {
            /*set neighbour value to NULL */
            neighbours[row][0] = NULL_NEIG;
            neighbours[row][1] = NULL_NEIG;
        }
        /* printf ( "nx:%d ny:%d\n", neighbours[row][0], neighbours[row][1]); */
    }
    return 0;
}

int execute ( move   *movements,
              list   *points,
              short  **road,
              short  **domain,
              float  **elevation,
              float **rdist,
              short  **rdir,
              short  **not_used,
              float  **rdrop_up,
              float  **rdrop_dw,
              int nrows,
              int ncols)
{
    int i, n, nx, ny, px, py, n_last, p_last;
    /* short value; */
    elem *e, *p;
    /* define row maps for neighbours and pixel*/
    short *ndomain, *ndir, *nnot_used, *nroad;
    short *proad, *pdir;
    short *pnot_used;
    float *nelev, *pelev, drop;
    float *ndrop_up, *ndrop_dw, *pdrop_up, *pdrop_dw;
    float *ndist, *pdist, new_dist;

    list *origin_list = create_empty_list();
    /* initialize the row */
    n_last = -999;
    p_last = -999;
    /* start the cycle for each point in the list */
    for ( i = 0, e = points->first; i < points->length; i++ )
    {
        /* over write row and col of the pixel */
        px = e->point.row;
        py = e->point.col;
        /* printf("\n\n====================\n");  */
        /* printf("px: %d, py: %d\n", px, py);    */

        /* check if pixel row change */
        if (px != p_last)
        {
            p_last = px;
            /* using grass function `get_row`
            * load row of different maps */
            proad = road[p_last];
            /* pdomain = domain[p_last]; */
            pdist = rdist[p_last];
            pdir = rdir[p_last];
            pnot_used = not_used[p_last];
            pelev = elevation[p_last];
            pdrop_up = rdrop_up[p_last];
            pdrop_dw = rdrop_dw[p_last];
        }
        
        pnot_used[py] = 0;
        if (proad[py] == 1){ pdir[py] = -1;}

        /* update global variable of pixel: `neighbours` */
        up_neighbours ( px, py, movements, nrows, ncols );
        for ( n = 0; n < NMV ; n++ )
        {
            nx = neighbours[n][0];
            ny = neighbours[n][1];
            /* Check if neighbours are valid*/
            if (nx != NULL_NEIG)
            {
                /* printf("    nx: %d, ny: %d\n", nx, ny); */
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
                if ((ndomain[ny] != NULL_MAP) && (nnot_used[ny] != 0) && nroad[ny] != 1)
                {
                    if (ndist[ny] != NULL_MAP)
                    {
                        /* distance already compute check the smaller one */
                        new_dist = pdist[py] + movements[n].dist;
                        if (ndist[ny] > new_dist)
                        {
                            /* assign the smaller one */
                            /* printf("new_dist: %f", new_dist); */
                            ndist[ny] = new_dist;
                            ndir[ny] = movements[n].dir;
                            nnot_used[ny] = 1;
                            add_point_to_order_list(nx, ny, origin_list);
                            drop = nelev[ny] - pelev[py];
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
                    /* printf("ndist[ny]: %f", ndist[ny]); */
                }
            }
        }
        p = e;
        e = e->next;
        free(p);
    }
    if (origin_list->length != 0 )
    {
        execute( movements, origin_list, road, domain, elevation, rdist, rdir,
                 not_used, rdrop_up, rdrop_dw, nrows, ncols );
    }
    return 0;
}

int distdrop(move   *movements,
             short  **road,
             short  **domain,
             float  **elevation,
             float **rdist,
             short  **rdir,
             short  **not_used,
             float  **rdrop_up,
             float  **rdrop_dw,
             int nrows,
             int ncols)
{
    /* printf ( "    Get not null...\n" ); */
    list *road_points = get_not_null ( road , nrows, ncols);
    execute( movements, road_points, road, domain, elevation,
             rdist, rdir, not_used, rdrop_up, rdrop_dw, nrows, ncols);
    return 0;
}
