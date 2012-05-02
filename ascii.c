#include <stdio.h>
#include <stdlib.h>

static void read_grass_header(FILE *f, int *nrows, int *ncols);

/* GRASS File
north: 80
south: 0
east: 120
west: 0
rows: 8
cols: 12
1 1 1 1 1 1 1 1 1 1 1 1 
1 1 1 1 1 1 1 1 1 1 1 1 
1 1 1 1 1 1 1 1 1 1 1 1 
1 1 1 1 1 1 1 1 1 1 1 1 
1 1 1 1 1 1 1 1 1 1 1 1 
1 1 1 1 1 1 1 1 1 1 1 1 
1 1 1 1 1 1 1 1 1 1 1 1 
1 1 1 1 1 1 1 1 1 1 1 1 
*/

static void read_grass_header(FILE *f, int *nrows, int *ncols) {
    
    char buff[1024];
    float dummy;
    
    /* north */
    fscanf(f, "%s %f", buff, &dummy);
    /* south */
    fscanf(f, "%s %f", buff, &dummy);
    /* east */
    fscanf(f, "%s %f", buff, &dummy);
    /* west */
    fscanf(f, "%s %f", buff, &dummy);
    /* rows */
    fscanf(f, "%s %i", buff, nrows);
    /* cols */
    fscanf(f, "%s %i", buff, ncols);

    /* printf(" rows %i cols %i \n", *nrows, *ncols); */
}

/* Read short data from a grass raster ascii file */
short **read_short_data(const char* filename, int *nrows, int *ncols)
{
    FILE *f;
    int i;
    int j;
    short **a;

    f = fopen(filename, "r");

    if(f == NULL) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        exit(1);
    }

    /* printf("Read file %s\n", filename); */

    read_grass_header(f, nrows, ncols);

    a = (short**)calloc(*nrows, sizeof(short *));

    if(a == NULL) {
        fprintf(stderr, "Unable allocate memory");
        exit(1);
    }

    for(i = 0; i < *nrows; i++){
        a[i] = (short*)calloc(*ncols, sizeof(short));
        if(a[i] == NULL) {
            fprintf(stderr, "Unable allocate memory");
            exit(1);
        }
    }

    for(i = 0; i < *nrows; i++)
        for(j = 0; j < *ncols; j++) {
            fscanf(f, "%hd", &a[i][j]);
        }
    fclose(f);

    return a;
}

/* Read float data from a grass raster ascii file */
float **read_float_data(const char* filename, int *nrows, int *ncols)
{
    FILE *f;
    int i;
    int j;
    float **a;

    f = fopen(filename, "r");

    if(f == NULL) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        exit(1);
    }

    /* printf("Read file %s\n", filename); */

    read_grass_header(f, nrows, ncols);

    a = (float**)calloc(*nrows, sizeof(float *));

    if(a == NULL) {
        fprintf(stderr, "Unable allocate memory");
        exit(1);
    }

    for(i = 0; i < *nrows; i++){
        a[i] = (float*)calloc(*ncols, sizeof(float));
        if(a[i] == NULL) {
            fprintf(stderr, "Unable allocate memory");
            exit(1);
        }
    }

    for(i = 0; i < *nrows; i++)
        for(j = 0; j < *ncols; j++) {
            fscanf(f, "%f", &a[i][j]);
        }
    fclose(f);

    return a;
}
