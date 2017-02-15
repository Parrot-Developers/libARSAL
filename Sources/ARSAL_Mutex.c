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

#if HAVE_DECL_SYS_GETTID
#include <sys/syscall.h>
#endif
#include <errno.h>

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#elif defined(HAVE_WINDOWS_H)
#include <windows.h>
#else
#error No mutant library found!
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
#elif defined(HAVE_WINDOWS_H)
    HANDLE hmutex = CreateMutex(NULL, FALSE, NULL);
    if (mutex == NULL)
    {
        result = -1;
    }

    *mutex = hmutex;
#endif

    return result;
}

int ARSAL_Mutex_Destroy(ARSAL_Mutex_t *mutex)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_mutex_destroy((pthread_mutex_t *)*mutex);
    free(*mutex);
#elif defined(HAVE_WINDOWS_H)
    CloseHandle((HANDLE)*mutex);
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
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex operation failed! errno = %d , %s ; thread_id = %ld",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    (long)syscall(SYS_gettid)
#else
                    0L
#endif
            );
    }
#elif defined(HAVE_WINDOWS_H)
    result = WaitForSingleObject((HANDLE)*mutex, INFINITE);
    if (result != WAIT_OBJECT_0) {
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG,
                    "Mutex operation failed! WaitForSingleObject ret %d", result);
    } else {
        result = 0;
    }
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
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex operation failed! errno = %d , %s ; thread_id = %ld",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    (long)syscall(SYS_gettid)
#else
                    0L
#endif
            );
    }
#elif defined(HAVE_WINDOWS_H)
    result = WaitForSingleObject((HANDLE)*mutex, 0);
    if (result == WAIT_OBJECT_0) {
        result = 0;
    } else if (result == WAIT_TIMEOUT) {
        result = EBUSY;
    } else {
        result = -1;
    }
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
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex operation failed! errno = %d , %s ; thread_id = %ld",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    (long)syscall(SYS_gettid)
#else
                    0L
#endif
            );
    }
#elif defined(HAVE_WINDOWS_H)
    result = ReleaseMutex((HANDLE)*mutex);
    if (result != TRUE) {
        int err = GetLastError();
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG,
                    "Mutex unlock operation failed! GetLastError() = %d", err);
        result = 1;
    } else {
        result = 0;
    }
#endif

    return result;
}

#ifdef HAVE_WINDOWS_H
/* http://www.cs.wustl.edu/~schmidt/win32-cv-1.html */
typedef struct ARSAL_WCond {
    int              waiters_count; /* number of waiting threads */
    CRITICAL_SECTION waiters_count_lock;

    HANDLE sema; /* Semaphore used to queue up threads waiting for the condition to become signaled. */
    HANDLE waiters_done; /* An auto-reset event used by the broadcast/signal thread to wait for all
                            the waiting thread(s) to wake up and be released from the semaphore. */

    size_t was_broadcast; /* Keeps track of whether we were broadcasting or signaling. This allows
                             us to optimize the code if we're just signaling. */
} ARSAL_WCond_t;
#endif

int ARSAL_Cond_Init(ARSAL_Cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    pthread_cond_t *pcond = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    *cond = (ARSAL_Cond_t)pcond;
    result = pthread_cond_init((pthread_cond_t *)*cond, NULL);
#elif defined(HAVE_WINDOWS_H)
    ARSAL_WCond_t *pcond = (ARSAL_WCond_t *)calloc(1, sizeof(ARSAL_WCond_t));
    InitializeCriticalSection(&pcond->waiters_count_lock);
    pcond->sema = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
    pcond->waiters_done = CreateEvent(NULL, FALSE, FALSE, NULL);
    *cond = (ARSAL_Cond_t)pcond;
#endif

    return result;
}

int ARSAL_Cond_Destroy(ARSAL_Cond_t *cond)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_cond_destroy((pthread_cond_t *)*cond);
    free(*cond);
#elif defined(HAVE_WINDOWS_H)
    ARSAL_WCond_t *pcond = (ARSAL_WCond_t *)*cond;
    CloseHandle(pcond->sema);
    CloseHandle(pcond->waiters_done);
    free(pcond);
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
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex/Cond operation failed! errno = %d , %s ; thread_id = %ld",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    (long)syscall(SYS_gettid)
#else
                    0L
#endif
            );
    }
#elif defined(HAVE_WINDOWS_H)
    result = ARSAL_Cond_Timedwait(cond, mutex, INFINITE);
#endif

    return result;
}

int ARSAL_Cond_Timedwait(ARSAL_Cond_t *cond, ARSAL_Mutex_t *mutex, int timeout)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    struct timespec ts;
    ARSAL_Time_GetLocalTime(&ts, NULL);
    ts.tv_nsec += MSEC_TO_NSEC(timeout % SEC_TO_MSEC(1));
    ts.tv_sec  += MSEC_TO_SEC(timeout);
    ts.tv_sec  += NSEC_TO_SEC(ts.tv_nsec);
    ts.tv_nsec %= SEC_TO_NSEC(1);

    result = pthread_cond_timedwait((pthread_cond_t *)*cond, (pthread_mutex_t *)*mutex, &ts);
    if ( (result != 0) && (result != ETIMEDOUT) )
    {
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex/Cond operation failed! errno = %d , %s ; thread_id = %ld",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    (long)syscall(SYS_gettid)
#else
                    0L
#endif
            );
    }
#elif defined(HAVE_WINDOWS_H)
    ARSAL_WCond_t *pcond = (ARSAL_WCond_t *)*cond;

    // Avoid race conditions.
    EnterCriticalSection(&pcond->waiters_count_lock);
    pcond->waiters_count++;
    LeaveCriticalSection(&pcond->waiters_count_lock);

    // This call atomically releases the mutex and waits on the
    // semaphore until <pthread_cond_signal> or <pthread_cond_broadcast>
    // are called by another thread.
    result = (int)SignalObjectAndWait((HANDLE)*mutex, pcond->sema, timeout, FALSE);

    // Reacquire lock to avoid race conditions.
    EnterCriticalSection(&pcond->waiters_count_lock);

    // We're no longer waiting...
    pcond->waiters_count--;

    // Check to see if we're the last waiter after <pthread_cond_broadcast>.
    int last_waiter = pcond->was_broadcast && pcond->waiters_count == 0;

    LeaveCriticalSection(&pcond->waiters_count_lock);

    // If we're the last waiter thread during this particular broadcast
    // then let all the other threads proceed.
    if (last_waiter && result != WAIT_TIMEOUT) {
        // This call atomically signals the <waiters_done_> event and waits until
        // it can acquire the <external_mutex>.  This is required to ensure fairness.
        SignalObjectAndWait(pcond->waiters_done, (HANDLE)*mutex, INFINITE, FALSE);
    } else {
        // Always regain the external mutex since that's the guarantee we
        // give to our callers.
        WaitForSingleObject((HANDLE)*mutex, INFINITE);
    }

    switch (result) {
    case WAIT_TIMEOUT:
        result = ETIMEDOUT;
        break;
    case WAIT_OBJECT_0:
        result = 0;
        break;
    default:
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG,
                    "Mutex/Cond operation failed! SignalObjectAndWait returned %d", result);
        result = -1;
        break;
    }
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
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex/Cond operation failed! errno = %d , %s ; thread_id = %ld",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    (long)syscall(SYS_gettid)
#else
                    0L
#endif
            );
    }
#elif defined(HAVE_WINDOWS_H)
    ARSAL_WCond_t *pcond = (ARSAL_WCond_t *)*cond;
    EnterCriticalSection(&pcond->waiters_count_lock);
    int have_waiters = pcond->waiters_count > 0;
    LeaveCriticalSection(&pcond->waiters_count_lock);

    if (have_waiters) {
        ReleaseSemaphore(pcond->sema, 1, NULL);
    }
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
        ARSAL_PRINT(ARSAL_PRINT_FATAL, ARSAL_MUTEX_TAG, "Mutex/Cond operation failed! errno = %d , %s ; thread_id = %ld",
                    result,
                    strerror(result),
#if HAVE_DECL_SYS_GETTID
                    (long)syscall(SYS_gettid)
#else
                    0L
#endif
            );
    }
#elif defined(HAVE_WINDOWS_H)
    result = -1;
    ARSAL_PRINT(ARSAL_PRINT_FATAL, "ARSALMutex", "Mutex/Cond operation failed! unimplemented on Win32");
#endif

    return result;
}
