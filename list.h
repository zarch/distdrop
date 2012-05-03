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


/* global variables declaration */
/* extern int quante_volte; */

/* global functions declaration */
extern list *create_empty_list ( void );
extern list **create_empty_array_of_list ( int ); /* */
extern void add_point_to_list ( int, int, list * );
extern void add_point_to_array_of_list ( int, int, list ** );
extern void populate ( int, list * );
extern void print_list ( list * );
extern void print_array_of_list ( list **, int );
extern void free_list ( list * );

#endif  /* LIST_H */
