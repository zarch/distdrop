#include <stdio.h>
#include <stdlib.h>
#include "drop.h"


#define M_SQRT2        1.41421356237309504880  /* sqrt(2) */
#define NULL_MAP 0
#define NULL_NEIG -999
#define INPUT_DOMAIN 10000.0f
#define INPUT_NOT_DOMAIN -1.0f
#define INPUT_ROAD 0.0f


/*
 *
 * DEFINE MOVEMENTS
 *
 */
/* Movement & Direction
 * -1 0 1
 * |1|2|3| -1
 * |4| |6|  0
 * |7|8|9|  1
 */
move movements[NMV] =
{
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

// utf8 symbols: ⬅ ⬆ ⬇ ⬈ ⬉ ⬊ ⬋ ⬚ ← ↑ → ↓ ↗ ↖ ↘ ↙ ⬛ ⬣
int print_dir ( short int **map, int nrows, int ncols )
{
    for ( int row = 0; row < nrows; row++ )
    {
        printf("%d: ", row);
        for ( int col = 0; col < ncols; col++ )
        {
            switch ( map[row][col] )
            {
                case 0: printf ( "⬚" ); break;
                case 1: printf ( "↖" ); break;
                case 2: printf ( "↑" ); break;
                case 3: printf ( "↗" ); break;
                case 4: printf ( "←" ); break;
                case 6: printf ( "→" ); break;
                case 7: printf ( "↙" ); break;
                case 8: printf ( "↓" ); break;
                case 9: printf ( "↘" ); break;
                case -1: printf ( "⬣" ); break;
            }
        }
        printf ( "\n" );
    }
    return 0;
}



int print_array ( void *map, int type, int nrows, int ncols )
{
    short **sa = ( short ** ) map;
    int **ia = ( int ** ) map;
    float **fa = ( float ** ) map;
    double **da = ( double ** ) map;

    for ( int row = 0; row < nrows; row++ )
    {
        printf("%d: ", row);
        for ( int col = 0; col < ncols; col++ )
        {
            if ( type == TYPE_SHORT )
                printf ( " % i", sa[row][col] );
            if ( type == TYPE_INT )
                printf ( " % i", ia[row][col] );
            if ( type == TYPE_FLOAT )
                printf ( " % 4.2f", fa[row][col] );
            if ( type == TYPE_DOUBLE )
                printf ( " % 4.2f", da[row][col] );
        }
                printf ( "\n" );
    }
    return 0;
}

int **new_int_map (int *nrows, int *ncols, int* set_number )
{
    int    **new_map = ( int**    ) malloc ( *nrows * sizeof ( int *    ) );
    if ( new_map == NULL )
    {
        fprintf ( stderr, "Unable allocate memory" );
        exit ( 1 );
    }

    for ( int i = 0; i < *nrows; i++ )
    {
        //new_map[i] = ( short* ) calloc ( *ncols, sizeof ( short ) );
        new_map[i] = ( int* ) malloc ( *ncols * sizeof ( int ) );
        if ( new_map[i] == NULL )
        {
            fprintf ( stderr, "Unable allocate memory" );
                      exit ( 1 );
        }
        if ( set_number != NULL )
        {
            for ( int j = 0; j < *ncols; j++ )
            {
                new_map[i] = set_number;
            }
        }
    }
    return new_map;

}

short **new_short_map ( int *nrows, int *ncols )
{
    //short **new_map = ( short** ) calloc ( *nrows, sizeof ( short * ) );
    short **new_map = ( short** ) malloc ( *nrows * sizeof ( short * ) );

    if ( new_map == NULL )
    {
        fprintf ( stderr, "Unable allocate memory" );
        exit ( 1 );
    }

    for ( int i = 0; i < *nrows; i++ )
    {
        //new_map[i] = ( short* ) calloc ( *ncols, sizeof ( short ) );
        new_map[i] = ( short* ) malloc ( *ncols * sizeof ( short ) );
        if ( new_map[i] == NULL )
        {
            fprintf ( stderr, "Unable allocate memory" );
                      exit ( 1 );
        }
    }
    return new_map;
}

float **new_float_map ( int *nrows, int *ncols )
{
    //float **new_map = ( float** ) calloc ( *nrows, sizeof ( float * ) );
    float **new_map = ( float** ) malloc ( *nrows * sizeof ( float * ) );

    if ( new_map == NULL )
    {
        fprintf ( stderr, "Unable allocate memory" );
        exit ( 1 );
    }

    for ( int i = 0; i < *nrows; i++ )
    {
    //new_map[i] = ( float* ) calloc ( *ncols, sizeof ( float ) );
        new_map[i] = ( float* ) malloc ( *ncols * sizeof ( float ) );
        if ( new_map[i] == NULL )
        {
            fprintf ( stderr, "Unable allocate memory" );
                      exit ( 1 );
        }
    }
    return new_map;
}




float **get_input_map ( short **road, short **domain, int *nrows, int *ncols )
{
    //
    // Merge road and domain in one map
    //
    float **input = new_float_map ( nrows, ncols );

    for ( int i = 0; i < *nrows; i++ )
    {
        /* get row */
        short *r_road = road[i];
        short *r_domain = domain[i];
        float *r_input = input[i];

        for ( int j = 0; j < *ncols; j++ )
        {
            r_input[j] = r_domain[j] != NULL_MAP ? INPUT_DOMAIN : INPUT_NOT_DOMAIN ;
            if ( r_road[j] != NULL_MAP )
            {
                r_input[j] = INPUT_ROAD;
            }
        }
    }
    return input;
}

float **update_float_cache ( int *row, int *last_row, int *nrows,
                             float **cache, float **map )
{
    /* TODO: save cache[0] into grass and free */
    if ( *row - *last_row == 1 )
    {
        if ( *row > 1 && *row < *nrows-1 )
        {
            //map[*last_row - 1] = cache[0];
            cache[0] = cache[1];
            cache[1] = cache[2];
            cache[2] = map[*row + 1]; // read row using get_row()
        }
        else if ( *row <= 1 )
        {
            cache[0] = *row == 0 ?  map[*row]: map[*row -1];
            cache[1] = map[*row];
            cache[2] = map[*row + 1];
        }
        else
        {
            // map[*last_row - 1] = cache[0];
            cache[0] = cache[1];
            cache[1] = map[*row]; // read the last
            cache[2] = map[*row]; //NULL; // TODO: put the NULL pointer?
        }
    }
    else
    {
        // save
        //map[*last_row - 1] = cache[0];
        //map[*last_row] = cache[1];
        //map[*last_row + 1] = cache[2];
        // read and load map row to the new cache
        cache[0] = *row == 0? map[*row] : map[*row -1];
        cache[1] = map[*row];
        cache[2] = *row == *nrows ? map[*row] : map[*row + 1];
    }
    //printf("Final cache, row: %d:\n", *row);
    //print_array(cache, TYPE_FLOAT, *nrows, 11);
    return cache;
}

short **update_short_cache ( int *row, int *last_row, int *nrows,
                             short **cache, short **map )
{
    /* TODO: save cache[0] into grass and free */
    if ( *row - *last_row == 1 )
    {
        if ( *row > 1 || *row < *nrows-1 )
        {
            //map[*last_row - 1] = cache[0];
            cache[0] = cache[1];
            cache[1] = cache[2];
            cache[2] = map[*row + 1];
        }
        else if ( *row <= 1 )
        {
            cache[0] = *row == 0 ? map[*row] : map[*row -1];
            cache[1] = map[*row];
            cache[2] = map[*row + 1];
        }
        else
        {
            //map[*row - 2] = cache[0];
            cache[0] = cache[1];
            cache[1] = map[*row];
            cache[2] = map[*row];
        }
    }
    else
    {
        // save
        //map[*last_row - 1] = cache[0];
        //map[*last_row] = cache[1];
        //map[*last_row + 1] = cache[2];
        // load map row to the new cache
        cache[0] = map[*row -1];
        cache[1] = map[*row];
        cache[2] = map[*row + 1];
    }
    return cache;
}



int *get_neighbours ( int *px, int *py, move *movements, int **neighbours,
                      int *nrows, int *ncols )
{
    /*
     * Function that return a pointer to a neighbours array
     *
     */
    for ( int row = 0; row < ( int )sizeof ( movements ); row++ )
    {
        // compute virtual row and col
        int vrow = *px + movements[row].mv[0];
        int vcol = *py + movements[row].mv[1];
        // Are neighbours in the region?
        if ( vrow<0  || vcol<0 || vrow>=*nrows || vcol>=*ncols )
        {
            // set neighbour value to NULL
            neighbours[row][0] = NULL_NEIG;
            neighbours[row][1] = NULL_NEIG;
        }
        else
        {
            // set neighbour value
            // I don't use vrow, because I use cache
            // transform row from -1, 0, 1 to 0, 1, 2
            neighbours[row][0] = movements[row].mv[0] + 1;
            neighbours[row][1] = vcol;
        }
    //printf ( "nx:%d ny:%d\n", neighbours[row][0], neighbours[row][1]);
    }
    return 0;
}


int all_pixel ( move   *movements,
                short  *redo_rows,
                float  **rdist,
                float  **elevation,
                short  **rdir,
                float  **rdrop_up,
                float  **rdrop_dw,
                int nrows,
                int ncols )
{
    int row_cache=3, nx, ny, count = 0, last_row=-1;
    float new_dist, drop;

    /* initialize cache, input and output cache */
    float **elev_cache = new_float_map ( &row_cache, &ncols );
    float **dist_cache = new_float_map ( &row_cache, &ncols );
    short **dir_cache = new_short_map ( &row_cache, &ncols );
    float **up_cache = new_float_map ( &row_cache, &ncols );
    float **dw_cache = new_float_map ( &row_cache, &ncols );

    // Initialize neighbours
    int r=8,c=2;
    int **neighbours = new_int_map ( &r, &c, NULL );

    for ( int i = 0; i < nrows; i++ )
    {
        /* check if some pixel in the row has been changed */
        if ( redo_rows[i] == 1 )
        {
            /* pixels inside the row changed, therefore we compute the row */
            redo_rows[i] = 0;
            /* update cache */
            dist_cache = update_float_cache ( &i, &last_row, &nrows, dist_cache, rdist );
            elev_cache = update_float_cache ( &i, &last_row, &nrows, elev_cache, elevation );
            dir_cache = update_short_cache ( &i, &last_row, &nrows, dir_cache, rdir );
            up_cache = update_float_cache ( &i, &last_row, &nrows, up_cache, rdrop_up );
            dw_cache = update_float_cache ( &i, &last_row, &nrows, dw_cache, rdrop_dw );
            last_row = i;

            for ( int j = 0; j < ncols; j++ )
            {
                if ( dist_cache[1][j] >= 0 )
                {
                    get_neighbours ( &i, &j, movements, neighbours, &nrows, &ncols );
                    for ( int n = 0; n < NMV ; n++ )
                    {
                        // TODO: after check if there are performce consegunece to declaire here or not
                        nx = neighbours[n][0];
                        ny = neighbours[n][1];
                        // check if is road, and set direction to -1
                        dir_cache[1][j] = dist_cache[1][j] == 0 ? -1:dir_cache[1][j];
                        //Check if neighbours are inside the region AND the domain
                        if ( nx != NULL_NEIG && dist_cache[nx][ny]>0 )
                        {
                            new_dist = dist_cache[1][j] + movements[n].dist;
                            if ( dist_cache[nx][ny] > new_dist )
                            {
                                // assign the smaller one
                                dist_cache[nx][ny] = new_dist;
                                dir_cache[nx][ny] = movements[n].dir;

                                //add_point_to_array_of_list(nx, ny, origin_list);

                                //check if drop is positive or negative
                                drop = elev_cache[nx][ny] - elev_cache[1][j];
                                if ( drop >= 0 )
                                {
                                    up_cache[nx][ny] = up_cache[1][j] + drop;
                                    dw_cache[nx][ny] = dw_cache[1][j];
                                }
                                else
                                {
                                    up_cache[nx][ny] = up_cache[1][j];
                                    dw_cache[nx][ny] = dw_cache[1][j] + drop;
                                }
                                redo_rows[i + nx - 1] = 1;
                                count += 1;
                            }
                        }
                    }
                }
            }
            //printf("--------------------------------\n");
            //printf("dist_cache, row: %d\n", i);
            //print_array(dist_cache, TYPE_FLOAT, 3, 11);
        }
    }
    //free cache //
    free ( elev_cache );
    free ( dist_cache );
    free ( dir_cache );
    free ( up_cache );
    free ( dw_cache );
    return count;
}



int distdrop ( move   *movements,
               float  **rdist,
               float  **elevation,
               float  **rdrop_up,
               float  **rdrop_dw,
               short  **rdir,
               int nrows,
               int ncols )
{
    int all_done = 1;

    // init redo_rows
    short *redo_rows = ( short* ) malloc ( nrows * sizeof ( short ) );

    if ( redo_rows == NULL )
    {
        fprintf ( stderr, "Unable allocate memory" );
        exit ( 1 );
    }
    for ( int i = 0; i < nrows; i++ )
        redo_rows[i] = 1;


    while ( all_done ) // if all_done != 0? continue: break
    {
        printf("inside the while: %d:\n", all_done);
        all_done = all_pixel ( movements, redo_rows, rdist, elevation,
                               rdir, rdrop_up, rdrop_dw,
                               nrows, ncols );
    }
    //printf("--rdist--\n");
    //print_array(rdist, TYPE_FLOAT, nrows, ncols);
    return 0;
}

