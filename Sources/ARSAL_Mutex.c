/**
 * @file libARSAL/ARSAL_Mutex.c
 * @brief This file contains sources about mutex abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
 */
#include <stdlib.h>
#include <config.h>
#include <libARSAL/ARSAL_Mutex.h>
#include <libARSAL/ARSAL_Time.h>
#include <libARSAL/ARSAL_Print.h>
#include <sys/syscall.h>

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#else
#error The pthread.h header is required in order to build the library
#endif

/**
 * Tag for ARSAL_PRINT
 */
#define ARSAL_MUTEX_TAG "ARSAL_Mutex"

int ARSAL_Mutex_Init(ARSAL_Mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    pthread_mutex_t *pmutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    *mutex = (ARSAL_Mutex_t)pmutex;
    result = pthread_mutex_init((pthread_mutex_t *)*mutex, NULL);
#endif

    return result;
}

int ARSAL_Mutex_Destroy(ARSAL_Mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_destroy((pthread_mutex_t *)*mutex);
    free(*mutex);
#endif

    return result;
}

int ARSAL_Mutex_Lock(ARSAL_Mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_lock((pthread_mutex_t *)*mutex);
#endif

    return result;
}

int ARSAL_Mutex_Trylock(ARSAL_Mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_trylock((pthread_mutex_t *)*mutex);
#endif

    return result;
}

int ARSAL_Mutex_Unlock(ARSAL_Mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_unlock((pthread_mutex_t *)*mutex);
    if (result != 0)
    {
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex operation failed! errno = %d , %s ; thread_id = %d",
                result,
                strerror(result),
                syscall(SYS_gettid));
    }
#endif

    return result;
}

int ARSAL_Cond_Init(ARSAL_Cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    pthread_cond_t *pcond = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    *cond = (ARSAL_Cond_t)pcond;
    result = pthread_cond_init((pthread_cond_t *)*cond, NULL);
#endif

    return result;
}

int ARSAL_Cond_Destroy(ARSAL_Cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_destroy((pthread_cond_t *)*cond);
    free(*cond);
#endif

    return result;
}

int ARSAL_Cond_Wait(ARSAL_Cond_t *cond, ARSAL_Mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_wait((pthread_cond_t *)*cond, (pthread_mutex_t *)*mutex);
#endif

    return result;
}

int ARSAL_Cond_Timedwait(ARSAL_Cond_t *cond, ARSAL_Mutex_t *mutex, int timeout)
{
    int result = 0;
    struct timespec ts;
    ARSAL_Time_GetLocalTime(&ts, NULL);
    ts.tv_nsec += MSEC_TO_NSEC(timeout % SEC_TO_MSEC(1));
    ts.tv_sec  += MSEC_TO_SEC(timeout);
    ts.tv_sec  += NSEC_TO_SEC(ts.tv_nsec);
    ts.tv_nsec %= SEC_TO_NSEC(1);

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_timedwait((pthread_cond_t *)*cond, (pthread_mutex_t *)*mutex, &ts);
#endif

    return result;
}

int ARSAL_Cond_Signal(ARSAL_Cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_signal((pthread_cond_t *)*cond);
#endif

    return result;
}

int ARSAL_Cond_Broadcast(ARSAL_Cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_broadcast((pthread_cond_t *)*cond);
#endif

    return result;
}
