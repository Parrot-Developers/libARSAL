/**
 * @file libSAL/mutex.c
 * @brief This file contains sources about mutex abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
 */
#include <stdlib.h>
#include <config.h>
#include <libSAL/mutex.h>
#include <libSAL/time.h>

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#else
#error The pthread.h header is required in order to build the library
#endif

int sal_mutex_init(sal_mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    pthread_mutex_t *pmutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    *mutex = (sal_mutex_t)pmutex;
    result = pthread_mutex_init((pthread_mutex_t *)*mutex, NULL);
#endif

    return result;
}

int sal_mutex_destroy(sal_mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_destroy((pthread_mutex_t *)*mutex);
    free(*mutex);
#endif

    return result;
}

int sal_mutex_lock(sal_mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_lock((pthread_mutex_t *)*mutex);
#endif

    return result;
}

int sal_mutex_trylock(sal_mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_trylock((pthread_mutex_t *)*mutex);
#endif

    return result;
}

int sal_mutex_unlock(sal_mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_unlock((pthread_mutex_t *)*mutex);
#endif

    return result;
}

int sal_cond_init(sal_cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    pthread_cond_t *pcond = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    *cond = (sal_cond_t)pcond;
    result = pthread_cond_init((pthread_cond_t *)*cond, NULL);
#endif

    return result;
}

int sal_cond_destroy(sal_cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_destroy((pthread_cond_t *)*cond);
    free(*cond);
#endif

    return result;
}

int sal_cond_wait(sal_cond_t *cond, sal_mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_wait((pthread_cond_t *)*cond, (pthread_mutex_t *)*mutex);
#endif

    return result;
}

int sal_cond_timedwait(sal_cond_t *cond, sal_mutex_t *mutex, int time_in_ms)
{
    int result = 0;
    struct timespec ts;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    TIMEVAL_TO_TIMESPEC(&tv, &ts);
    ts.tv_nsec += (time_in_ms % 1000) * 1000000;
    ts.tv_sec  += (time_in_ms / 1000);
    ts.tv_sec += ts.tv_nsec / 1000000000;
    ts.tv_nsec %= 1000000000;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_timedwait((pthread_cond_t *)*cond, (pthread_mutex_t *)*mutex, &ts);
#endif

    return result;
}

int sal_cond_signal(sal_cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_signal((pthread_cond_t *)*cond);
#endif

    return result;
}

int sal_cond_broadcast(sal_cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_broadcast((pthread_cond_t *)*cond);
#endif

    return result;
}
