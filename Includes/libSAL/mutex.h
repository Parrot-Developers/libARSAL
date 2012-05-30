/**
 * @file libSAL/mutex.h
 * @brief This file contains headers about mutex abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
*/
//------------------------------
#ifndef _MUTEX_H_
#define _MUTEX_H_

typedef void* sal_mutex_t;

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

#endif // _MUTEX_H_
