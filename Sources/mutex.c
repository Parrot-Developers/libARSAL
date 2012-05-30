#include <config.h>
#include <libSAL/mutex.h>

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#else
#error The pthread library is required in order to build the library.
#endif

int sal_mutex_init(sal_mutex_t *mutex)
{
	int result = 0;

#if defined(HAVE_PTHREAD_H)
	result = pthread_mutex_init((pthread_mutex_t *)mutex, NULL);
#endif

	return result;
}

int sal_mutex_destroy(sal_mutex_t *mutex)
{
	int result = 0;

#if defined(HAVE_PTHREAD_H)
	result = pthread_mutex_destroy((pthread_mutex_t *)mutex);
#endif

	return result;
}

int sal_mutex_lock(sal_mutex_t *mutex)
{
	int result = 0;

#if defined(HAVE_PTHREAD_H)
	result = pthread_mutex_lock((pthread_mutex_t *)mutex);
#endif

	return result;
}

int sal_mutex_trylock(sal_mutex_t *mutex)
{
	int result = 0;

#if defined(HAVE_PTHREAD_H)
	result = pthread_mutex_trylock((pthread_mutex_t *)mutex);
#endif

	return result;
}

int sal_mutex_unlock(sal_mutex_t *mutex)
{
	int result = 0;

#if defined(HAVE_PTHREAD_H)
	result = pthread_mutex_unlock((pthread_mutex_t *)mutex);
#endif

	return result;
}
