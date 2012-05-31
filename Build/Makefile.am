###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################
# the library names to build (note we are building static libs only)
lib_LIBRARIES = libsal.a

# where to install the headers on the system
libsal_adir = $(includedir)/libsal

# the list of header files that belong to the library (to be installed later)
libsal_a_HEADERS = ../Includes/libSAL/time.h ../Includes/libSAL/thread.h ../Includes/libSAL/mutex.h 

# the sources to add to the library and to add to the source distribution
libsal_a_SOURCES = $(libsal_a_HEADERS) ../Sources/time.c ../Sources/thread.c ../Sources/mutex.c  

# ensure the distribution of the doxygen configuration file
EXTRA_DIST = bootstrap

# Doxygen support
if HAVE_DOXYGEN
	include doxygen.am
	EXTRA_DIST += doxygen.ac doxygen.am Doxyfile ../Doc
endif
