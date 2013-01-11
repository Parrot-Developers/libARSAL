#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_INIT([libSAL], [1.0], [ardrone@ardrone.org])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(m4)

# automake initialisation and check for minimal automake API version 1.9
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# use the C compiler for the following checks
AC_LANG([C])

##########################################################################
# Debug compilation support
##########################################################################
AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
    [enable debug data generation (def=no)])],
    [debugit="$enableval"],
    [debugit=no])
AC_MSG_RESULT([$debugit])

if test x"$debugit" = x"yes"; then
    AC_DEFINE([DEBUG],[],[Debug Mode])
    CFLAGS+=" -g -Wall -Werror -Wno-uninitialized -O0"
else
    AC_DEFINE([NDEBUG],[],[No-debug Mode])
    CFLAGS+=" -O3"
fi
AM_CONDITIONAL([DEBUG_MODE], [test "$debugit" = "yes"])

##########################################################################
# Define includes directories
##########################################################################
CFLAGS+=" -I$srcdir/../Includes"

CPPFLAGS="$CFLAGS"

##########################################################################
# Check headers
##########################################################################
AC_CHECK_HEADERS([sys/time.h time.h stdio.h stdarg.h])

AC_CHECK_HEADERS([android/log.h], LIBS+=" -llog",)

# Checks for pthread.h
AC_CHECK_HEADERS([pthread.h], [
    AC_CHECK_FUNC(pthread_create, , [
        AC_CHECK_LIB(pthread, pthread_create,
        LIBS="$LIBS -pthread", AC_MSG_ERROR(The pthread library is required in order to build the library!))
    ])
], AC_MSG_ERROR(The pthread header is required in order to build the library!))

# Checks for semaphore.h
AC_CHECK_HEADERS([semaphore.h], , AC_MSG_ERROR(The semaphore header is required in order to build the library!))

#Check for semaphore functions
AC_CHECK_FUNCS([sem_init sem_destroy sem_wait sem_trywait sem_timedwait sem_post sem_getvalue], ,
	AC_MSG_WARN(Your semaphore implmentation does not support all needed functions --> Using custom one)
)

# Checks for socket.h
AC_CHECK_HEADERS([sys/socket.h netinet/in.h], , AC_MSG_ERROR(The socket header is required in order to build the library!))

# Checks for mach/mach_time.h
# TO CHECK
# AC_CHECK_HEADERS([mach/mach_time.h])

##########################################################################
# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

AC_CHECK_PROGS([DOXYGEN], [doxygen])
m4_include(doxygen.ac)
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([libSAL], [doxygen.cfg], [$srcdir/../Doc])
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# distribute additional compiler and linker flags
# --> set these variables instead of CFLAGS or LDFLAGS
AC_SUBST([CFLAGS])
AC_SUBST([LDFLAGS])

# Checks for library functions.


# Generates Makefile
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "
  ($PACKAGE_NAME) version $PACKAGE_VERSION
  Prefix.........: $prefix
  Debug Build....: $debugit
  C Compiler.....: $CC $CFLAGS
  Linker.........: $LD $LDFLAGS $LIBS
  Doxygen........: ${DOXYGEN:-NONE}
"

