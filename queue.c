#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

void append ( int row, int col, queue *l )
{
    elem *e = malloc ( sizeof ( elem ) );

    e->point.row = row;
    e->point.col = col;
    e->next = NULL;

    // set the first point if queue is empty, otherwise
    // update the link from the last element to new point
    if ( l->length == 0 )
        l->first = e;
    else
        l->last->next = e;

    // update the last in the queue
    l->last = e;

    // update the length attribute
    l->length += 1;
}


elem *pop ( queue *l )
{
    //
    // return the first element of a queue and remove from the queue
    // if the queue is empty return NULL
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



void array_append ( int row, int col, queue **l )
{
    elem *e = malloc ( sizeof ( elem ) );

    // initialize the element
    e->point.row = row;
    e->point.col = col;
    e->next = NULL;

    append ( row, col, l[row] );
}


void populate ( int nrows, queue *l )
{
    for ( int row = 0; row < nrows; row++ )
        append ( row, 0, l );
}



queue **create_empty_array_of_queue( int nrows )
{
    queue **a = malloc ((long unsigned int)nrows * sizeof ( queue * ) );

    for(int i = 0; i < nrows; i++)
        a[i] = create_empty_queue();

    return a;
}



queue * create_empty_queue()
{
    queue *l = malloc ( sizeof ( queue ) );

    l->length = 0;
    l->first = l->last = NULL;
    return l;
}

void print_array_of_queue ( queue **l, int nrows )
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

void print_queue ( queue *l )
{
    elem *e = l->first;

    for ( int i = 0; i < l->length; i++ )
    {
        printf ( "%d) row=%d col=%d\n", i, e->point.row, e->point.col );
        e = e->next;
    }
}

void free_queue ( queue *l )
{
    elem *e = l->first, *p;

    for ( int i = 0; i < l->length; i++ )
    {
        p = e;
        e = e->next;
        free ( p );
    }
}


