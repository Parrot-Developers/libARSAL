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
    ARSAL_PRINT_FATAL,    /**< The fatal level, print on release and debug mode */
    ARSAL_PRINT_ERROR,    /**< The error level, print on release and debug mode */
    ARSAL_PRINT_WARNING,  /**< The warning level, print on release and debug mode */
    ARSAL_PRINT_INFO,     /**< The info level, print on release and debug mode */
    ARSAL_PRINT_DEBUG,    /**< The debug level, print on debug mode only */
    ARSAL_PRINT_VERBOSE,  /**< The verbose level, print on debug mode only */

    ARSAL_PRINT_MAX,      /**< The maximum of enum, do not use !*/
} eARSAL_PRINT_LEVEL;

#define     ARSAL_PRINT_DATE_STRING_LENGTH 9        // HH:MM:SS\0

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
            char __nowTimeStr [ARSAL_PRINT_DATE_STRING_LENGTH];         \
            struct timespec __ts;                                       \
            struct tm __tm;                                             \
            ARSAL_Time_GetLocalTime(&__ts, &__tm);                      \
            strftime (__nowTimeStr, ARSAL_PRINT_DATE_STRING_LENGTH, "%H:%M:%S", &__tm); \
            if (!strlen (format) || format[strlen (format)-1] != '\n')  \
            {                                                           \
                ARSAL_Print_PrintRaw(level, tag, "%s:%03d | %s:%d - " format "\n", __nowTimeStr, NSEC_TO_MSEC(__ts.tv_nsec), __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            }                                                           \
            else                                                        \
            {                                                           \
                ARSAL_Print_PrintRaw(level, tag, "%s:%03d | %s:%d - " format, __nowTimeStr, NSEC_TO_MSEC(__ts.tv_nsec), __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            }                                                           \
        }                                                               \
        /* NO ELSE : enum value can't be > level */                     \
    } while (0)
#else
#define ARSAL_PRINT(level, tag, format, ...)                            \
    do                                                                  \
    {                                                                   \
        if (ARSAL_PRINT_DEBUG > level)                                  \
        {                                                               \
            char __nowTimeStr [ARSAL_PRINT_DATE_STRING_LENGTH];         \
            struct timespec __ts;                                       \
            struct tm __tm;                                             \
            ARSAL_Time_GetLocalTime(&__ts, &__tm);                      \
            strftime (__nowTimeStr, ARSAL_PRINT_DATE_STRING_LENGTH, "%H:%M:%S", &__tm); \
            if (!strlen (format) || format[strlen (format)-1] != '\n')  \
            {                                                           \
                ARSAL_Print_PrintRaw(level, tag, "%s:%03d | %s:%d - " format "\n", __nowTimeStr, NSEC_TO_MSEC(__ts.tv_nsec), __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            }                                                           \
            else                                                        \
            {                                                           \
                ARSAL_Print_PrintRaw(level, tag, "%s:%03d | %s:%d - " format, __nowTimeStr, NSEC_TO_MSEC(__ts.tv_nsec), __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            }                                                           \
        }                                                               \
        /* NO ELSE : no print in debug mode */                          \
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
 * @retval On success, ARSAL_Print_PrintRaw() returns the number of characters printed. Otherwise, it returns a negative value.
 */
int ARSAL_Print_PrintRaw(eARSAL_PRINT_LEVEL level, const char *tag, const char *format, ...);

#include <stdarg.h>
typedef int (*ARSAL_Print_Callback_t) (eARSAL_PRINT_LEVEL level, const char *tag, const char *format, va_list va);
void ARSAL_Set_Print_Callback( ARSAL_Print_Callback_t callback);

#endif // _ARSAL_PRINT_H_
