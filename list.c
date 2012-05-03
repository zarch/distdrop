#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void add_point_to_list ( int row, int col, list *l )
{
    elem *e = malloc ( sizeof ( elem ) );

    e->point.row = row;
    e->point.col = col;
    e->next = NULL;

    /* set the first point if list is empty, otherwise */
    /* update the link from the last element to new point */
    if ( l->length == 0 )
        l->first = e;
    else
        l->last->next = e;

    /* update the last in the list */
    l->last = e;

    /* update the length attribute */
    l->length += 1;
}

void add_point_to_array_of_list ( int row, int col, list **l )
{
    elem *e = malloc ( sizeof ( elem ) );

    e->point.row = row;
    e->point.col = col;
    e->next = NULL;

    /* set the first point if list is empty, otherwise */
    /* update the link from the last element to new point */
    if ( l[row]->length == 0 )
        l[row]->first = e;
    else
        l[row]->last->next = e;

    /* update the last in the list */
    l[row]->last = e;

    /* update the length attribute */
    l[row]->length += 1;
}


void populate ( int nrows, list *l )
{
    int row;
    for ( row = 0; row < nrows; row++ )
        add_point_to_list ( row, 0, l );
}



list **create_empty_array_of_list( int nrows )
{
    int i;
    list **a;
    a = malloc ((long unsigned int)nrows * sizeof ( list * ) );
    for(i = 0; i < nrows; i++){
        a[i] = create_empty_list();
    }
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
    int i,j;
    elem *e;
    /* elem *p; */

    for ( i = 0; i < nrows; i++ )
    {
        printf("\nrow: %d, length: %d\n", i, l[i]->length);
        if (l[i]->length != 0){
            for ( j = 0, e = l[i]->first; j < l[i]->length; j++ )
            {
                printf ( "%d ", e->point.col );
                /* p = e; */
                e = e->next;
                /* free(p); */
            }
        }
    }
}

void print_list ( list *l )
{
    int i;
    elem *e;
    /* elem *p; */

    for ( i = 0, e = l->first; i < l->length; i++ )
    {
        printf ( "%d) row=%d col=%d\n", i, e->point.row, e->point.col );
        /* p = e; */
        e = e->next;
        /* free(p); */
    }
}

void free_list ( list *l )
{
    int i;
    elem *e, *p;

    for ( i = 0, e = l->first; i < l->length; i++ )
    {
        p = e;
        e = e->next;
        free ( p );
    }
}

void join_list ( char str, list *l )
{
    int i;
    elem *e;

    for ( i = 0, e = l->first; i < l->length; i++ )
    {
        if ( i == l->length - 1 )
        {
            printf ( "%d) row=%d col=%d\n", i, e->point.row, e->point.col ); /* vedi pagina 98 di programmare in C */
        }
        else
        {
            printf ( "%d) row=%d col=%d\n", i, e->point.row, e->point.col );
        }
    };
}

/* int
main()
{
  list *l = create_empty_list();
  populate(10, l);
  print_list(l);
  free_list(l);
  return 0;
}
*/
