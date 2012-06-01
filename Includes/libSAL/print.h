/**
 * \file libSAL/print.h
 * \brief This file contains headers about debug print abstraction layer
 * \date 06/01/2012
 * \author frederic.dhaeyer@parrot.com 
*/
//------------------------------
#ifndef _PRINT_H_
#define _PRINT_H_

#define SAL_PRINT(...)	sal_printf("%s:%d - %s\n", __FUNCTION__, __LINE__, __VA_ARGS__)

/**
 * @fn int sal_printf(const char *format, ...)
 * @brief Convert a formatted output.
 *
 * @param format output format
 * @retval On success, sal_printf() returns the number of characters printed. Otherwise, it returns a negative value.
 */
int sal_printf(const char *format, ...);

#endif // _PRINT_H_
