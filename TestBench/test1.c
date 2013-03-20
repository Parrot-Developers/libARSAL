#include <stdio.h>
#include <libARSAL/ARSAL_Mutex.h>
#include <libARSAL/ARSAL_Thread.h>
#include <libARSAL/ARSAL_Print.h>

#define __TAG__ "test1"

ARSAL_Mutex_t mutex;
ARSAL_Cond_t cond;
int variable = 0;

void *routine(void *arg)
{
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, __TAG__, "Routine started\n");
    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "mutex lock\n");
    ARSAL_Mutex_Lock(&mutex);
    variable = 1;
    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "mutex signal\n");
    ARSAL_Cond_Signal(&cond);
    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "mutex unlock\n");
    ARSAL_Mutex_Unlock(&mutex);

    return NULL;
}

int main(int argc, char **argv)
{
    ARSAL_Thread_t thread;

    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "mutex init\n");
    ARSAL_Mutex_Init(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "condition init\n");
    ARSAL_Cond_Init(&cond);

    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "thread create\n");
    ARSAL_Thread_Create(&thread, routine, NULL);

    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "Variable : %d\n", variable);

    ARSAL_Thread_Join(thread, NULL);

    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "mutex lock\n");
    ARSAL_Mutex_Lock(&mutex);
    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "mutex wait\n");
    ARSAL_Cond_Timedwait(&cond, &mutex, 1000);
    //ARSAL_Cond_Wait(&cond, &mutex);
    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "mutex unlock\n");
    ARSAL_Mutex_Unlock(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, __TAG__, "Variable : %d\n", variable);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, __TAG__, "condition destroy\n");
    ARSAL_Cond_Destroy(&cond);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, __TAG__, "mutex destroy\n");
    ARSAL_Mutex_Destroy(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, __TAG__, "thread destroy\n");
    ARSAL_Thread_Destroy(&thread);
    return 0;
}
