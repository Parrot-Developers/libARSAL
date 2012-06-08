/**
 * \file libSAL/thread.c
 * \brief This file contains sources about thread abstraction layer
 * \date 05/18/2012
 * \author frederic.dhaeyer@parrot.com
*/
#include <stdlib.h>
#include <config.h>
#include <stdio.h>
#include <libSAL/thread.h>

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#else
#error The pthread.h header is required in order to build the library
#endif

int sal_thread_create(sal_thread_t *thread, sal_thread_routine routine, void *arg)
{
	int result = 0;

#if defined(HAVE_PTHREAD_H)
	pthread_t *pthread = (pthread_t *)malloc(sizeof(pthread_t));
	*thread = (sal_thread_t)pthread;
	result = pthread_create((pthread_t *)*thread, NULL, routine, arg);
#endif

	return result;
}

int sal_thread_join(sal_thread_t thread, void **retval)
{
	int result = 0;

#if defined(HAVE_PTHREAD_H)
	result = pthread_join((**(pthread_t **)thread), retval);
#endif

	return result;
}

int sal_thread_destroy(sal_thread_t *thread)
{
	int result = 0;

#if defined(HAVE_PTHREAD_H)
	free(*thread);
#endif

	return result;
}
