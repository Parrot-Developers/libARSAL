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
#include <stdio.h>
#include <libARSAL/ARSAL_Sem.h>
#include <libARSAL/ARSAL_Print.h>
#include <libARSAL/ARSAL_Thread.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/*
  TEST PATTERN :
  - WAIT TEST
  -- 2 post
  -- start thread
  -- 3 more post when sem_count reaches zero
  -> Thread should have the following counts :
  -- 3->0 during three first wait
  -- blocking for the two last, 0 after

  - TRYWAIT TEST
  -- 2 post
  -- start thread
  -> Thread should pass two time, return -1/EAGAIN the third

  - TIMEDWAIT (2 sec timeout) TEST
  -- 2 post
  -- start thread
  -- one post after 1 sec (should NOT timeout !)
  -- wait for timeout
  -> Thread should pass three time, return -1/ETIMEDOUT the fourth


*/

ARSAL_Sem_t errCountSem;

#define TEST_COUNT_VALUE(SEM,TEST)                                      \
    do                                                                  \
    {                                                                   \
        int __count;                                                    \
        if (0 == ARSAL_Sem_Getvalue(SEM, &__count))                     \
        {                                                               \
            if (TEST != __count)                                        \
            {                                                           \
                ARSAL_PRINT (ARSAL_PRINT_ERROR, "testSem", "BAD COUNT, got %d, expected %d\n", __count, TEST); \
                ARSAL_Sem_Post (&errCountSem);                          \
            }                                                           \
        }                                                               \
        else                                                            \
        {                                                               \
            ARSAL_PRINT (ARSAL_PRINT_ERROR, "testSem", "Unable to get sem value\n"); \
            ARSAL_Sem_Post (&errCountSem);                              \
        }                                                               \
    } while (0)

void *
waitTest (void *data)
{
    ARSAL_Sem_t *psem = (ARSAL_Sem_t *)data;

    TEST_COUNT_VALUE (psem, 3);
    ARSAL_Sem_Wait (psem);
    TEST_COUNT_VALUE (psem, 2);
    ARSAL_Sem_Wait (psem);
    TEST_COUNT_VALUE (psem, 1);
    ARSAL_Sem_Wait (psem);
    TEST_COUNT_VALUE (psem, 0);

    /* At this point, the thread will block until main thread post sem */
    /* We don't check count between the waits because we cant predict */
    /* the execution order */
    ARSAL_Sem_Wait (psem);
    ARSAL_Sem_Wait (psem);

    TEST_COUNT_VALUE (psem, 0);

    return NULL;
}

void *
tryWaitTest (void *data)
{
    ARSAL_Sem_t *psem = (ARSAL_Sem_t *)data;
    int waitCount = 0;
    int locerrno;
    int absDiff;
    while (0 == ARSAL_Sem_Trywait (psem))
    {
        waitCount++;
    }
    locerrno = errno;

    absDiff = abs (2 - waitCount);
    if (0 != absDiff)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testSem", "Bad wait count : got %d, expected %d\n", waitCount, 2);
    }
    while (0 != absDiff--)
    {
        ARSAL_Sem_Post (&errCountSem);
    }

    if (EAGAIN != locerrno)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testSem", "Trywait failed with error %d, expected a fail with error %d (EAGAIN)\n", locerrno, EAGAIN);
        ARSAL_Sem_Post (&errCountSem);
    }
    return NULL;
}

void *
timedWaitTest (void *data)
{
    ARSAL_Sem_t *psem = (ARSAL_Sem_t *)data;
    int waitCount = 0;
    int locerrno;
    int absDiff;
    const struct timespec tOut = {2, 0}; /* 2 sec, 0 nsec */
    while (0 == ARSAL_Sem_Timedwait (psem, &tOut))
    {
        waitCount++;
    }
    locerrno = errno;

    absDiff = abs (3 - waitCount);
    if (0 != absDiff)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testSem", "Bad wait count : got %d, expected %d\n", waitCount, 3);
    }
    while (0 != absDiff--)
    {
        ARSAL_Sem_Post (&errCountSem);
    }

    if (ETIMEDOUT != locerrno)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testSem", "Timedwait failed with error %d, expected a fail with error %d (ETIMEDOUT)\n", locerrno, ETIMEDOUT);
        ARSAL_Sem_Post (&errCountSem);
    }
    return NULL;
}

int
main (int argc, char *argv[])
{
    ARSAL_Sem_t testSem;
    int errCount = 0;
    int sCount = 0;
    ARSAL_Thread_t testThread;
    if (-1 == ARSAL_Sem_Init (&errCountSem, 0, 0))
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testSem", "Unable to initialize semaphore, aborting tests\n");
        return 1;
    }

    /* END OF INIT */

    /* WAIT TEST */
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testSem", "WAIT TEST ...\n");
    if (-1 == ARSAL_Sem_Init (&testSem, 0, 0))
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testSem", "Unable to initialize semaphore, aborting wait test\n");
        ARSAL_Sem_Post (&errCountSem);
    }
    else
    {
        /* Post 3 times */
        ARSAL_Sem_Post (&testSem);
        ARSAL_Sem_Post (&testSem);
        ARSAL_Sem_Post (&testSem);
        /* Create thread */
        ARSAL_Thread_Create (&testThread, waitTest, &testSem);
        /* Wait for sem value to reach zero */
        ARSAL_Sem_Getvalue (&testSem, &sCount);
        while (0 != sCount)
        {
            usleep (10000);
            ARSAL_Sem_Getvalue (&testSem, &sCount);
        }
        /* Do two more post */
        ARSAL_Sem_Post (&testSem);
        ARSAL_Sem_Post (&testSem);
        /* Join thread */
        ARSAL_Thread_Join (testThread, NULL);
        /* Destroy thread/sem */
        ARSAL_Thread_Destroy (&testThread);
        ARSAL_Sem_Destroy (&testSem);
    }
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testSem", "END OF TEST\n");

    /* TRYWAIT TEST */
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testSem", "TRYWAIT TEST ...\n");
    if (-1 == ARSAL_Sem_Init (&testSem, 0, 0))
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testSem", "Unable to initialize semaphore, aborting trywait test\n");
        ARSAL_Sem_Post (&errCountSem);
    }
    else
    {
        /* Post 2 times */
        ARSAL_Sem_Post (&testSem);
        ARSAL_Sem_Post (&testSem);
        /* Create thread */
        ARSAL_Thread_Create (&testThread, tryWaitTest, &testSem);
        /* Join thread */
        ARSAL_Thread_Join (testThread, NULL);
        /* Destroy thread/sem */
        ARSAL_Thread_Destroy (&testThread);
        ARSAL_Sem_Destroy (&testSem);
    }
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testSem", "END OF TEST\n");

    /* TIMEDWAIT TEST */
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testSem", "TIMEDWAIT TEST ...\n");
    if (-1 == ARSAL_Sem_Init (&testSem, 0, 0))
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testSem", "Unable to initialize semaphore, aborting timedwait test\n");
        ARSAL_Sem_Post (&errCountSem);
    }
    else
    {
        /* Post 2 times */
        ARSAL_Sem_Post (&testSem);
        ARSAL_Sem_Post (&testSem);
        /* Create thread */
        ARSAL_Thread_Create (&testThread, timedWaitTest, &testSem);
        /* Wait 1 sec */
        sleep (1);
        /* One more post */
        ARSAL_Sem_Post (&testSem);
        /* Join thread */
        ARSAL_Thread_Join (testThread, NULL);
        /* Destroy thread/sem */
        ARSAL_Thread_Destroy (&testThread);
        ARSAL_Sem_Destroy (&testSem);
    }
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testSem", "END OF TEST\n");

    /* SUMMARY PRINT */
    ARSAL_Sem_Getvalue (&errCountSem, &errCount);
    ARSAL_Sem_Destroy (&errCountSem);
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testSem", "\n\n\n");
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testSem", "<<< SUMMARY : >>>\n");
    if (0 == errCount)
    {
        ARSAL_PRINT (ARSAL_PRINT_WARNING, "testSem", "    NO ERROR\n");
    }
    else
    {
        char term = (errCount > 1) ? 'S' : ' ';
        ARSAL_PRINT (ARSAL_PRINT_WARNING, "testSem", "    %d ERROR%c\n", errCount, term);
    }

    return errCount;
}
