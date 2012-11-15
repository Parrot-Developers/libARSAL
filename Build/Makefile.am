###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################
ACLOCAL_AMFLAGS = -I m4

# The list of header files that belong to the library (to be installed later)
HEADER_FILES	=	../Includes/libSAL/time.h 	\
					../Includes/libSAL/thread.h \
					../Includes/libSAL/mutex.h 	\
					../Includes/libSAL/socket.h \
					../Includes/libSAL/print.h \
					../Includes/libSAL/endianness.h

# The sources to add to the library and to add to the source distribution
SOURCE_FILES 	= 	$(HEADER_FILES)		\
					../Sources/time.c	\
					../Sources/thread.c \
					../Sources/mutex.c  \
					../Sources/socket.c \
					../Sources/print.c

# The library names to build (note we are building static and shared libs)
if DEBUG_MODE
lib_LTLIBRARIES 		= libsal_dbg.la
libsal_dbg_ladir 		= $(includedir)/libSAL
libsal_dbg_la_HEADERS 	= $(HEADER_FILES)
libsal_dbg_la_SOURCES 	= $(SOURCE_FILES)
else
lib_LTLIBRARIES 	= libsal.la
libsal_ladir 		= $(includedir)/libSAL
libsal_la_HEADERS 	= $(HEADER_FILES)
libsal_la_SOURCES 	= $(SOURCE_FILES)
endif

check_PROGRAMS = ../TestBench/test1 ../TestBench/test2
___TestBench_test1_SOURCES 	= ../TestBench/test1.c
___TestBench_test2_SOURCES 	= ../TestBench/test2.c
if DEBUG_MODE
___TestBench_test1_LDADD 	= libsal_dbg.la
___TestBench_test2_LDADD 	= libsal_dbg.la
else
___TestBench_test1_LDADD 	= libsal.la
___TestBench_test2_LDADD 	= libsal.la
endif

CLEAN_FILES			= libsal.la libsal_dbg.la

# Ensure the distribution of the doxygen configuration file
EXTRA_DIST = bootstrap

# Doxygen support
if HAVE_DOXYGEN
include doxygen.am
EXTRA_DIST += doxygen.ac doxygen.am $(DX_CONFIG)
endif
