
/****************************************************************************
 *
 * MODULE:       r.example
 * AUTHOR(S):    Pietro Zambelli - s/ /./ pietro zambelli at ing uni tn it
 *               with hints from: Sören Gebbert - soerengebbert at googlemail com
 * PURPOSE:      Compute the distance and the elevation different of a pixel,
 *               between the pixel and the closest road points
 *
 * COPYRIGHT:    (C) 2002, 2012-2012 by the GRASS Development Team
 *
 *               This program is free software under the GNU General Public
 *               License (>=v2). Read the file COPYING that comes with GRASS
 *               for details.
 *
 *****************************************************************************/

/*#include <grass/gis.h>
#include <grass/raster.h>
#include <grass/glocale.h>
*/

/*
 * global function declaration
 */
/*extern CELL f_c(CELL);
extern FCELL f_f(FCELL);
extern DCELL f_d(DCELL);
*/

#include <stdio.h>
#include <stdlib.h>
#include "ascii.h"
#include "queue.h"
#include "drop.h"

/*
 * check the code with:
 *
 * $ clang main.c -fsyntax-only
 * $ gcc -Wall -pedantic -std=c11 -c main.c
 * $ gcc -Wall -pedantic -std=c99 -c main.c
 * $ gcc -O -Wall -W -pedantic -ansi -std=c99 -o pixel_by_pixel pixel_by_pixe
 * $ valgrind --leak-check=full --show-reachable=yes ./distdrop
 *
 *
 */

static void test_distdrop();
static int test_array_of_queue();



static int test_array_of_queue()
{
    int nrows = 10;
    queue **a = create_empty_array_of_queue( nrows );
    array_append(0, 0, a);
    array_append(0, 1, a);
    array_append(0, 2, a);
    array_append(1, 0, a);
    array_append(1, 1, a);
    array_append(1, 3, a);
    print_array_of_queue( a, nrows );
    printf("\n\nlen %d\n", a[0]->length);
    elem *el = pop(a[0]);
    printf("\n\npop: row:0 el_row:%d, el_col:%d\n\n", el->point.row, el->point.col);
    printf("\n\nlen %d\n", a[0]->length);

    printf("\n\nlen %d\n", a[0]->length);
    el = pop(a[0]);
    printf("\n\npop: row:0 el_row:%d, el_col:%d\n\n", el->point.row, el->point.col);
    printf("\n\nlen %d\n", a[0]->length);

    printf("\n\nlen %d\n", a[0]->length);
    el = pop(a[0]);
    printf("\n\npop: row:0 el_row:%d, el_col:%d\n\n", el->point.row, el->point.col);
    printf("\n\nlen %d\n", a[0]->length);

    print_array_of_queue( a, nrows );
    queue *rows = get_row_not_null(a, nrows);
    print_queue ( rows );
    return 0;
}



static void test_distdrop(char *road, char *domain, char *elevation)
{
    int nrows, ncols;

    // Read data
    short **point = read_short_data(road, &nrows, &ncols);
    short **line = read_short_data(domain, &nrows, &ncols);
    float **elev = read_float_data(elevation, &nrows, &ncols);

    printf ( "\n\n\nTest distdrop: %s - %s - %s\n", road, domain, elevation );
    printf ( "==========================================================\n\n" );
    printf ( "\n\n    Print %s...\n", road );
    print_array( point, TYPE_SHORT, nrows, ncols);
    printf ( "\n\n    Print %s...\n", domain );
    print_array(line, TYPE_SHORT, nrows, ncols);
    printf ( "\n\n    Print %s...\n", elevation );
    print_array(elev, TYPE_FLOAT, nrows, ncols);

    // Initialize empty array
    short **rdir = read_short_data("null.txt", &nrows, &ncols);
    // merge road and domain map
    float **rdist = get_input_map ( point, line, &nrows, &ncols );
    float **rdrop_up = read_float_data("null.txt", &nrows, &ncols);
    float **rdrop_dw = read_float_data("null.txt", &nrows, &ncols);
    //short **not_used = read_short_data("ones.txt", &nrows, &ncols);

    //distdrop ( movements, rdist, elev, rdrop_up, rdrop_dw,
    //           rdir, nrows, ncols );
    distdrop_queue ( movements, rdist, elev, rdrop_up, rdrop_dw,
                    rdir, nrows, ncols );
    printf("\n\n");

    print_array(rdir, TYPE_SHORT, nrows, ncols);
    printf ( "\n\n    Print rdir...\n" );
    print_dir(rdir, nrows, ncols);
    printf ( "\n\n    Print rdist...\n" );
    print_array(rdist, TYPE_FLOAT, nrows, ncols);
    printf ( "\n\n    Print rdrop_up...\n" );
    print_array(rdrop_up, TYPE_FLOAT, nrows, ncols );
    printf ( "\n\n    Print rdrop_dw...\n" );
    print_array(rdrop_dw, TYPE_FLOAT, nrows, ncols);

    // free memory
    free(point);
    free(line);
    free(elev);
    free(rdir);
    free(rdist);
    free(rdrop_up);
    free(rdrop_dw);
}


int main()
{
    //test_queue();
    test_array_of_queue();

    // test distdrop algorithm
    //             road         domain       elevation
    test_distdrop("points.txt", "lines.txt",  "elev.txt");
    test_distdrop("points.txt", "circle.txt", "elev.txt");
    test_distdrop("points.txt", "area.txt",   "elev.txt");

    test_distdrop("lines.txt",  "lines.txt",  "elev.txt");
    test_distdrop("lines.txt",  "area.txt",   "elev.txt");

    return 0;
}

