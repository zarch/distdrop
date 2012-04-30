#include <stdio.h>
#include "list.h"
#include "test.h"
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


void checktest ( int result )
{
    /* Interrupt the execution if one of test failed */
    if ( result != 0 ) printf ( "\n\n Test Failed!\n\n\n" ); /* TODO: INTERRUPT*/;
}

int test_list()
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


int print_result ()
{
    printf ( "\n\n    Print rdir...\n" );
    print_dir( rdir );
    printf ( "\n\n    Print rdist...\n" );
    print_array_double( rdist );
    return 0;
}

int test_distdrop_point_line()
{
    set_globals( NROWS, NCOLS, NULL_MAP, NULL_NEIG );
    printf ( "\n\n    Print point...\n" );
    print_array_short( point );
    printf ( "\n\n    Print line...\n" );
    print_array_short( line );
    distdrop( movements, point, line, elevation0,
              rdist, rdir, not_used, rdrop_up, rdrop_dw );
    return 0;
}


int test_distdrop_point_circle()
{
    set_globals(NROWS, NCOLS, NULL_MAP, NULL_NEIG);
    printf ( "\n\n    Print point...\n" );
    print_array_short( point );
    printf ( "\n\n    Print circle...\n" );
    print_array_short( circle );
    distdrop( movements, point, circle, elevation0,
              rdist, rdir, not_used, rdrop_up, rdrop_dw );

    return 0;
}


int test_distdrop_point_area()
{
    set_globals(NROWS, NCOLS, NULL_MAP, NULL_NEIG);
    printf ( "\n\n    Print point...\n" );
    print_array_short( point );
    printf ( "\n\n    Print area...\n" );
    print_array_short( area );
    distdrop( movements, point, area, elevation0,
              rdist, rdir, not_used, rdrop_up, rdrop_dw );

    return 0;
}

int test_distdrop_line_line()
{
    set_globals(NROWS, NCOLS, NULL_MAP, NULL_NEIG);
    printf ( "\n\n    Print line...\n" );
    print_array_short( line );
    printf ( "\n\n    Print line...\n" );
    print_array_short( line );
    distdrop( movements, line, line, elevation0,
              rdist, rdir, not_used, rdrop_up, rdrop_dw );

    return 0;
}

int test_distdrop_line_area()
{
    set_globals(NROWS, NCOLS, NULL_MAP, NULL_NEIG);
    printf ( "\n\n    Print line...\n" );
    print_array_short( line );
    printf ( "\n\n    Print area...\n" );
    print_array_short( area );
    distdrop( movements, line, area, elevation0,
              rdist, rdir, not_used, rdrop_up, rdrop_dw );

    return 0;
}

int test_distdrop_point_line_elev1()
{
    set_globals(NROWS, NCOLS, NULL_MAP, NULL_NEIG);
    printf ( "\n\n    Print line...\n" );
    print_array_short( point );
    printf ( "\n\n    Print area...\n" );
    print_array_short( line );
    printf ( "\n\n    Print elevation...\n" );
    print_array_float( elevation1 );
    distdrop( movements, point, line, elevation1,
              rdist, rdir, not_used, rdrop_up, rdrop_dw );

    return 0;
}

int test_distdrop_point_line_elev2()
{
    set_globals(NROWS, NCOLS, NULL_MAP, NULL_NEIG);
    printf ( "\n\n    Print line...\n" );
    print_array_short( point );
    printf ( "\n\n    Print area...\n" );
    print_array_short( line );
    printf ( "\n\n    Print elevation...\n" );
    print_array_float( elevation2 );
    distdrop( movements, point, line, elevation2,
              rdist, rdir, not_used, rdrop_up, rdrop_dw );
    
    return 0;
}

int all_tests()
{
    /* printf ( "\n\n\nTest list\n=============\n\n" );
    checktest ( test_list() ); */

    /* printf ( "\n\n\nTest distdrop: point-line\n" );
    printf ( "===================================\n\n" );
    checktest ( test_distdrop_point_line() );
    print_result (); */
    
    /* printf ( "\n\n\nTest distdrop: point-circle\n" );
    printf ( "===================================\n\n" );
    checktest ( test_distdrop_point_circle() );
    print_result (); */
    
    /* printf ( "\n\n\nTest distdrop: point-area\n" );
    printf ( "===================================\n\n" );
    checktest ( test_distdrop_point_area() );
    print_result (); */

    /* printf ( "\n\n\nTest distdrop: line-line\n" );
    printf ( "===================================\n\n" );
    checktest ( test_distdrop_line_line() );
    print_result (); */
    
    /* printf ( "\n\n\nTest distdrop: line-area\n" );
    printf ( "===================================\n\n" );
    checktest ( test_distdrop_line_area() );
    print_result (); */

    /* printf ( "\n\n\nTest distdrop: line-area-elev1\n" );
    printf ( "===================================\n\n" );
    checktest ( test_distdrop_point_line_elev1() );
    print_result ();
    printf ( "\n\n    Print rdrop_up...\n" );
    print_array_float( rdrop_up );
    printf ( "\n\n    Print rdrop_dw...\n" );
    print_array_float( rdrop_dw ); */

    printf ( "\n\n\nTest distdrop: line-area-elev2\n" );
    printf ( "===================================\n\n" );
    checktest ( test_distdrop_point_line_elev2() );
    print_result ();
    printf ( "\n\n    Print rdrop_up...\n" );
    print_array_float( rdrop_up );
    printf ( "\n\n    Print rdrop_dw...\n" );
    print_array_float( rdrop_dw );
    
    return 0;
}



int main()
{
    all_tests();
    return 0;
}

