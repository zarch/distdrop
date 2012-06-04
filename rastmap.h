#ifndef DRASTMAP_H
#define RASTMAP_H
#include <grass/raster.h>
#include <grass/segment.h>


/* RASTER MAP */
typedef struct __cell_map__ {
    char *name;
    char *mapset;
    int fd;
    RASTER_MAP_TYPE type;
    const char *temp_file;
    int temp_fd;
    SEGMENT seg;
    void *buf;
    CELL *cbuf;
    FCELL *fbuf;
    DCELL *dbuf;
} cell_map;

/* COLLECT SEGMENTATION INFO */
typedef struct __seg_map__ {
    int nrows;
    int ncols;
    int srows;
    int scols;
    int maxmem;
    int nseg;
    int segments_in_memory;
} seg_map;

extern int allocate_buf(cell_map *);
extern int check_fd(cell_map *);
extern int manage_segments(seg_map *);
extern int init_seg_map(cell_map *, seg_map *);
extern int copy_segment(cell_map *, short);

#endif  /* RASTMAP_H */
