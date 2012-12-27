/**
 * @file libSAL/sem.c
 * @brief This file contains sources about semaphore abstraction layer
 * @date 12/27/2012
 * @author nicolas.brulez@parrot.com
 */
#include <stdlib.h>
#include <config.h>
#include <libSAL/sem.h>
#include <libSAL/time.h>
#include <errno.h>

#if defined(HAVE_SEMAPHORE_H)
#include <semaphore.h>
#else
#error The semaphore.h header is required in order to build the library
#endif

int sal_sem_init(sal_sem_t *sem, int shared, int value)
{
    int result = -1;

#if defined(HAVE_SEMAPHORE_H)
    sem_t *psem = (sem_t *) malloc (sizeof (sem_t));
    if (NULL != psem)
    {
        result = sem_init(psem, shared, value);
        if (0 == result)
        {
            *sem = (sal_sem_t)psem;
        }
        else
        {
            free (psem);
            psem = NULL;
        }
    }
#endif

    return result;
}

int sal_sem_destroy(sal_sem_t *sem)
{
    int result = -1;

#if defined(HAVE_SEMAPHORE_H)
    errno = 0;
    result = sem_destroy((sem_t *)*sem);
    free(*sem);
#endif

    return result;
}

int sal_sem_wait(sal_sem_t *sem)
{
    int result = -1;

#if defined(HAVE_SEMAPHORE_H)
    result = sem_wait((sem_t *)*sem);
#endif

    return result;
}

int sal_sem_trywait(sal_sem_t *sem)
{
    int result = -1;

#if defined(HAVE_SEMAPHORE_H)
    result = sem_trywait((sem_t *)*sem);
#endif

    return result;
}

int sal_sem_timedwait(sal_sem_t *sem, const struct timespec *timeout)
{
    int result = -1;

#if defined(HAVE_SEMAPHORE_H)
    const struct timeval currentTime = {0};
    struct timespec finalTime = {0};
    gettimeofday (&currentTime, NULL);
    TIMEVAL_TO_TIMESPEC (&currentTime, &finalTime);
    finalTime.tv_nsec += timeout->tv_nsec;
#define NSEC_TO_SEC (1000000000)
    finalTime.tv_sec += timeout->tv_sec + (finalTime.tv_nsec / NSEC_TO_SEC);
    finalTime.tv_nsec %= NSEC_TO_SEC;
#undef NSEC_TO_SEC
    result = sem_timedwait((sem_t *)*sem, &finalTime);
#endif

    return result;
}

int sal_sem_post(sal_sem_t *sem)
{
    int result = -1;

#if defined(HAVE_SEMAPHORE_H)
    result = sem_post ((sem_t *)*sem);
#endif

    return result;
}

int sal_sem_getvalue(sal_sem_t *sem, int *value)
{
    int result = -1;

    if (NULL == value)
    {
        errno = EINVAL;
        return result;
    }

#if defined(HAVE_SEMAPHORE_H)
    result = sem_getvalue((sem_t *)*sem, value);
#endif

    return result;
}
