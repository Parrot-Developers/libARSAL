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

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>

#define ARSAL_Mutex_NORMAL PTHREAD_MUTEX_NORMAL
#define ARSAL_Mutex_ERRORCHECK PTHREAD_MUTEX_ERRORCHECK
#define ARSAL_Mutex_RECURSIVE PTHREAD_MUTEX_RECURSIVE
#define ARSAL_Mutex_DEFAULT PTHREAD_MUTEX_DEFAULT

#else
#error The pthread.h header is required in order to build the library
#endif

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

int ARSAL_Mutex_InitWithType(ARSAL_Mutex_t *mutex, eARSAL_MUTEX_TYPE type)
{
    int result = 0;
#if defined(HAVE_PTHREAD_H)
    pthread_mutexattr_t attr;
    int pthreadMutexType = PTHREAD_MUTEX_DEFAULT;

    /** set pthreadMutexType frome type */
    switch(type)
    {
        case ARSAL_MUTEX_TYPE_NORMAL:
            pthreadMutexType = PTHREAD_MUTEX_NORMAL;
            break;
        
        case ARSAL_MUTEX_TYPE_ERRORCHECK:
            pthreadMutexType = PTHREAD_MUTEX_ERRORCHECK;
            break;
        
        case ARSAL_MUTEX_TYPE_RECURSIVE:
            pthreadMutexType = PTHREAD_MUTEX_RECURSIVE;
            break;
        
        case PTHREAD_MUTEX_DEFAULT:
            pthreadMutexType = PTHREAD_MUTEX_DEFAULT;
            break;
        
        default:
            pthreadMutexType = PTHREAD_MUTEX_DEFAULT;
            break;
    }
    
    /** Initializes the attribute */
    result = pthread_mutexattr_settype(&attr, pthreadMutexType); 
    
    if(result == 0)
    {
        /** Allocates the mutex */
        pthread_mutex_t *pmutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        *mutex = (ARSAL_Mutex_t)pmutex;
        
        /** Initializes the mutex with the attribute */
        result = pthread_mutex_init((pthread_mutex_t *)*mutex, &attr);
    }
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

int ARSAL_Cond_Timedwait(ARSAL_Cond_t *cond, ARSAL_Mutex_t *mutex, int time_in_ms)
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
