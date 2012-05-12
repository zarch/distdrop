#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void add_point_to_list ( int row, int col, list *l )
{
    elem *e = malloc ( sizeof ( elem ) );

    e->point.row = row;
    e->point.col = col;
    e->next = NULL;

    // set the first point if list is empty, otherwise
    // update the link from the last element to new point
    if ( l->length == 0 )
        l->first = e;
    else
        l->last->next = e;

    // update the last in the list
    l->last = e;

    // update the length attribute
    l->length += 1;
}


elem *pop ( list *l )
{
    //
    // return the first element of a list and remove from the list
    // if the list is empty return NULL
    elem *el;

    if ( l->length != 0 )
    {
        el = (elem *) l->first;
        l->first = el->next;
        l->length -= 1;
    }
    else
        el = NULL;

    return el;
}



void add_point_to_array_of_list ( int row, int col, list **l )
{
    elem *e = malloc ( sizeof ( elem ) );

    // initialize the element
    e->point.row = row;
    e->point.col = col;
    e->next = NULL;

    add_point_to_list ( row, col, l[row] );
}


void populate ( int nrows, list *l )
{
    for ( int row = 0; row < nrows; row++ )
        add_point_to_list ( row, 0, l );
}



list **create_empty_array_of_list( int nrows )
{
    list **a = malloc ((long unsigned int)nrows * sizeof ( list * ) );

    for(int i = 0; i < nrows; i++)
        a[i] = create_empty_list();

    return a;
}



list * create_empty_list()
{
    list *l = malloc ( sizeof ( list ) );

    l->length = 0;
    l->first = l->last = NULL;
    return l;
}

void print_array_of_list ( list **l, int nrows )
{
    elem *e;

    for ( int i = 0; i < nrows; i++ )
    {
        printf("\nrow: %d, length: %d\n", i, l[i]->length);
        if (l[i]->length != 0)
        {
            e = l[i]->first;
            for ( int j = 0; j < l[i]->length; j++ )
            {
                printf ( "%d ", e->point.col );
                e = e->next;
            }
        }
    }
}

void print_list ( list *l )
{
    elem *e = l->first;

    for ( int i = 0; i < l->length; i++ )
    {
        printf ( "%d) row=%d col=%d\n", i, e->point.row, e->point.col );
        e = e->next;
    }
}

void free_list ( list *l )
{
    elem *e = l->first, *p;

    for ( int i = 0; i < l->length; i++ )
    {
        p = e;
        e = e->next;
        free ( p );
    }
}


