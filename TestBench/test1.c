#include <stdio.h>
#include <libSAL/mutex.h>
#include <libSAL/thread.h>
#include <libSAL/print.h>

sal_mutex_t mutex;
sal_cond_t cond;
int variable = 0;

void *routine(void *arg)
{
	int i = 0;
	SAL_PRINT(PRINT_DEBUG, "Routine started\n");
	SAL_PRINT(PRINT_WARNING, "mutex lock\n");
	sal_mutex_lock(&mutex);
	variable = 1;
	SAL_PRINT(PRINT_ERROR, "mutex signal\n");
	sal_cond_signal(&cond);
	SAL_PRINT(PRINT_WARNING, "mutex unlock\n");
	sal_mutex_unlock(&mutex);

	return NULL;
}

int main(int argc, char **argv)
{
	sal_thread_t thread;

	SAL_PRINT(PRINT_ERROR, "mutex init\n");
	sal_mutex_init(&mutex);

	SAL_PRINT(PRINT_WARNING, "condition init\n");
	sal_cond_init(&cond);

	SAL_PRINT(PRINT_WARNING, "thread create\n");
	sal_thread_create(&thread, routine, NULL);

	SAL_PRINT(PRINT_ERROR, "Variable : %d\n", variable);

	sal_thread_join(thread, NULL);

	SAL_PRINT(PRINT_ERROR, "mutex lock\n");
	sal_mutex_lock(&mutex);
	SAL_PRINT(PRINT_ERROR, "mutex wait\n");
	sal_cond_timedwait(&cond, &mutex, 1000);
	//sal_cond_wait(&cond, &mutex);
	SAL_PRINT(PRINT_ERROR, "mutex unlock\n");
	sal_mutex_unlock(&mutex);

	SAL_PRINT(PRINT_DEBUG, "Variable : %d\n", variable);

	SAL_PRINT(PRINT_DEBUG, "condition destroy\n");
	sal_cond_destroy(&cond);

	SAL_PRINT(PRINT_DEBUG, "mutex destroy\n");
	sal_mutex_destroy(&mutex);

	SAL_PRINT(PRINT_DEBUG, "thread destroy\n");
	sal_thread_destroy(&thread);
	return 0;
}
