#include <stdio.h>
#include "list.h"
#include "test.h"

list*
get_not_null( short int **map )
{
    /*
     * Return a pointer to a list of couple (x, y) with not null values
     */
    unsigned int row;
    unsigned int col;
    printf("Create an empty list\n");
    list *not_null = create_empty_list();

    /* Analyse each cell in the map */
    for ( row = 0; row < NROWS; row++ )
    {
      printf("for row: %d\n", row);
        /* G_percent ( row, window.rows, 10 ); */
        for ( col = 0; col < NCOLS; col++ )
          printf("pixel: %d; value: %d", col, map[row][col]);
            /*int Rast_is_null_value(const void *rast, RASTER_MAP_TYPE data_type) */
            if (map[row][col] != NULL_MAP) /* Rast_is_null_value( ) */
            {
                /* add to a list */
                add_point_to_order_list(row, col, not_null);
            }
    }
    return not_null;
}