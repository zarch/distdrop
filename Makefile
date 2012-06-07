# fix this relative to include/
# or use absolute path to the GRASS source code

MODULE_TOPDIR = ../..

PGM = r.distdrop

LIBES = $(GISLIB) $(RASTERLIB) $(SEGMENTLIB)
DEPENDENCIES = $(GISDEP) $(RASTERDEP) $(SEGMENTDEP)
EXTRA_CFLAGS = -O -Wall -W -pedantic -ansi -std=c99 -ggdb -g2

include $(MODULE_TOPDIR)/include/Make/Module.make

default: cmd
