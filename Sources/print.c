/**
 * \file libSAL/print.c
 * \brief This file contains sources about debug print abstraction layer
 * \date 06/01/2012
 * \author frederic.dhaeyer@parrot.com
 */
#include <config.h>
#include <stdio.h>
#include <stdarg.h>
#include <libSAL/print.h>

const char* sal_prefix_table[PRINT_MAX] =
{
    [PRINT_ERROR]   = "[ERR]",
    [PRINT_WARNING] = "[WNG]",
    [PRINT_DEBUG]   = "[DBG]"
};

int sal_print(ePRINT_LEVEL level, const char *format, ...)
{
    int result = -1;

    va_list va;
    va_start(va, format);

    switch(level)
    {
    case PRINT_ERROR:
    case PRINT_WARNING:
#if defined(DEBUG)
    case PRINT_DEBUG:
#endif
        result = vprintf(format, va);
        break;

    default:
        break;
    }

    va_end(va);
    return result;
}
