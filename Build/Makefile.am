###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################
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
lib_LIBRARIES 			= libsal_dbg.a
libsal_dbg_adir 		= $(includedir)/libsal
libsal_dbg_a_HEADERS 	= $(HEADER_FILES)
libsal_dbg_a_SOURCES 	= $(SOURCE_FILES)
else
lib_LIBRARIES 		= libsal.a
libsal_adir 		= $(includedir)/libsal
libsal_a_HEADERS 	= $(HEADER_FILES)
libsal_a_SOURCES 	= $(SOURCE_FILES)
endif

# ensure the distribution of the doxygen configuration file
EXTRA_DIST = bootstrap

# Doxygen support
if HAVE_DOXYGEN
include doxygen.am
EXTRA_DIST += doxygen.ac doxygen.am $(DX_CONFIG)
endif
