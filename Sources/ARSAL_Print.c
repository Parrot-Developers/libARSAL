/**
 * @file libARSAL/ARSAL_Print.c
 * @brief This file contains sources about debug print abstraction layer
 * @date 06/01/2012
 * @author frederic.dhaeyer@parrot.com
 */
#include <config.h>
#ifdef HAVE_ANDROID_LOG_H
#include <android/log.h>
#else
#endif
#include <stdio.h>
#include <stdarg.h>
#include <libARSAL/ARSAL_Print.h>

const char* cARSAL_Print_prefixTable[ARSAL_PRINT_MAX] =
{
    [ARSAL_PRINT_ERROR]   = "[ERR]",
    [ARSAL_PRINT_WARNING] = "[WNG]",
    [ARSAL_PRINT_DEBUG]   = "[DBG]"
};

#ifdef HAVE_ANDROID_LOG_H
int ARSAL_PrintRaw(eARSAL_PRINT_LEVEL level, const char *tag, const char *format, ...)
{
    int result = -1;

    va_list va;
    va_start(va, format);

    switch(level)
    {
    case ARSAL_PRINT_ERROR:
        result = __android_log_vprint (ANDROID_LOG_ERROR, tag, format, va);
        break;
    case ARSAL_PRINT_WARNING:
        result = __android_log_vprint (ANDROID_LOG_WARN, tag, format, va);
        break;
    case ARSAL_PRINT_DEBUG:
        result = __android_log_vprint (ANDROID_LOG_DEBUG, tag, format, va);
        break;

    default:
        break;
    }

    va_end(va);
    return result;
}
#else
int ARSAL_PrintRaw(eARSAL_PRINT_LEVEL level, const char *tag, const char *format, ...)
{
    int result = -1;

    va_list va;
    va_start(va, format);

    switch(level)
    {
    case ARSAL_PRINT_ERROR:
    case ARSAL_PRINT_WARNING:
    case ARSAL_PRINT_DEBUG:
        printf ("%s %s | ", cARSAL_Print_prefixTable [level], tag);
        result = vprintf(format, va);
        break;

    default:
        break;
    }

    va_end(va);
    return result;
}
#endif
