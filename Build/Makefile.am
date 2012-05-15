## Process this file with automake to produce makefile.in
AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

lib_LIBRARIES = libSAL.a
libSAL_a_SOURCES = libSAL.c libSAL.h 
dist_noinst_SCRIPTS = autogen.sh

# the library names to build (note we are building static libs only)
lib_LIBRARIES = libsal.a

# where to install the headers on the system
libsal_adir = $(includedir)/libsal

# the list of header files that belong to the library (to be installed later)
libsal_a_HEADERS = libsal.h 

# the sources to add to the library and to add to the source distribution
libsal_a_SOURCES = $(libsal_a_HEADERS) libsal.c 
						


