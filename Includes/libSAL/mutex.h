/**
 * @file libSAL/mutex.h
 * @brief This file contains headers about mutex abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
*/
#ifndef _SAL_MUTEX_H_
#define _SAL_MUTEX_H_

typedef void* sal_mutex_t;
typedef void* sal_cond_t;

/**
 * @fn int sal_mutex_init(sal_mutex_t *mutex)
 * @brief Initializes a mutex.
 *
 * @param mutex The mutex to initialize
 * @retval On success, sal_mutex_init() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int sal_mutex_init(sal_mutex_t *mutex);

/**
 * @fn int sal_mutex_destroy(sal_mutex_t *mutex)
 * @brief Destroys a mutex
 *
 * @param mutex The mutex to destroy
 * @retval On success, sal_mutex_init() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int sal_mutex_destroy(sal_mutex_t *mutex);

/**
 * @fn int sal_mutex_lock(sal_mutex_t *mutex)
 * @brief Locks a mutex
 *
 * @param mutex The mutex to lock
 * @retval On success, sal_mutex_init() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int sal_mutex_lock(sal_mutex_t *mutex);

/**
 * @fn int sal_mutex_trylock(sal_mutex_t *mutex)
 * @brief Tries to lock a mutex
 *
 * @param mutex The mutex to lock
 * @retval On success, sal_mutex_init() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int sal_mutex_trylock(sal_mutex_t *mutex);

/**
 * @fn int sal_mutex_unlock(sal_mutex_t *mutex)
 * @brief Unlocks a mutex
 *
 * @param mutex The mutex to unlock
 * @retval On success, sal_mutex_init() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int sal_mutex_unlock(sal_mutex_t *mutex);

/**
 * @fn int sal_cond_init(sal_thread_cond_t *cond)
 * @brief Initializes a condition
 *
 * @param cond The condition to initialize
 * @retval On success, sal_cond_init() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int sal_cond_init(sal_cond_t *cond);

/**
 * @fn int sal_cond_destroy(sal_cond_t *cond)
 * @brief Destroy a condition
 *
 * @param cond The condition to destroy
 * @retval On success, sal_cond_destroy() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int sal_cond_destroy(sal_cond_t *cond);

/**
 * @fn int sal_cond_wait(sal_cond_t *cond, sal_mutex_t *mutex)
 * @brief Wait on a condition
 *
 * @param cond The condition to wait
 * @param mutex The mutex link to condition.
 * @retval On success, sal_cond_wait() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int sal_cond_wait(sal_cond_t *cond, sal_mutex_t *mutex);

/**
 * @fn int sal_cond_timedwait(sal_thread_cond_t *cond, sal_thread_mutex_t *mutex)
 * @brief Wait on a condition and returns error
 *
 * @param cond The condition to wait
 * @param mutex The mutex linked to condition.
 * @param timeout The time to wait before sal_cond_timedwait() returns ETIMEDOUT.
 * @retval On success, sal_cond_timedwait() returns 0. If the time is specified by timeout has passed, pthread_cond_timedwait() returns ETIMEDOUT. Otherwise, it returns an error number (See errno.h)
 */
int sal_cond_timedwait(sal_cond_t *cond, sal_mutex_t *mutex, int timeout);

/**
 * @fn int sal_cond_signal(sal_thread_cond_t *cond)
 * @brief Signal a condition
 *
 * @param cond The condition to signal
 * @retval On success, sal_cond_signal() returns 0. If the time is specified by timeout has passed, pthread_cond_timedwait() returns ETIMEDOUT. Otherwise, it returns an error number (See errno.h)
 */
int sal_cond_signal(sal_cond_t *cond);

/**
 * @fn int sal_cond_broadcast(sal_thread_cond_t *cond)
 * @brief Signal a condition
 *
 * @param cond The condition to broadcast
 * @retval On success, sal_cond_broadcast() returns 0. If the time is specified by timeout has passed, pthread_cond_timedwait() returns ETIMEDOUT. Otherwise, it returns an error number (See errno.h)
 */
int sal_cond_broadcast(sal_cond_t *cond);

#endif // _SAL_MUTEX_H_
