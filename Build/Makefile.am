###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################
ACLOCAL_AMFLAGS                         =   -I m4

# The list of header files that belong to the library (to be installed later)
HEADER_FILES                            =   ../Includes/libARSAL/ARSAL_Time.h       \
                                            ../Includes/libARSAL/ARSAL_Thread.h     \
                                            ../Includes/libARSAL/ARSAL_Mutex.h      \
                                            ../Includes/libARSAL/ARSAL_Sem.h        \
                                            ../Includes/libARSAL/ARSAL_Socket.h     \
                                            ../Includes/libARSAL/ARSAL_Print.h      \
                                            ../Includes/libARSAL/ARSAL_Endianness.h

# The sources to add to the library and to add to the source distribution
SOURCE_FILES                            =   $(HEADER_FILES)             \
                                            ../Sources/ARSAL_Time.c     \
                                            ../Sources/ARSAL_Thread.c   \
                                            ../Sources/ARSAL_Mutex.c    \
                                            ../Sources/ARSAL_Sem.c      \
                                            ../Sources/ARSAL_Socket.c   \
                                            ../Sources/ARSAL_Print.c

# The library names to build (note we are building static and shared libs)
if DEBUG_MODE
lib_LTLIBRARIES                         =   libarsal_dbg.la
if NO_SO_VERSION
libarsal_dbg_la_LDFLAGS                 =   -avoid-version
endif
libarsal_dbg_ladir                      =   $(includedir)/libARSAL
libarsal_dbg_la_HEADERS                 =   $(HEADER_FILES)
libarsal_dbg_la_SOURCES                 =   $(SOURCE_FILES)
else
lib_LTLIBRARIES                         =   libarsal.la
if NO_SO_VERSION
libarsal_la_LDFLAGS                     =   -avoid-version
endif
libarsal_ladir                          =   $(includedir)/libARSAL
libarsal_la_HEADERS                     =   $(HEADER_FILES)
libarsal_la_SOURCES                     =   $(SOURCE_FILES)
endif

check_PROGRAMS                          =   ../TestBench/test1          \
                                            ../TestBench/test2          \
                                            ../TestBench/testEndianness \
                                            ../TestBench/testSem

___TestBench_test1_SOURCES              =   ../TestBench/test1.c
___TestBench_test2_SOURCES              =   ../TestBench/test2.c
___TestBench_testEndianness_SOURCES     =   ../TestBench/testEndianness.c
___TestBench_testSem_SOURCES            =   ../TestBench/testSem.c
if DEBUG_MODE
___TestBench_test1_LDADD                =   libarsal_dbg.la
___TestBench_test2_LDADD                =   libarsal_dbg.la
___TestBench_testEndianness_LDADD       =   libarsal_dbg.la
___TestBench_testSem_LDADD              =   libarsal_dbg.la
else
___TestBench_test1_LDADD                =   libarsal.la
___TestBench_test2_LDADD                =   libarsal.la
___TestBench_testEndianness_LDADD       =   libarsal.la
___TestBench_testSem_LDADD              =   libarsal.la
endif

CLEAN_FILES                             =   libarsal.la \
                                            libarsal_dbg.la

# Ensure the distribution of the doxygen configuration file
EXTRA_DIST                              =   bootstrap

# Doxygen support
if HAVE_DOXYGEN
include doxygen.am
EXTRA_DIST                              +=  doxygen.ac  \
                                            doxygen.am  \
                                            $(DX_CONFIG)
endif
