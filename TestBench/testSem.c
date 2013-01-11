#include <stdio.h>
#include <libSAL/sem.h>
#include <libSAL/print.h>
#include <libSAL/thread.h>
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

sal_sem_t errCountSem;

#define TEST_COUNT_VALUE(SEM,TEST)                                      \
    do                                                                  \
    {                                                                   \
        int __count;                                                    \
        if (0 == sal_sem_getvalue (SEM, &__count))                      \
        {                                                               \
            if (TEST != __count)                                        \
            {                                                           \
                SAL_PRINT (PRINT_ERROR, "testSem", "BAD COUNT, got %d, expected %d\n", __count, TEST); \
                sal_sem_post (&errCountSem);                            \
            }                                                           \
        }                                                               \
        else                                                            \
        {                                                               \
            SAL_PRINT (PRINT_ERROR, "testSem", "Unable to get sem value\n");       \
            sal_sem_post (&errCountSem);                                \
        }                                                               \
    } while (0)

void *
waitTest (void *data)
{
    sal_sem_t *psem = (sal_sem_t *)data;

    TEST_COUNT_VALUE (psem, 3);
    sal_sem_wait (psem);
    TEST_COUNT_VALUE (psem, 2);
    sal_sem_wait (psem);
    TEST_COUNT_VALUE (psem, 1);
    sal_sem_wait (psem);
    TEST_COUNT_VALUE (psem, 0);

    /* At this point, the thread will block until main thread post sem */
    /* We don't check count between the waits because we cant predict */
    /* the execution order */
    sal_sem_wait (psem);
    sal_sem_wait (psem);

    TEST_COUNT_VALUE (psem, 0);

    return NULL;
}

void *
tryWaitTest (void *data)
{
    sal_sem_t *psem = (sal_sem_t *)data;
    int waitCount = 0;
    int locerrno;
    int absDiff;
    while (0 == sal_sem_trywait (psem))
    {
        waitCount++;
    }
    locerrno = errno;

    absDiff = abs (2 - waitCount);
    if (0 != absDiff)
    {
        SAL_PRINT (PRINT_ERROR, "testSem", "Bad wait count : got %d, expected %d\n", waitCount, 2);
    }
    while (0 != absDiff--)
    {
        sal_sem_post (&errCountSem);
    }

    if (EAGAIN != locerrno)
    {
        SAL_PRINT (PRINT_ERROR, "testSem", "Trywait failed with error %d, expected a fail with error %d (EAGAIN)\n", locerrno, EAGAIN);
        sal_sem_post (&errCountSem);
    }
    return NULL;
}

void *
timedWaitTest (void *data)
{
    sal_sem_t *psem = (sal_sem_t *)data;
    int waitCount = 0;
    int locerrno;
    int absDiff;
    const struct timespec tOut = {2, 0}; /* 2 sec, 0 nsec */
    while (0 == sal_sem_timedwait (psem, &tOut))
    {
        waitCount++;
    }
    locerrno = errno;

    absDiff = abs (3 - waitCount);
    if (0 != absDiff)
    {
        SAL_PRINT (PRINT_ERROR, "testSem", "Bad wait count : got %d, expected %d\n", waitCount, 3);
    }
    while (0 != absDiff--)
    {
        sal_sem_post (&errCountSem);
    }

    if (ETIMEDOUT != locerrno)
    {
        SAL_PRINT (PRINT_ERROR, "testSem", "Timedwait failed with error %d, expected a fail with error %d (ETIMEDOUT)\n", locerrno, ETIMEDOUT);
        sal_sem_post (&errCountSem);
    }
    return NULL;
}

int
main (int argc, char *argv[])
{
    sal_sem_t testSem;
    int errCount = 0;
    int sCount = 0;
    sal_thread_t testThread;
    if (-1 == sal_sem_init (&errCountSem, 0, 0))
    {
        SAL_PRINT (PRINT_ERROR, "testSem", "Unable to initialize semaphore, aborting tests\n");
        return 1;
    }

    /* END OF INIT */

    /* WAIT TEST */
    SAL_PRINT (PRINT_WARNING, "testSem", "WAIT TEST ...\n");
    if (-1 == sal_sem_init (&testSem, 0, 0))
    {
        SAL_PRINT (PRINT_ERROR, "testSem", "Unable to initialize semaphore, aborting wait test\n");
        sal_sem_post (&errCountSem);
    }
    else
    {
        /* Post 3 times */
        sal_sem_post (&testSem);
        sal_sem_post (&testSem);
        sal_sem_post (&testSem);
        /* Create thread */
        sal_thread_create (&testThread, waitTest, &testSem);
        /* Wait for sem value to reach zero */
        sal_sem_getvalue (&testSem, &sCount);
        while (0 != sCount)
        {
            usleep (10000);
            sal_sem_getvalue (&testSem, &sCount);
        }
        /* Do two more post */
        sal_sem_post (&testSem);
        sal_sem_post (&testSem);
        /* Join thread */
        sal_thread_join (testThread, NULL);
        /* Destroy thread/sem */
        sal_thread_destroy (&testThread);
        sal_sem_destroy (&testSem);
    }
    SAL_PRINT (PRINT_WARNING, "testSem", "END OF TEST\n");

    /* TRYWAIT TEST */
    SAL_PRINT (PRINT_WARNING, "testSem", "TRYWAIT TEST ...\n");
    if (-1 == sal_sem_init (&testSem, 0, 0))
    {
        SAL_PRINT (PRINT_ERROR, "testSem", "Unable to initialize semaphore, aborting trywait test\n");
        sal_sem_post (&errCountSem);
    }
    else
    {
        /* Post 2 times */
        sal_sem_post (&testSem);
        sal_sem_post (&testSem);
        /* Create thread */
        sal_thread_create (&testThread, tryWaitTest, &testSem);
        /* Join thread */
        sal_thread_join (testThread, NULL);
        /* Destroy thread/sem */
        sal_thread_destroy (&testThread);
        sal_sem_destroy (&testSem);
    }
    SAL_PRINT (PRINT_WARNING, "testSem", "END OF TEST\n");

    /* TIMEDWAIT TEST */
    SAL_PRINT (PRINT_WARNING, "testSem", "TIMEDWAIT TEST ...\n");
    if (-1 == sal_sem_init (&testSem, 0, 0))
    {
        SAL_PRINT (PRINT_ERROR, "testSem", "Unable to initialize semaphore, aborting timedwait test\n");
        sal_sem_post (&errCountSem);
    }
    else
    {
        /* Post 2 times */
        sal_sem_post (&testSem);
        sal_sem_post (&testSem);
        /* Create thread */
        sal_thread_create (&testThread, timedWaitTest, &testSem);
        /* Wait 1 sec */
        sleep (1);
        /* One more post */
        sal_sem_post (&testSem);
        /* Join thread */
        sal_thread_join (testThread, NULL);
        /* Destroy thread/sem */
        sal_thread_destroy (&testThread);
        sal_sem_destroy (&testSem);
    }
    SAL_PRINT (PRINT_WARNING, "testSem", "END OF TEST\n");

    /* SUMMARY PRINT */
    sal_sem_getvalue (&errCountSem, &errCount);
    sal_sem_destroy (&errCountSem);
    SAL_PRINT (PRINT_WARNING, "testSem", "\n\n\n");
    SAL_PRINT (PRINT_WARNING, "testSem", "<<< SUMMARY : >>>\n");
    if (0 == errCount)
    {
        SAL_PRINT (PRINT_WARNING, "testSem", "    NO ERROR\n");
    }
    else
    {
        char term = (errCount > 1) ? 'S' : ' ';
        SAL_PRINT (PRINT_WARNING, "testSem", "    %d ERROR%c\n", errCount, term);
    }

    return errCount;
}
