/**
 * @file libARSAL/ARSAL_Mutex.h
 * @brief This file contains headers about mutex abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
 */
#ifndef _ARSAL_MUTEX_H_
#define _ARSAL_MUTEX_H_

/**
 * @brief Define a mutex type.
 */
typedef void* ARSAL_Mutex_t;

/**
 * @brief Define a condition type.
 */
typedef void* ARSAL_Cond_t;

/**
 * @brief Initializes a mutex.
 *
 * @param mutex The mutex to initialize
 * @retval On success, ARSAL_Mutex_Init() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Mutex_Init(ARSAL_Mutex_t *mutex);

/**
 * @brief Destroys a mutex
 *
 * @param mutex The mutex to destroy
 * @retval On success, ARSAL_Mutex_Destroy() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Mutex_Destroy(ARSAL_Mutex_t *mutex);

/**
 * @brief Locks a mutex
 *
 * @param mutex The mutex to lock
 * @retval On success, ARSAL_Mutex_Lock() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Mutex_Lock(ARSAL_Mutex_t *mutex);

/**
 * @brief Tries to lock a mutex
 *
 * @param mutex The mutex to lock
 * @retval On success, ARSAL_Mutex_Trylock() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Mutex_Trylock(ARSAL_Mutex_t *mutex);

/**
 * @brief Unlocks a mutex
 *
 * @param mutex The mutex to unlock
 * @retval On success, ARSAL_Mutex_Unlock() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Mutex_Unlock(ARSAL_Mutex_t *mutex);

/**
 * @brief Initializes a condition
 *
 * @param cond The condition to initialize
 * @retval On success, ARSAL_Cond_Init() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Cond_Init(ARSAL_Cond_t *cond);

/**
 * @brief Destroy a condition
 *
 * @param cond The condition to destroy
 * @retval On success, ARSAL_Cond_Destroy() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Cond_Destroy(ARSAL_Cond_t *cond);

/**
 * @brief Wait on a condition
 *
 * @param cond The condition to wait
 * @param mutex The mutex link to condition.
 * @retval On success, ARSAL_Cond_Wait() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Cond_Wait(ARSAL_Cond_t *cond, ARSAL_Mutex_t *mutex);

/**
 * @brief Wait on a condition and returns error
 *
 * @param cond The condition to wait
 * @param mutex The mutex linked to condition.
 * @param timeout The time (ms) to wait before ARSAL_Cond_Timedwait() returns ETIMEDOUT.
 * @retval On success, ARSAL_Cond_Timedwait() returns 0. If the time is specified by timeout has passed, ARSAL_Cond_Timedwait() returns ETIMEDOUT. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Cond_Timedwait(ARSAL_Cond_t *cond, ARSAL_Mutex_t *mutex, int timeout);

/**
 * @brief Signal a condition
 *
 * @param cond The condition to signal
 * @retval On success, ARSAL_Cond_Signal() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Cond_Signal(ARSAL_Cond_t *cond);

/**
 * @brief Broadcast a condition
 *
 * @param cond The condition to broadcast
 * @retval On success, ARSAL_Cond_Broadcast() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Cond_Broadcast(ARSAL_Cond_t *cond);

#endif // _ARSAL_MUTEX_H_
