#include <stdio.h>
#include <libARSAL/ARSAL_Mutex.h>
#include <libARSAL/ARSAL_Thread.h>
#include <libARSAL/ARSAL_Print.h>

ARSAL_Mutex_t mutex;
ARSAL_Cond_t cond;
int variable = 0;

void *routine(void *arg)
{
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, "test1", "Routine started\n");
    ARSAL_PRINT(ARSAL_PRINT_WARNING, "test1", "mutex lock\n");
    ARSAL_Mutex_Lock(&mutex);
    variable = 1;
    ARSAL_PRINT(ARSAL_PRINT_ERROR, "test1", "mutex signal\n");
    ARSAL_Cond_Signal(&cond);
    ARSAL_PRINT(ARSAL_PRINT_WARNING, "test1", "mutex unlock\n");
    ARSAL_Mutex_Unlock(&mutex);

    return NULL;
}

int main(int argc, char **argv)
{
    ARSAL_Thread_t thread;

    ARSAL_PRINT(ARSAL_PRINT_ERROR, "test1", "mutex init\n");
    ARSAL_Mutex_Init(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_WARNING, "test1", "condition init\n");
    ARSAL_Cond_Init(&cond);

    ARSAL_PRINT(ARSAL_PRINT_WARNING, "test1", "thread create\n");
    ARSAL_Thread_Create(&thread, routine, NULL);

    ARSAL_PRINT(ARSAL_PRINT_ERROR, "test1", "Variable : %d\n", variable);

    ARSAL_Thread_Join(thread, NULL);

    ARSAL_PRINT(ARSAL_PRINT_ERROR, "test1", "mutex lock\n");
    ARSAL_Mutex_Lock(&mutex);
    ARSAL_PRINT(ARSAL_PRINT_ERROR, "test1", "mutex wait\n");
    ARSAL_Cond_Timedwait(&cond, &mutex, 1000);
    //ARSAL_Cond_Wait(&cond, &mutex);
    ARSAL_PRINT(ARSAL_PRINT_ERROR, "test1", "mutex unlock\n");
    ARSAL_Mutex_Unlock(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, "test1", "Variable : %d\n", variable);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, "test1", "condition destroy\n");
    ARSAL_Cond_Destroy(&cond);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, "test1", "mutex destroy\n");
    ARSAL_Mutex_Destroy(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, "test1", "thread destroy\n");
    ARSAL_Thread_Destroy(&thread);
    return 0;
}
