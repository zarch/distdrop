#include <stdio.h>
#include <stdlib.h>
#include "ascii.h"
#include "list.h"
#include "drop.h"

/*
 * check the code with:
 *
 * $ clang main.c -fsyntax-only
 * $ gcc -Wall -pedantic -std=c11 -c main.c
 * $ gcc -Wall -pedantic -std=c99 -c main.c
 * $ valgrind --leak-check=full --show-reachable=yes ./distdrop
 *
 *
 */

static void test_distdrop_point_line_elev2();
static int test_list();

static int test_list()
{
    list *l = create_empty_list();
    populate_inverse_rows_cols ( 10, 3, l );
    print_list ( l );
    free_list ( l );

    /* test the function to add element in a order list */
    printf ( "\nTest the function to add element in a order list\n\n" );

    list *o = create_empty_list();
    order_populate_inverse_rows_cols ( 10, 3, o );
    print_list ( o );
    free_list ( o );
    return 0;
}

static void test_distdrop_point_line_elev2()
{
    int nrows, ncols;

    /* Read data */
    short **point = read_short_data("points.txt", &nrows, &ncols);
    short **line = read_short_data("lines.txt", &nrows, &ncols);
    float **elev = read_float_data("elev.txt", &nrows, &ncols);

    printf ( "\n\n\nTest distdrop: line-area-elev2\n" );
    printf ( "===================================\n\n" );
    printf ( "\n\n    Print points...\n" );
    print_array( point, TYPE_SHORT, nrows, ncols);
    printf ( "\n\n    Print lines...\n" );
    print_array(line, TYPE_SHORT, nrows, ncols);
    printf ( "\n\n    Print elevation...\n" );
    print_array(elev, TYPE_FLOAT, nrows, ncols);

    short **rdir = read_short_data("null.txt", &nrows, &ncols);
    float **rdist = read_float_data("null.txt", &nrows, &ncols);
    float **rdrop_up = read_float_data("null.txt", &nrows, &ncols);
    float **rdrop_dw = read_float_data("null.txt", &nrows, &ncols);
    short **not_used = read_short_data("ones.txt", &nrows, &ncols);

    distdrop( movements, point, line, elev,
              rdist, rdir, not_used, rdrop_up, rdrop_dw, nrows, ncols );

    printf ( "\n\n    Print rdir...\n" );
    print_dir(rdir, nrows, ncols);
    printf ( "\n\n    Print rdist...\n" );
    print_array(rdist, TYPE_FLOAT, nrows, ncols);
    printf ( "\n\n    Print rdrop_up...\n" );
    print_array(rdrop_up, TYPE_FLOAT, nrows, ncols );
    printf ( "\n\n    Print rdrop_dw...\n" );
    print_array(rdrop_dw, TYPE_FLOAT, nrows, ncols);
}

int main()
{
    /*test_list();*/
    test_distdrop_point_line_elev2();
    return 0;
}

