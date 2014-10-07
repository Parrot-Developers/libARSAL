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
    [ARSAL_PRINT_FATAL]   = "[WTF]",
    [ARSAL_PRINT_ERROR]   = "[ERR]",
    [ARSAL_PRINT_WARNING] = "[WNG]",
    [ARSAL_PRINT_INFO]    = "[INF]",
    [ARSAL_PRINT_DEBUG]   = "[DBG]",
    [ARSAL_PRINT_VERBOSE] = "[VRB]",
};

#ifdef HAVE_ANDROID_LOG_H
static int ARSAL_Print_PrintRaw_VA(eARSAL_PRINT_LEVEL level, const char *tag, const char *format, va_list va)
{
    int result = -1;

    switch(level)
    {
    case ARSAL_PRINT_FATAL:
        result = __android_log_vprint (ANDROID_LOG_FATAL, tag, format, va);
        break;
    case ARSAL_PRINT_ERROR:
        result = __android_log_vprint (ANDROID_LOG_ERROR, tag, format, va);
        break;
    case ARSAL_PRINT_WARNING:
        result = __android_log_vprint (ANDROID_LOG_WARN, tag, format, va);
        break;
    case ARSAL_PRINT_INFO:
        result = __android_log_vprint (ANDROID_LOG_INFO, tag, format, va);
        break;
    case ARSAL_PRINT_DEBUG:
        result = __android_log_vprint (ANDROID_LOG_DEBUG, tag, format, va);
        break;
    case ARSAL_PRINT_VERBOSE:
        result = __android_log_vprint (ANDROID_LOG_VERBOSE, tag, format, va);
        break;

    default:
        break;
    }

    return result;
}
#else

static int ARSAL_Print_PrintRaw_VA(eARSAL_PRINT_LEVEL level, const char *tag, const char *format, va_list va)
{
    int result = -1;

    switch(level)
    {
    case ARSAL_PRINT_ERROR:
    case ARSAL_PRINT_FATAL:
        fprintf (stderr, "%s %s | ", cARSAL_Print_prefixTable [level], tag);
        result = vfprintf(stderr, format, va);
        break;
    case ARSAL_PRINT_WARNING:
    case ARSAL_PRINT_INFO:
    case ARSAL_PRINT_DEBUG:
    case ARSAL_PRINT_VERBOSE:
        printf ("%s %s | ", cARSAL_Print_prefixTable [level], tag);
        result = vprintf(format, va);
        break;

    default:
        break;
    }

    return result;
}
#endif

ARSAL_Print_Callback_t ARSAL_Print_Callback = NULL;
void ARSAL_Print_SetCallback( ARSAL_Print_Callback_t callback)
{
    ARSAL_Print_Callback = callback;
}

int ARSAL_Print_PrintRaw(eARSAL_PRINT_LEVEL level, const char *tag, const char *format, ...)
{
    int result = -1;
    va_list va;

    va_start(va, format);
    {
        if ( ARSAL_Print_Callback == NULL )
            result = ARSAL_Print_PrintRaw_VA(level, tag, format, va);
        else
            result = ARSAL_Print_Callback(level, tag, format, va);
    }
    va_end(va);

    return result;
}
