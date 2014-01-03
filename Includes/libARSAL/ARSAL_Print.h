/**
 * @file libARSAL/ARSAL_Print.h
 * @brief This file contains headers about debug print abstraction layer
 * @date 06/01/2012
 * @author frederic.dhaeyer@parrot.com
 */
#ifndef _ARSAL_PRINT_H_
#define _ARSAL_PRINT_H_
#include <time.h>
#include <string.h>
#include <libARSAL/ARSAL_Time.h>

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
 * @brief Prints a specific output
 *
 * This is the default entry point for printing.
 * This function outputs a string of the following format:
 * `[LEVEL] TAG | HH:MM:SS:mmm | function:line - FORMAT`
 *
 * @param level The print level (eARSAL_PRINT_LEVEL enum)
 * @param tag A short tag which will prefix the log timestamp
 * @param format The format string to print
 * @param ... The format parameters
 */
#if defined(DEBUG)
#define ARSAL_PRINT(level, tag, format, ...)                            \
    do                                                                  \
    {                                                                   \
        if (ARSAL_PRINT_MAX > level)                                    \
        {                                                               \
            char __nowTimeStr [9];                                      \
            struct timeval __tv;                                        \
            struct tm __tm;                                             \
            gettimeofday (&__tv, NULL);                                 \
            localtime_r (&(__tv.tv_sec), &__tm);                        \
            strftime (__nowTimeStr, 9, "%H:%M:%S", &__tm);              \
            if (!strlen (format) || format[strlen (format)-1] != '\n')                      \
            {                                                           \
                ARSAL_PrintRaw(level, tag, "%s:%03d | %s:%d - " format "\n", __nowTimeStr, __tv.tv_usec / 1000, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            }                                                           \
            else                                                        \
            {                                                           \
                ARSAL_PrintRaw(level, tag, "%s:%03d | %s:%d - " format, __nowTimeStr, __tv.tv_usec / 1000, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            }                                                           \
        }                                                               \
    } while (0)
#else
#define ARSAL_PRINT(level, tag, format, ...)                            \
    do                                                                  \
    {                                                                   \
        if (ARSAL_PRINT_DEBUG > level)                                  \
        {                                                               \
            char __nowTimeStr [9];                                      \
            struct timeval __tv;                                        \
            struct tm __tm;                                             \
            gettimeofday (&__tv, NULL);                                 \
            localtime_r (&(__tv.tv_sec), &__tm);                        \
            strftime (__nowTimeStr, 9, "%H:%M:%S", &__tm);              \
            if (!strlen (format) || format[strlen (format)-1] != '\n')                      \
            {                                                           \
                ARSAL_PrintRaw(level, tag, "%s:%03d | %s:%d - " format "\n", __nowTimeStr, __tv.tv_usec / 1000, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            }                                                           \
            else                                                        \
            {                                                           \
                ARSAL_PrintRaw(level, tag, "%s:%03d | %s:%d - " format, __nowTimeStr, __tv.tv_usec / 1000, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            }                                                           \
        }                                                               \
    } while (0)
#endif

/**
 * @brief Prints a formatted output
 * @warning This function should not be used directly
 * @see ARSAL_PRINT()
 *
 * @param level The level of output
 * @param tag The tag of the output
 * @param format output format
 * @retval On success, ARSAL_PrintRaw() returns the number of characters printed. Otherwise, it returns a negative value.
 */
int ARSAL_PrintRaw(eARSAL_PRINT_LEVEL level, const char *tag, const char *format, ...);

#endif // _ARSAL_PRINT_H_
