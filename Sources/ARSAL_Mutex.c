/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the 
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/

/* generic condition implementation copied from SDL 2 and adapted to use
 * ARSAL stuff.
 * SDL Licence:
 *  Simple DirectMedia Layer
 *  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 */

/**
 * @file libARSAL/ARSAL_Mutex.c
 * @brief This file contains sources about mutex abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
 */
#include <stdlib.h>
#include <config.h>
#include <libARSAL/ARSAL_Mutex.h>
#include <libARSAL/ARSAL_Thread.h>
#include <libARSAL/ARSAL_Sem.h>
#include <libARSAL/ARSAL_Time.h>
#include <libARSAL/ARSAL_Print.h>
#if HAVE_DECL_SYS_GETTID
#include <sys/syscall.h>
#endif
#include <errno.h>

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#endif

/* Genric mutex implementation */
struct ARSAL_Mutex_Implementation
{
    ARSAL_Sem_t sem;
    int owner;
};

typedef struct ARSAL_Mutex_Implementation ARSAL_Mutex_Implementation;

static int ARSAL_Mutex_Implementation_Init(ARSAL_Mutex_Implementation *mutex)
{
    if (ARSAL_Sem_Init(&mutex->sem, 0, 1) < 0)
    {
        return -1;
    }

    mutex->owner = 0;

    return 0;
}

static int ARSAL_Mutex_Implementation_Destroy(ARSAL_Mutex_Implementation *mutex)
{
    ARSAL_Sem_Destroy(&mutex->sem);
    return 0;
}

static int ARSAL_Mutex_Implementation_Lock(ARSAL_Mutex_Implementation *mutex)
{
    int thread_id;

    thread_id = ARSAL_Thread_GetThreadID();
    if (thread_id == mutex->owner)
    {
        return -1;
    }

    if (ARSAL_Sem_Wait(&mutex->sem) < 0)
    {
        return -1;
    }

    /* to prevent other thread to unlock the mutex */
    mutex->owner = thread_id;

    return 0;
}

static int ARSAL_Mutex_Implementation_Trylock(ARSAL_Mutex_Implementation *mutex)
{
    int thread_id;

    thread_id = ARSAL_Thread_GetThreadID();
    if (thread_id == mutex->owner)
    {
        return -1;
    }

    if (ARSAL_Sem_Trywait(&mutex->sem) < 0)
    {
        return -1;
    }

    /* to prevent other thread to unlock the mutex */
    mutex->owner = thread_id;

    return 0;
}

static int ARSAL_Mutex_Implementation_Unlock(ARSAL_Mutex_Implementation *mutex)
{
    int thread_id;

    thread_id = ARSAL_Thread_GetThreadID();
    if (mutex->owner != thread_id)
    {
        errno = EPERM;
        return -1;
    }

    mutex->owner = 0;

    return ARSAL_Sem_Post(&mutex->sem);
}


/* Generic condition implementation */
struct ARSAL_Cond_Implementation
{
    ARSAL_Mutex_t lock;
    int waiting;
    int signals;
    ARSAL_Sem_t wait_sem;
    ARSAL_Sem_t wait_done;
};

typedef struct ARSAL_Cond_Implementation ARSAL_Cond_Implementation;

static int ARSAL_Cond_Implementation_Init(ARSAL_Cond_Implementation *cond)
{
    int ret;

    ret = ARSAL_Mutex_Init(&cond->lock);
    if (ret < 0)
    {
        goto done;
    }

    ret = ARSAL_Sem_Init(&cond->wait_sem, 0, 0);
    if (ret < 0)
    {
        ARSAL_Mutex_Destroy(&cond->lock);
        goto done;
    }

    ret = ARSAL_Sem_Init(&cond->wait_done, 0, 0);
    if (ret < 0)
    {
        ARSAL_Sem_Destroy(&cond->wait_sem);
        ARSAL_Mutex_Destroy(&cond->lock);
        goto done;
    }

    cond->waiting = cond->signals = 0;

done:
    return ret;
}

static int ARSAL_Cond_Implementation_Destroy(ARSAL_Cond_Implementation *cond)
{
    ARSAL_Mutex_Destroy(&cond->lock);
    ARSAL_Sem_Destroy(&cond->wait_sem);
    ARSAL_Sem_Destroy(&cond->wait_done);
    return 0;
}

static int ARSAL_Cond_Implementation_TimedWait(ARSAL_Cond_Implementation *cond,
        ARSAL_Mutex_t *mutex, int timeout_ms)
{
    int ret;
    int have_timeout = 0;

    if (!cond)
    {
        errno = EINVAL;
        return -1;
    }

    /* Obtain the protection mutex, and increment the number of waiters.
     * This allows the signal mechanism to only perform a signal if there
     * are waiting threads.
     */
    ARSAL_Mutex_Lock(&cond->lock);
    ++cond->waiting;
    ARSAL_Mutex_Unlock(&cond->lock);

    /* Unlock the mutex, as is required by condition variable semantics */
    ARSAL_Mutex_Unlock(mutex);

    /* Wait for a signal */
    if (timeout_ms == -1)
    {
        ret = ARSAL_Sem_Wait(&cond->wait_sem);
    }
    else
    {
        struct timespec timeout;

        timeout.tv_sec = MSEC_TO_SEC(timeout_ms);
        timeout.tv_nsec = MSEC_TO_NSEC(timeout_ms % SEC_TO_MSEC(1));

        ret = ARSAL_Sem_Timedwait(&cond->wait_sem, &timeout);
    }

    if (ret < 0)
    {
        if (errno == ETIMEDOUT)
        {
            have_timeout = 1;
        }
        else
        {
            goto done;
        }
    }

    /* Let the signaler know we have completed the wait, otherwise
     * the signaler can race ahead and get the condition semaphore
     * if we are stopped between the mutex unlock and semaphore wait,
     * giving a deadlock.  See the following URL for details:
     * http://www-classic.be.com/aboutbe/benewsletter/volume_III/Issue40.html
     */
    ARSAL_Mutex_Lock(&cond->lock);
    if (cond->signals > 0)
    {
        /* If we timed out, we need to eat a condition signal */
        if (have_timeout)
        {
            ARSAL_Sem_Wait(&cond->wait_sem);
        }

        /* We always notify the signal thread that we are done */
        ARSAL_Sem_Post(&cond->wait_done);

        /* Signal handshake complete */
        --cond->signals;
    }

    --cond->waiting;
    ARSAL_Mutex_Unlock(&cond->lock);

    /* Lock the mutex, as is required by condition variable semantics */
    ARSAL_Mutex_Lock(mutex);

done:
    return ret;
}

static int ARSAL_Cond_Implementation_Wait(ARSAL_Cond_Implementation *cond,
        ARSAL_Mutex_t *mutex)
{
    return ARSAL_Cond_Implementation_TimedWait(cond, mutex, -1);
}

static int ARSAL_Cond_Implementation_Signal(ARSAL_Cond_Implementation *cond)
{
    if (!cond)
    {
        errno = EINVAL;
        return -1;
    }

    /* If there are waiting threads not already signalled, then
     * signal the condition and wait for the thread to respond.
     */
    ARSAL_Mutex_Lock(&cond->lock);
    if (cond->waiting > cond->signals)
    {
        ++cond->signals;
        ARSAL_Sem_Post(&cond->wait_sem);
        ARSAL_Mutex_Unlock(&cond->lock);
        ARSAL_Sem_Wait(&cond->wait_done);
    }
    else
    {
        ARSAL_Mutex_Unlock(&cond->lock);
    }

    return 0;
}

static int ARSAL_Cond_Implementation_Broadcast(ARSAL_Cond_Implementation *cond)
{
    if (!cond)
    {
        errno = EINVAL;
        return -1;
    }

    /* If there are waiting threads not already signalled, then
     * signal the condition and wait for the thread to respond.
     */
    ARSAL_Mutex_Lock(&cond->lock);
    if (cond->waiting > cond->signals)
    {
        int i, num_waiting;

        num_waiting = (cond->waiting - cond->signals);
        cond->signals = cond->waiting;
        for (i = 0; i < num_waiting; ++i)
        {
            ARSAL_Sem_Post(&cond->wait_sem);
        }

        /* Now all released threads are blocked here, waiting for us.
         * Collect them all (and win fabulous prizes!) :-)
         */
        ARSAL_Mutex_Unlock(&cond->lock);
        for (i = 0; i < num_waiting; ++i)
        {
            ARSAL_Sem_Wait(&cond->wait_done);
        }
    }
    else
    {
        ARSAL_Mutex_Unlock(&cond->lock);
    }

    return 0;
}


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
#else
    ARSAL_Mutex_Implementation *i_mutex;

    i_mutex = malloc(sizeof(ARSAL_Mutex_Implementation));
    *mutex = (ARSAL_Mutex_t) i_mutex;
    result = ARSAL_Mutex_Implementation_Init(i_mutex);
#endif

    return result;
}

int ARSAL_Mutex_Destroy(ARSAL_Mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_destroy((pthread_mutex_t *)*mutex);
    free(*mutex);
#else
    ARSAL_Mutex_Implementation *i_mutex = (ARSAL_Mutex_Implementation *) *mutex;

    result = ARSAL_Mutex_Implementation_Destroy(i_mutex);
    free(i_mutex);
#endif

    return result;
}

int ARSAL_Mutex_Lock(ARSAL_Mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_lock((pthread_mutex_t *)*mutex);
    if (result != 0)
    {
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex operation failed! errno = %d , %s ; thread_id = %d",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    syscall(SYS_gettid)
#else
                    0
#endif
            );
    }
#else
    ARSAL_Mutex_Implementation *i_mutex = (ARSAL_Mutex_Implementation *) *mutex;

    result = ARSAL_Mutex_Implementation_Lock(i_mutex);
#endif

    return result;
}

int ARSAL_Mutex_Trylock(ARSAL_Mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_trylock((pthread_mutex_t *)*mutex);
    if ( (result != 0) && (result != EBUSY) )
    {
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex operation failed! errno = %d , %s ; thread_id = %d",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    syscall(SYS_gettid)
#else
                    0
#endif
            );
    }
#else
    ARSAL_Mutex_Implementation *i_mutex = (ARSAL_Mutex_Implementation *) *mutex;

    result = ARSAL_Mutex_Implementation_Trylock(i_mutex);
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
#if HAVE_DECL_SYS_GETTID
                    syscall(SYS_gettid)
#else
                    0
#endif
            );
    }
#else
    ARSAL_Mutex_Implementation *i_mutex = (ARSAL_Mutex_Implementation *) *mutex;

    result = ARSAL_Mutex_Implementation_Unlock(i_mutex);
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
#else
    ARSAL_Cond_Implementation *i_cond = malloc(sizeof(*i_cond));
    *cond = (ARSAL_Cond_t) i_cond;
    result = ARSAL_Cond_Implementation_Init(i_cond);
#endif

    return result;
}

int ARSAL_Cond_Destroy(ARSAL_Cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_destroy((pthread_cond_t *)*cond);
    free(*cond);
#else
    ARSAL_Cond_Implementation *i_cond = (ARSAL_Cond_Implementation *) *cond;
    result = ARSAL_Cond_Implementation_Destroy(i_cond);
    free(i_cond);
#endif

    return result;
}

int ARSAL_Cond_Wait(ARSAL_Cond_t *cond, ARSAL_Mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_wait((pthread_cond_t *)*cond, (pthread_mutex_t *)*mutex);
    if (result != 0)
    {
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex/Cond operation failed! errno = %d , %s ; thread_id = %d",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    syscall(SYS_gettid)
#else
                    0
#endif
            );
    }
#else
    ARSAL_Cond_Implementation *i_cond = (ARSAL_Cond_Implementation *) *cond;
    result = ARSAL_Cond_Implementation_Wait(i_cond, mutex);
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
    if ( (result != 0) && (result != ETIMEDOUT) )
    {
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex/Cond operation failed! errno = %d , %s ; thread_id = %d",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    syscall(SYS_gettid)
#else
                    0
#endif
            );
    }
#else
    ARSAL_Cond_Implementation *i_cond = (ARSAL_Cond_Implementation *) *cond;
    result = ARSAL_Cond_Implementation_TimedWait(i_cond, mutex, timeout);
#endif

    return result;
}

int ARSAL_Cond_Signal(ARSAL_Cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_signal((pthread_cond_t *)*cond);
    if (result != 0)
    {
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex/Cond operation failed! errno = %d , %s ; thread_id = %d",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    syscall(SYS_gettid)
#else
                    0
#endif
            );
    }
#else
    ARSAL_Cond_Implementation *i_cond = (ARSAL_Cond_Implementation *) *cond;
    result = ARSAL_Cond_Implementation_Signal(i_cond);
#endif

    return result;
}

int ARSAL_Cond_Broadcast(ARSAL_Cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_broadcast((pthread_cond_t *)*cond);
    if (result != 0)
    {
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex/Cond operation failed! errno = %d , %s ; thread_id = %d",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    syscall(SYS_gettid)
#else
                    0
#endif
            );
    }
#else
    ARSAL_Cond_Implementation *i_cond = (ARSAL_Cond_Implementation *) *cond;
    result = ARSAL_Cond_Implementation_Broadcast(i_cond);
#endif

    return result;
}
