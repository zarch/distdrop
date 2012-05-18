
/****************************************************************************
 *
 * MODULE:       r.example
 * AUTHOR(S):    Markus Neteler - neteler itc.it
 *               with hints from: Glynn Clements - glynn gclements.plus.com
 * PURPOSE:      Just copies a raster map, preserving the raster map type
 *               Intended to explain GRASS raster programming
 *
 * COPYRIGHT:    (C) 2002, 2005-2009 by the GRASS Development Team
 *
 *               This program is free software under the GNU General Public
 *   	    	 License (>=v2). Read the file COPYING that comes with GRASS
 *   	    	 for details.
 *
 *****************************************************************************
export CFLAGS="-O -Wall -W -pedantic -ansi -std=c99 -ggdb -g2"

r.distdrop elevation=elev_null@atest road=points_null@atest domain=lines_null@atest \
distance=dist0 direction=dir0 up=up0 down=dw0
g.remove rast=dir0@atest,dist0@atest,up0,dw0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grass/gis.h>
#include <grass/raster.h>
#include <grass/glocale.h>
#include <grass/rowio.h>

#include "glob.h"
#include "cache.h"


/*
 * global function declaration
 */
extern CELL f_c ( CELL );
extern FCELL f_f ( FCELL );
extern DCELL f_d ( DCELL );

/*
 * function definitions
 */

CELL c_calc ( CELL x )
{
    /* we do nothing exciting here */
    return x;
}

FCELL f_calc ( FCELL x )
{
    /* we do nothing exciting here */
    return x;
}

DCELL d_calc ( DCELL x )
{
    /* we do nothing exciting here */
    return x;
}





/*
 * main function
 * it copies raster input raster map, calling the appropriate function for each
 * data type (CELL, DCELL, FCELL)
 */
int main ( int argc, char *argv[] )
{
    struct Cell_head cellhd;	/* it stores region information,
				   and header information of rasters */

    //RASTER_MAP_TYPE data_type;	/* type of the map (CELL/DCELL/...) */
    struct History history;	/* holds meta-data (title, comments,..) */

    struct GModule *module;	/* GRASS module for parsing arguments */

    fcell_map elev, dist, up, dw;
    cell_map road, domain, dir;

    /* options */
    struct
    {
        struct Option *elevation, *road, *domain, *distance, *direction,
                *drop_up, *drop_dw;
    } parm;

    /* initialize GIS environment */
    G_gisinit ( argv[0] );		/* reads grass env, stores program name to G_program->name() */

    /* initialize module */
    module = G_define_module();
    G_add_keyword ( _ ( "raster" ) );
    G_add_keyword ( _ ( "distance" ) );
    G_add_keyword ( _ ( "drop" ) );
    module->description = _ ( "Compute the distance and the drop from closest road" );

    /*
     *
     * INPUT
     *
     */
    /* Define the different options as defined in gis.h */
    parm.elevation = G_define_standard_option ( G_OPT_R_ELEV );
    parm.elevation->key = "elevation";
    parm.elevation->required = YES;
    parm.elevation->description = _ ( "Name of the elevation map" );

    parm.road = G_define_standard_option ( G_OPT_R_INPUT );
    parm.road->key = "road";
    parm.road->required = YES;
    parm.road->description = _ ( "Name of the road map" );

    parm.domain = G_define_standard_option ( G_OPT_R_INPUT );
    parm.domain->key = "domain";
    parm.domain->required = YES;
    parm.domain->description = _ ( "Name of the domain map" );

    /*
     *
     * OUTPUT
     *
     */
    // Distance
    parm.distance = G_define_standard_option ( G_OPT_R_OUTPUT );
    parm.distance->key = "distance";
    parm.distance->required = NO;
    parm.distance->description = _ ( "Name for output distance raster map" );
    parm.distance->guisection = _ ( "Outputs" );

    // Direction
    parm.direction = G_define_standard_option ( G_OPT_R_OUTPUT );
    parm.direction->key = "direction";
    parm.direction->required = NO;
    parm.direction->description = _ ( "Name for output direction raster map" );
    parm.direction->guisection = _ ( "Outputs" );

    // Drop up
    parm.drop_up = G_define_standard_option ( G_OPT_R_OUTPUT );
    parm.drop_up->key = "up";
    parm.drop_up->required = NO;
    parm.drop_up->description = _ ( "Name for output drop up raster map" );
    parm.drop_up->guisection = _ ( "Outputs" );

    // Drop down
    parm.drop_dw = G_define_standard_option ( G_OPT_R_OUTPUT );
    parm.drop_dw->key = "down";
    parm.drop_dw->required = NO;
    parm.drop_dw->description = _ ( "Name for output drop down raster map" );
    parm.drop_dw->guisection = _ ( "Outputs" );

    /* options and flags parser */
    if ( G_parser ( argc, argv ) )
        exit ( EXIT_FAILURE );


    /* stores options and flags to variables */
    elev.name   = parm.elevation->answer;
    road.name   = parm.road->answer;
    domain.name = parm.domain->answer;

    dist.name = parm.distance->answer;
    dir.name  = parm.direction->answer;
    up.name   = parm.drop_up->answer;
    dw.name   = parm.drop_dw->answer;

    /* store maps types */
    elev.type = FCELL_TYPE;
    printf ( "\nCheck where segmentation is...\n" );
    road.type = CELL_TYPE;
    domain.type = CELL_TYPE;
    dist.type = FCELL_TYPE;
    dir.type = CELL_TYPE;
    up.type = FCELL_TYPE;
    dw.type = FCELL_TYPE;

    printf ( "\nCheck input and output maps...\n" );

    G_check_input_output_name ( elev.name, dist.name, G_FATAL_EXIT );
    G_check_input_output_name ( road.name, dir.name, G_FATAL_EXIT );
    G_check_input_output_name ( domain.name, up.name, G_FATAL_EXIT );
    G_check_input_output_name ( domain.name, dw.name, G_FATAL_EXIT );

    printf ( "\nGet the file descriptor of the input maps\n" );
    /* Rast_open_old - returns file destriptor (>0) */
    elev.fd = Rast_open_old ( elev.name, "" );
    road.fd = Rast_open_old ( road.name, "" );
    domain.fd = Rast_open_old ( domain.name, "" );

    printf ( "\nGet the file descriptor of the output maps\n" );
    dist.fd = Rast_open_new ( dist.name, dist.type );
    //slp_raster = Rast_allocate_buf ( data_type );
    //Rast_set_null_value ( slp_raster, Rast_window_cols(), data_type );
    dir.fd = Rast_open_new ( dir.name, dir.type );
    up.fd = Rast_open_new ( up.name, up.type );
    dw.fd = Rast_open_new ( dw.name, dw.type );

    /* Allocate input buffer */
    elev.buf = (FCELL *) Rast_allocate_buf ( FCELL_TYPE );
    road.buf = (CELL *) Rast_allocate_buf ( CELL_TYPE );
    domain.buf = (CELL *) Rast_allocate_buf ( CELL_TYPE );

    /* Allocate output buffer, use input map data_type */
    dist.buf = (FCELL *) Rast_allocate_buf ( FCELL_TYPE );
    dir.buf = (CELL *) Rast_allocate_buf ( CELL_TYPE );
    up.buf = (FCELL *) Rast_allocate_buf ( FCELL_TYPE );
    dw.buf = (FCELL *) Rast_allocate_buf ( FCELL_TYPE );

    nrows = Rast_window_rows();
    ncols = Rast_window_cols();
    int row_cache = 3;

    /* Allocate row cache*/
    elev.cache =   ( FCELL ** ) G_malloc( row_cache * sizeof(FCELL *));
    road.cache =   (CELL **) G_malloc(row_cache * sizeof(CELL *));
    domain.cache = (CELL **) G_malloc(row_cache* sizeof(CELL *));

    dist.cache = (FCELL **) G_malloc(row_cache * sizeof(FCELL *));
    dir.cache =  (CELL **) G_malloc(row_cache * sizeof(CELL *));
    up.cache =   (FCELL **) G_malloc(row_cache * sizeof(FCELL *));
    dw.cache =   (FCELL **) G_malloc(row_cache * sizeof(FCELL *));

    int rowtest = 4;
    // setup the ROWIO struct
    // similar to line 68, r.mfilter/perform.c
    //            R,        fd,      nrows, len, getrow, putrow
    Rowio_setup ( &elev.io, elev.fd, nrows, ncols * sizeof(FCELL), getrow, NULL );
    elev.cache[0] = Rowio_get(&elev.io, rowtest-1);
    elev.cache[1] = Rowio_get(&elev.io, rowtest-1);
    elev.cache[2] = Rowio_get(&elev.io, rowtest-1);
    // read and modify the value in the row
    elev.cache[0][0] = 10 * elev.cache[0][0];
    // write in to the file
    Rowio_put(&elev.io, elev.cache[0], rowtest-1);


    /*
     *
     * Do something cool!
     *
     */
    int row, col;

    for ( row = 0; row < nrows; row++ )
{
        Rast_get_f_row ( elev.fd, elev.buf, row );
        Rast_get_c_row ( road.fd, road.buf, row );
        Rast_get_c_row ( domain.fd, domain.buf, row );
        for ( col = 0; col < ncols; col++ )
        {
            dist.buf[col] = ( FCELL ) ( elev.buf[col] + road.buf[col] + domain.buf[col] );
            dir.buf[col] = ( CELL ) ( elev.buf[col] + road.buf[col] + domain.buf[col] );
            up.buf[col] = ( FCELL ) ( elev.buf[col] + road.buf[col] + domain.buf[col] );
            dw.buf[col] = ( FCELL ) ( elev.buf[col] + road.buf[col] + domain.buf[col] );
        }
        Rast_put_f_row ( dist.fd, dist.buf );
        Rast_put_c_row ( dir.fd, dir.buf );
        Rast_put_f_row ( up.fd, up.buf );
        Rast_put_f_row ( dw.fd, dw.buf );
    }

    //DEBUG:
    fprintf ( stdout, "\nINPUT: elev = %s, road=%s, domain=%s\n", elev.name, road.name, domain.name );

    /* closing raster maps */
    Rast_close ( elev.fd );
    Rast_close ( road.fd );
    Rast_close ( domain.fd );
    Rast_close ( dist.fd );
    Rast_close ( dir.fd );
    Rast_close ( up.fd );
    Rast_close ( dw.fd );

    /* add command line incantation to history file */

    /* History Distance map */
    Rast_short_history ( dist.name, "raster", &history );
    Rast_append_format_history ( &history, "distance map elev = %s, road=%s, domain=%s", elev.name, road.name, domain.name );
    Rast_command_history ( &history );
    Rast_write_history ( dist.name, &history );

    /* History Direction map */
    Rast_short_history ( dir.name, "raster", &history );
    Rast_append_format_history ( &history, "direction map elev = %s, road=%s, domain=%s", elev.name, road.name, domain.name);
    Rast_command_history ( &history );
    Rast_write_history ( dir.name, &history );

    /* History Drop up map */
    Rast_short_history ( up.name, "raster", &history );
    Rast_append_format_history ( &history, "drop up map elev = %s, road=%s, domain=%s", elev.name, road.name, domain.name );
    Rast_command_history ( &history );
    Rast_write_history ( up.name, &history );

    /* History Drop down map */
    Rast_short_history ( dw.name, "raster", &history );
    Rast_append_format_history ( &history, "drop down map elev = %s, road=%s, domain=%s", elev.name, road.name, domain.name );
    Rast_command_history ( &history );
    Rast_write_history ( dw.name, &history );


    /* memory cleanup
    free ( &elev );
    free ( &road );
    free ( &domain );
    free ( &dist );
    free ( &dir );
    free ( &up );
    free ( &dw );*/

    exit ( EXIT_SUCCESS );
}
