#define NMV 8

#define TYPE_SHORT 1
#define TYPE_INT 2
#define TYPE_FLOAT 3
#define TYPE_DOUBLE 4

typedef struct __move__
{
    short int mv[2];
    short int dir;
    float    dist;
} move;

/* global variables declaration */
extern move movements[NMV];