/**
 * @file libARSAL/ARSAL_Thread.c
 * @brief This file contains sources about thread abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
 */
#include <stdlib.h>
#include <config.h>
#include <stdio.h>
#include <libARSAL/ARSAL_Thread.h>
#include <libARSAL/ARSAL_Print.h>

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#else
#error The pthread.h header is required in order to build the library
#endif

int ARSAL_Thread_Create(ARSAL_Thread_t *thread, ARSAL_Thread_Routine_t routine, void *arg)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    pthread_t *pthread = (pthread_t *)malloc(sizeof(pthread_t));
    *thread = (ARSAL_Thread_t)pthread;
    result = pthread_create((pthread_t *)*thread, NULL, routine, arg);
#endif

    return result;
}

int ARSAL_Thread_Join(ARSAL_Thread_t thread, void **retval)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_join((*(pthread_t *)thread), retval);
#endif

    return result;
}

int ARSAL_Thread_Destroy(ARSAL_Thread_t *thread)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    free(*thread);
#endif

    return result;
}
