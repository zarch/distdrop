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
 *
 *
 */

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


int test_drop()
{
    list *road_points = get_not_null ( road );
    print_list ( road_points );
    free_list ( road_points );
    return 0;
}

int all_tests()
{
    printf ( "\n\n\nTest list\n=============\n\n" );
    test_list();
    printf ( "\n\n\nTest drop\n=============\n\n" );
    test_drop();
    return 0;
}

int main()
{
    all_tests();
    return 0;
}

