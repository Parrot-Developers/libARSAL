###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################
ACLOCAL_AMFLAGS = -I m4

# The list of header files that belong to the library (to be installed later)
HEADER_FILES	=	../Includes/libSAL/time.h 	\
					../Includes/libSAL/thread.h \
					../Includes/libSAL/mutex.h 	\
					../Includes/libSAL/socket.h \
					../Includes/libSAL/print.h

# The sources to add to the library and to add to the source distribution
SOURCE_FILES 	= 	$(HEADER_FILES)		\
					../Sources/time.c	\
					../Sources/thread.c \
					../Sources/mutex.c  \
					../Sources/socket.c \
					../Sources/print.c

# The library names to build (note we are building static and shared libs)
if DEBUG_MODE
lib_LTLIBRARIES 					= ../Library/libsal_dbg.la
___Library_libsal_dbg_ladir 		= $(includedir)/libsal
___Library_libsal_dbg_la_HEADERS 	= $(HEADER_FILES)
___Library_libsal_dbg_la_SOURCES 	= $(SOURCE_FILES)
else
lib_LTLIBRARIES 				= ../Library/libsal.la
___Library_libsal_ladir 		= $(includedir)/libsal
___Library_libsal_la_HEADERS 	= $(HEADER_FILES)
___Library_libsal_la_SOURCES 	= $(SOURCE_FILES)
endif

check_PROGRAMS = ../TestBench/test1 ../TestBench/test2
___TestBench_test1_SOURCES 	= ../TestBench/test1.c
___TestBench_test1_LDADD 	= ../Library/libsal.la
___TestBench_test2_SOURCES 	= ../TestBench/test2.c
___TestBench_test2_LDADD 	= ../Library/libsal.la

# Ensure the distribution of the doxygen configuration file
EXTRA_DIST = bootstrap

# Doxygen support
if HAVE_DOXYGEN
include doxygen.am
EXTRA_DIST += doxygen.ac doxygen.am $(DX_CONFIG)
endif
	