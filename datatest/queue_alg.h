/* new variable types declaration */
#ifndef QUEUE_H
#define QUEUE_H

// point
typedef struct __pnt__
{
    int row;
    int col;
} pnt;


// element
typedef struct __elem__
{
    pnt point;
    struct __elem__ *next;
} elem;


// queue of element
typedef struct __queue__
{
    elem *first;
    elem *last;
    int length;
} queue;


// global variables declaration


// global functions declaration
extern queue *create_empty_queue ( void );
extern queue **create_empty_array_of_queue ( int );
extern void append ( int, int, queue * );
extern void array_append ( int, int, queue ** );
extern elem *pop ( queue * );
extern void populate ( int, queue * );
extern void print_queue ( queue * );
extern void print_array_of_queue ( queue **, int );
extern void free_queue ( queue * );

#endif  /* QUEUE_H */
