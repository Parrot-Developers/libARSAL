#include <stdio.h>
#include <libSAL/mutex.h>

int main(int argc, char **argv)
{
	int variable = 0;
	sal_mutex_t mutex;
	sal_cond_t cond;

	printf("mutex init\n");
	sal_mutex_init(&mutex);

	printf("condition init\n");
	sal_cond_init(&cond);

	printf("mutex lock\n");
	sal_mutex_lock(&mutex);
	sal_cond_timedwait(&cond, &mutex, 1000);
	//sal_cond_wait(&cond, &mutex);
	variable = 1;
	printf("mutex unlock\n");
	sal_mutex_unlock(&mutex);

	printf("mutex trylock\n");
	sal_mutex_trylock(&mutex);
	variable = 1;
	printf("mutex unlock\n");
	sal_mutex_unlock(&mutex);

	printf("condition destroy\n");
	sal_cond_destroy(&cond);

	printf("mutex destroy\n");
	sal_mutex_destroy(&mutex);

	return 0;
}

