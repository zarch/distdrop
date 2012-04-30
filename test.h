#define NROWS 10        /* number of rows */
#define NCOLS 11        /* number of columns */
#define NULL_MAP 0      /* value of null value in the map */
#define NULL_NEIG -9999 /* value of null value in the map */



/* domain examples */
extern short point[NROWS][NCOLS];
extern short line[NROWS][NCOLS];
extern short circle[NROWS][NCOLS];
extern short area[NROWS][NCOLS];

/* elevation examples */
extern float elevation0[NROWS][NCOLS];
extern float elevation1[NROWS][NCOLS];
extern float elevation2[NROWS][NCOLS];

/* results */
extern double rdist[NROWS][NCOLS];
extern short rdir[NROWS][NCOLS];
extern float rdrop_up[NROWS][NCOLS];
extern float rdrop_dw[NROWS][NCOLS];
extern short not_used[NROWS][NCOLS];



