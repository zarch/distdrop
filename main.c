
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
 * export CFLAGS="-O -Wall -W -pedantic -ansi -std=c99 -ggdb -g2"
 *
 *
 *
 * to use the test maps, just loading with v.in.ogr
 *
 * then give: ::
 * $ r.distdrop elevation=elev_null road=points_null domain=lines_null \
 * distance=dist0 direction=dir0 up=up0 down=dw0
 *
 * then remove the generated rasters: ::
 * $ g.remove rast=dir0@atest,dist0@atest,up0,dw0
 *
 * SEGMENTATION SIZE:
 *
 * seg 256 x 256
 * real    0m16.262s
 * user    0m12.519s
 * sys     0m0.913s
 *
 * seg 64 x 64
 * real    0m13.384s
 * user    0m10.283s
 * sys     0m0.723s
 *
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <grass/gis.h>
#include <grass/raster.h>
#include <grass/glocale.h>
#include <grass/segment.h>


#include "distdrop.h"

#define SEGROWSIZE  64
#define SEGCOLSIZE  64


/*
 * global function declaration
 */
extern CELL f_c ( CELL );
extern FCELL f_f ( FCELL );
extern DCELL f_d ( DCELL );

/*
 * function definitions
 */

CELL c_calc ( CELL x ) {
    /* we do nothing exciting here */
    return x;
}

FCELL f_calc ( FCELL x ) {
    /* we do nothing exciting here */
    return x;
}

DCELL d_calc ( DCELL x ) {
    /* we do nothing exciting here */
    return x;
}



int main ( int argc, char *argv[] ){
    struct History history;	/* holds meta-data (title, comments,..) */

    struct GModule *module;	/* GRASS module for parsing arguments */

    cell_map elev, dist, up, dw;
    cell_map road, domain, dir;

    /* options */
    struct{
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
    parm.domain->required = NO;
    parm.domain->description = _ ( "Name of the domain map" );

    /*
     *
     * OUTPUT
     *
     */
    // Distance
    parm.distance = G_define_standard_option ( G_OPT_R_OUTPUT );
    parm.distance->key = "distance";
    parm.distance->required = YES;
    parm.distance->description = _ ( "Name for output distance raster map" );
    parm.distance->guisection = _ ( "Outputs" );

    // Direction
    parm.direction = G_define_standard_option ( G_OPT_R_OUTPUT );
    parm.direction->key = "direction";
    parm.direction->required = YES;
    parm.direction->description = _ ( "Name for output direction raster map" );
    parm.direction->guisection = _ ( "Outputs" );

    // Drop up
    parm.drop_up = G_define_standard_option ( G_OPT_R_OUTPUT );
    parm.drop_up->key = "up";
    parm.drop_up->required = YES;
    parm.drop_up->description = _ ( "Name for output drop up raster map" );
    parm.drop_up->guisection = _ ( "Outputs" );

    // Drop down
    parm.drop_dw = G_define_standard_option ( G_OPT_R_OUTPUT );
    parm.drop_dw->key = "down";
    parm.drop_dw->required = YES;
    parm.drop_dw->description = _ ( "Name for output drop down raster map" );
    parm.drop_dw->guisection = _ ( "Outputs" );

    /* options and flags parser */
    if ( G_parser ( argc, argv ) )
        exit ( EXIT_FAILURE );


    /* stores options and flags to variables */
    elev.name   = parm.elevation->answer;
    road.name   = parm.road->answer;


    if (parm.domain->answer){
        domain.name = parm.domain->answer;
    }
    else {
        domain.name = elev.name;
    }

    dist.name = parm.distance->answer;
    dir.name  = parm.direction->answer;
    up.name   = parm.drop_up->answer;
    dw.name   = parm.drop_dw->answer;

    dist.type = FCELL_TYPE;
    dir.type = CELL_TYPE;
    up.type = FCELL_TYPE;
    dw.type = FCELL_TYPE;

    G_message ( "Check input and output maps...\n" );

    G_check_input_output_name ( elev.name, dist.name, G_FATAL_EXIT );
    G_check_input_output_name ( road.name, dir.name, G_FATAL_EXIT );
    G_check_input_output_name ( domain.name, up.name, G_FATAL_EXIT );
    G_check_input_output_name ( domain.name, dw.name, G_FATAL_EXIT );

    seg_map segment_info;
    segment_info.nrows = Rast_window_rows();
    segment_info.ncols = Rast_window_cols();
    segment_info.srows = SEGROWSIZE;
    segment_info.scols = SEGCOLSIZE;
    segment_info.maxmem = 1024;

    G_message ( "Read and prepare input and output maps...\n" );
    queue **redo_segments = prepare_input ( &road, &domain, &dist,
                                        &dir, &up, &dw, &segment_info);

    G_message ( "Start processing data...\n" );
    distdrop ( &elev, &dist, &dir, &up, &dw,
               &segment_info, &movements,
               redo_segments);

    //print_dir ( &dir, &segment_info );


    /* Copy the segments back to the input maps */
    copy_segment(&dist, 1);
    copy_segment(&dir, 1);
    copy_segment(&up, 1);
    copy_segment(&dw, 1);

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

    exit ( EXIT_SUCCESS );
}
