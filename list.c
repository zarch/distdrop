#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void
add_point_to_list(int row, int col, list *l)
{
  elem *e = malloc(sizeof(elem));

  e->point.row = row;
  e->point.col = col;
  e->next = NULL;

  /* set the first point if list is empty, otherwise */
  /* update the link from the last element to new point */
  if (l->length == 0)
    l->first = e;
  else
    l->last->next = e;

  /* update the last in the list */
  l->last = e;

  /* update the length attribute */
  l->length += 1;
}

void
add_point_to_order_list(int row, int col, list *l)
{
  int i;
  int insert = 0;
  elem *e_prev;
  elem *e_next;
  elem *e_new = malloc(sizeof(elem));

  e_new->point.row = row;
  e_new->point.col = col;
  e_new->next = NULL;

  /* set the first point if list is empty, otherwise */
  /* update the link from the last element to new point */
  if (l->length == 0)
  {
    l->first = e_new;
    /* update the last in the list */
    l->last = e_new;
  }
  else
  {
    /* check if the new el is in the same or later row of the last in list */
    if (e_new->point.row >= l->last->point.row)
    {
      /* update the last element in the list */
      l->last->next = e_new;
      /* update the last in the list */
      l->last = e_new;
    }
    /* check if the new el is in a row before the first in the list */
    else if (e_new->point.row <= l->first->point.row)
    {
      /* update the the new element insert as next the first in the list */
      e_new->next = l->first;
      /* update the first el in the list */
      l->first = e_new;
    }
    /* If is not in the first and is not in the last, is in the middle */
    else
    {
      /* go through the list and find the right position */
      for (i = 0, e_prev = l->first; i < l->length-1; i++)
      {
        e_next = e_prev->next;
        if (e_new->point.row <= e_next->point.row)
        {
          /* Insert between */
          e_prev->next = e_new;
          e_new->next = e_next;
          break;
        }
        else
        {
          /* Update previous element with the next */
          e_prev = e_next;
        }
      }
    }
  }
  /* update the length attribute */
  l->length += 1;
}

void
populate(int nrows, list *l)
{
  int row;
  for (row = 0; row < nrows; row++)
    add_point_to_list(row, 0, l);
}

void
populate_inverse_rows_cols(int nrows, int ncols, list *l)
{
  int row;
  int col;
  for (col = 0; col < ncols; col++)
    for (row = 0; row < nrows; row++)
      add_point_to_list(row, col, l);
}

void
order_populate_inverse_rows_cols(int nrows, int ncols, list *l)
{
  int row;
  int col;
  for (col = 0; col < ncols; col++)
    for (row = 0; row < nrows; row++)
      add_point_to_order_list(row, col, l);
}

list *
create_empty_list()
{
  list *l = malloc(sizeof(list));
  l->length = 0;
  l->first = l->last = NULL;
  return l;
}

void
print_list(list *l)
{
  int i;
  elem *e;
  /* elem *p; */

  for (i = 0, e = l->first; i < l->length; i++)
    {
      printf("%d) row=%d col=%d\n", i, e->point.row, e->point.col);
      /* p = e; */
      e = e->next;
      /* free(p); */
    }
}

void
free_list(list *l)
{
  int i;
  elem *e;
  elem *p;

  for (i = 0, e = l->first; i < l->length; i++)
    {
      p = e;
      e = e->next;
      free(p);
    }
}

void
join_list(char str, list *l)
{
    int i;
    elem *e;

    for (i = 0, e = l->first; i < l->length; i++)
    {
        if (i == l->length - 1)
        {
            printf("%d) row=%d col=%d\n", i, e->point.row, e->point.col); /* vedi pagina 98 di programmare in C */
        }
        else
        {
            printf("%d) row=%d col=%d\n", i, e->point.row, e->point.col);
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
