/**
 * @file libSAL/sem.h
 * @brief This file contains headers about semaphore abstraction layer
 * @date 12/27/2012
 * @author nicolas.brulez@parrot.com
 */
#ifndef _SAL_SEM_H_
#define _SAL_SEM_H_

/**
 * @brief Define a semaphore type.
 */
typedef void* sal_sem_t;

/**
 * @brief Initializes a semaphore.
 *
 * @param sem The semaphore to initialize
 * @param shared Flag asking for a multi-process shared semaphore
 * @param value Initial value of the semaphore
 * @retval On success, sal_sem_init() returns 0. Otherwise, it returns -1 and set errno (See errno.h)
 */
int sal_sem_init(sal_sem_t *sem, int shared, int value);

/**
 * @brief Destroys a semaphore
 *
 * @param sem The sem to destroy
 * @retval On success, sal_sem_destroy() returns 0. Otherwise, it returns -1 and set errno (See errno.h)
 */
int sal_sem_destroy(sal_sem_t *sem);

/**
 * @brief Wait for a semaphore
 *
 * @param sem The sem to wait for
 * @retval On success, sal_sem_wait() returns 0. Otherwise, it returns -1 and set errno (See errno.h)
 */
int sal_sem_wait(sal_sem_t *sem);

/**
 * @brief Non blocking wait for a semaphore
 *
 * @param sem The sem to wait for
 * @retval If the semaphore was successfully decremented, sal_sem_trywait() returns 0. If the call would have blocked, it returns -1 and sets errno to "EAGAIN". On any other error, return -1 and set errno (See errno.h)
 */
int sal_sem_trywait(sal_sem_t *sem);

/**
 * @brief Wait for a semaphore with a timeout
 *
 * @param sem The sem to wait for
 * @param timeout Maximum time to wait
 * @warning POSIX.1-2001 semaphore use an absolute time as timeout. Instead, libSAL use a relative time !
 * @retval If the semaphore was sucessfully decremented, sal_sem_timedwait() returns 0. If the call has timed-out, it returns -1 and sets errno to "ETIMEDOUT". On any other error, return -1 and set errno (See errno.h)
 */
int sal_sem_timedwait(sal_sem_t *sem, const struct timespec *timeout);

/**
 * @brief Increment a semaphore
 *
 * @param sem The semaphore to increment
 * @retval On success, sal_sem_post() returns 0. Otherwise, it returns -1 and set errno (See errno.h)
 */
int sal_sem_post(sal_sem_t *sem);

/**
 * @brief Get the current value of a semaphore
 *
 * @param sem The semaphore to get value from
 * @param value Pointer which will hold the current value of the semaphore
 * @retval On success, sal_sem_getvalue() returns 0. Otherwise, it returns -1 and set errno (See errno.h)
 */
int sal_sem_getvalue(sal_sem_t *sem, int *value);

#endif // _SAL_SEM_H_
