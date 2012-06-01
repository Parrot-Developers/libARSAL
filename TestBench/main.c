#include <stdio.h>
#include <libSAL/mutex.h>
#include <libSAL/thread.h>

sal_mutex_t mutex;
sal_cond_t cond;
int variable = 0;

void *routine(void *arg)
{
	int i = 0;
	printf("Routine started\n");
	printf("mutex lock\n");
	sal_mutex_lock(&mutex);
	variable = 1;
	printf("mutex signal\n");
	sal_cond_signal(&cond);
	printf("mutex unlock\n");
	sal_mutex_unlock(&mutex);

	while(i++ < 3)
		sleep(1);

	return NULL;
}

int main(int argc, char **argv)
{
	sal_thread_t thread;

	printf("mutex init\n");
	sal_mutex_init(&mutex);

	printf("condition init\n");
	sal_cond_init(&cond);

	printf("thread create\n");
	sal_thread_create(&thread, routine, NULL);

	printf("Variable : %d\n", variable);

	sal_thread_join(thread, NULL);

	printf("mutex lock\n");
	sal_mutex_lock(&mutex);
	printf("mutex wait\n");
	sal_cond_timedwait(&cond, &mutex, 1000);
	//sal_cond_wait(&cond, &mutex);
	printf("mutex unlock\n");
	sal_mutex_unlock(&mutex);

	printf("Variable : %d\n", variable);

	printf("condition destroy\n");
	sal_cond_destroy(&cond);

	printf("mutex destroy\n");
	sal_mutex_destroy(&mutex);

	printf("thread destroy\n");
	sal_thread_destroy(&thread);
	return 0;
}

