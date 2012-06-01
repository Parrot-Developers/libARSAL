/**
 * \file libSAL/print.h
 * \brief This file contains headers about debug print abstraction layer
 * \date 06/01/2012
 * \author frederic.dhaeyer@parrot.com 
*/
//------------------------------
#ifndef _PRINT_H_
#define _PRINT_H_
#include <time.h>

/**
 * @enum ePRINT_LEVEL
 * @brief Output level
*/
typedef enum
{
	PRINT_ERROR, 	/**< The error level, print on release and debug mode */
	PRINT_WARNING,	/**< The error level, print on release and debug mode */
	PRINT_DEBUG,	/**< The error level, print on debug mode only */
	PRINT_MAX,		/**< The maximum of enum, do not use !*/
} ePRINT_LEVEL;

extern const char *sal_prefix_table[];

/**
 * @fn SAL_PRINT(...)
 * @brief print a specific output (i.e. "[ERR] 121545444:main:10 - My debug log")
*/
#define SAL_PRINT(level, ...) sal_print(level, "%s %d:%s:%d - %s\n", sal_prefix_table[level], time(NULL), __FUNCTION__, __LINE__, __VA_ARGS__)

/**
 * @fn int sal_printf(const char *format, ...)
 * @brief Convert a formatted output.
 *
 * @param format output format
 * @retval On success, sal_printf() returns the number of characters printed. Otherwise, it returns a negative value.
 */
int sal_print(ePRINT_LEVEL level, const char *format, ...);

#endif // _PRINT_H_
