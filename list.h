/* new variable types declaration */
#ifndef LIST_H
#define LIST_H

/* point */
typedef struct __pnt__
{
  int row;
  int col;
} pnt;


/* element */
typedef struct __elem__
{
  pnt point;
  struct __elem__ *next;
} elem;


/* list of element */
typedef struct __list__
{
  elem *first;
  elem *last;
  int length;
} list;

#endif  /* LIST_H */

/* global variables declaration */
/* extern int quante_volte; */

/* global functions declaration */
extern list *create_empty_list(void);
extern void add_point_to_list(int, int, list *);
extern void add_point_to_order_list(int, int, list *);
extern void populate(int, list *);
extern void populate_inverse_rows_cols(int, int, list *);
extern void order_populate_inverse_rows_cols(int, int, list *);
extern void print_list(list *);
/* extern void free_list(list *); */
