###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################
builddir=@builddir@

# the list of header files that belong to the library (to be installed later)
HEADER_FILES	=	../Includes/libSAL/time.h 	\
					../Includes/libSAL/thread.h \
					../Includes/libSAL/mutex.h 	\
					../Includes/libSAL/print.h

# the sources to add to the library and to add to the source distribution
SOURCE_FILES 	= 	$(HEADER_FILES)		\
					../Sources/time.c	\
					../Sources/thread.c \
					../Sources/mutex.c  \
					../Sources/print.c

# the library names to build (note we are building static libs only)
if DEBUG_MODE
lib_LIBRARIES 					= ../Library/libsal_dbg.a
___Library_libsal_dbg_adir 		= $(includedir)/libsal
___Library_libsal_dbg_a_HEADERS = $(HEADER_FILES)
___Library_libsal_dbg_a_SOURCES = $(SOURCE_FILES)
else
lib_LIBRARIES 				= ../Library/libsal.a
___Library_libsal_adir 		= $(includedir)/libsal
___Library_libsal_a_HEADERS = $(HEADER_FILES)
___Library_libsal_a_SOURCES = $(SOURCE_FILES)
endif

# ensure the distribution of the doxygen configuration file
EXTRA_DIST = bootstrap

# Doxygen support
if HAVE_DOXYGEN
include doxygen.am
EXTRA_DIST += doxygen.ac doxygen.am $(DX_CONFIG)
endif

display:
	@echo builddir=$(builddir)
	