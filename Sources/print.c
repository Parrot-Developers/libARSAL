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

int sal_printf(const char *format, ...)
{
	int result = -1;
	va_list va;
	va_start(va, format);
	result = vprintf(format, va);
	va_end(va);

	return result;
}
