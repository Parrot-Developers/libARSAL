/**
 * @file libARSAL/ARSAL_Print.h
 * @brief This file contains headers about debug print abstraction layer
 * @date 06/01/2012
 * @author frederic.dhaeyer@parrot.com
 */
#ifndef _ARSAL_PRINT_H_
#define _ARSAL_PRINT_H_
#include <time.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/**
 * @brief Output level
 */
typedef enum
{
    ARSAL_PRINT_ERROR,    /**< The error level, print on release and debug mode */
    ARSAL_PRINT_WARNING,  /**< The error level, print on release and debug mode */
    ARSAL_PRINT_DEBUG,    /**< The error level, print on debug mode only */
    ARSAL_PRINT_MAX,      /**< The maximum of enum, do not use !*/
} eARSAL_PRINT_LEVEL;

/**
 * @brief print a specific output (i.e. "[ERR] TAG | 14:30:24 | main:10 - My debug log")
 */
#if defined(DEBUG)
#define ARSAL_PRINT(level, tag, format, ...)                            \
    do                                                                  \
    {                                                                   \
        char __nowTimeStr [9];                                          \
        time_t __nowTimeT = time (NULL);                                \
        strftime (__nowTimeStr, 9, "%H:%M:%S", localtime (&__nowTimeT)); \
        ARSAL_PrintRaw(level, tag, "%s | %s:%d - " format, __nowTimeStr, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#else
#define ARSAL_PRINT(level, tag, format, ...)                            \
    do                                                                  \
    {                                                                   \
        if (ARSAL_PRINT_DEBUG > level)                                  \
        {                                                               \
            char __nowTimeStr [9];                                      \
            time_t __nowTimeT = time (NULL);                            \
            strftime (__nowTimeStr, 9, "%H:%M:%S", localtime (&__nowTimeT)); \
            ARSAL_PrintRaw(level, tag, "%s | %s:%d - " format, __nowTimeStr, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        }                                                               \
    } while (0)
#endif

/**
 * @brief Convert a formatted output.
 *
 * @param level The level of output format
 * @param format output format
 * @retval On success, sal_print() returns the number of characters printed. Otherwise, it returns a negative value.
 */
int ARSAL_PrintRaw(eARSAL_PRINT_LEVEL level, const char *tag, const char *format, ...);

#endif // _ARSAL_PRINT_H_
