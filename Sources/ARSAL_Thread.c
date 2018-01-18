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
 * @file libARSAL/ARSAL_Thread.c
 * @brief This file contains sources about thread abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
 */
#include <config.h>
#include <libARSAL/ARSAL_Print.h>
#include <libARSAL/ARSAL_Thread.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#elif defined(HAVE_WINDOWS_H)
#include <windows.h>
#include <process.h>
#else
#error No compatible threading library found!
#endif

#ifdef HAVE_WINDOWS_H
typedef struct ARSAL_WThread {
    HANDLE thread_handle;
    ARSAL_Thread_Routine_t routine;
    void *arg;
    void *ret;
    int detached;
} ARSAL_WThread_t;

static uint32_t ARSAL_Thread_W32_Entry(void *arg)
{
    ARSAL_WThread_t *thread = (ARSAL_WThread_t *)arg;
    thread->ret = thread->routine(thread->arg);

    if (thread->detached) {
        free(thread);
    }

    return 0;
}
#endif

int ARSAL_Thread_Create(ARSAL_Thread_t *thread, ARSAL_Thread_Routine_t routine, void *arg)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    pthread_t *pthread = (pthread_t *)calloc(1, sizeof(pthread_t));
    if (!pthread) {
        result = -1;
    } else {
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        if (!thread) {
            result = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        }

        if (result == 0) {
            result = pthread_create(pthread, &attr, routine, arg);
            if (thread && result == 0) {
                *thread = (ARSAL_Thread_t)pthread;
            } else {
                free(pthread);
            }
        }
    }
#elif defined(HAVE_WINDOWS_H)
    ARSAL_WThread_t *wthread = (ARSAL_WThread_t *)calloc(1, sizeof(ARSAL_WThread_t));
    if (wthread) {
        wthread->arg = arg;
        wthread->routine = routine;
        wthread->detached = thread == NULL ? 1 : 0;
        wthread->thread_handle =
            (HANDLE)_beginthreadex(NULL, 0, ARSAL_Thread_W32_Entry, wthread, 0, NULL);

        if (wthread->thread_handle == 0) {
            free(wthread);
            result = -1;

            if (thread) {
                *thread = NULL;
            }
        } else if (thread) {
            *thread = wthread;
        }
    } else {
        result = -1;
    }
#endif

    return result;
}

int ARSAL_Thread_Join(ARSAL_Thread_t thread, void **retval)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    result = pthread_join((*(pthread_t *)thread), retval);
#elif defined (HAVE_WINDOWS_H)
    ARSAL_WThread_t *wthread = thread;
    result = WaitForSingleObject(wthread->thread_handle, INFINITE);
#endif

    return result;
}

int ARSAL_Thread_Destroy(ARSAL_Thread_t *thread)
{
    int result = 0;

#if defined(HAVE_PTHREAD_H)
    free(*thread);
#elif defined(HAVE_WINDOWS_H)
    ARSAL_WThread_t *wthread = *thread;
    CloseHandle(wthread->thread_handle);
    free(wthread);
#endif

    *thread = NULL;
    return result;
}
