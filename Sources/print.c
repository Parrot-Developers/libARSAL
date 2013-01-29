/**
 * \file libSAL/print.c
 * \brief This file contains sources about debug print abstraction layer
 * \date 06/01/2012
 * \author frederic.dhaeyer@parrot.com
 */
#include <config.h>
#ifdef HAVE_ANDROID_LOG_H
#include <android/log.h>
#else
#endif
#include <stdio.h>
#include <stdarg.h>
#include <libSAL/print.h>

const char* sal_prefix_table[PRINT_MAX] =
{
    [PRINT_ERROR]   = "[ERR]",
    [PRINT_WARNING] = "[WNG]",
    [PRINT_DEBUG]   = "[DBG]"
};

#ifdef HAVE_ANDROID_LOG_H
int sal_print(ePRINT_LEVEL level, const char *tag, const char *format, ...)
{
    int result = -1;

    va_list va;
    va_start(va, format);

    switch(level)
    {
    case PRINT_ERROR:
        result = __android_log_vprint (ANDROID_LOG_ERROR, tag, format, va);
        break;
    case PRINT_WARNING:
        result = __android_log_vprint (ANDROID_LOG_WARN, tag, format, va);
        break;
    case PRINT_DEBUG:
        result = __android_log_vprint (ANDROID_LOG_DEBUG, tag, format, va);
        break;

    default:
        break;
    }

    va_end(va);
    return result;
}
#else
int sal_print(ePRINT_LEVEL level, const char *tag, const char *format, ...)
{
    int result = -1;

    va_list va;
    va_start(va, format);

    switch(level)
    {
    case PRINT_ERROR:
    case PRINT_WARNING:
    case PRINT_DEBUG:
        printf ("%s %s | ", sal_prefix_table [level], tag);
        result = vprintf(format, va);
        break;

    default:
        break;
    }

    va_end(va);
    return result;
}
#endif
