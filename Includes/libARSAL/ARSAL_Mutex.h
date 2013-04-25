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
 *  @brief ARSAL mutex type known.
**/
typedef enum
{
    ARSAL_MUTEX_TYPE_DEFAULT = 0, /**< Attempting to recursively lock a mutex of this type results in undefined behavior. Attempting to unlock a mutex of this type which was not locked by the calling thread results in undefined behavior. Attempting to unlock a mutex of this type which is not locked results in undefined behavior. An implementation may map this mutex to one of the other mutex types. */
    ARSAL_MUTEX_TYPE_NORMAL, /**< This type of mutex does not detect deadlock. A thread attempting to relock this mutex without first unlocking it shall deadlock. Attempting to unlock a mutex locked by a different thread results in undefined behavior. Attempting to unlock an unlocked mutex results in undefined behavior. */
    ARSAL_MUTEX_TYPE_ERRORCHECK, /**< This type of mutex provides error checking. A thread attempting to relock this mutex without first unlocking it shall return with an error. A thread attempting to unlock a mutex which another thread has locked shall return with an error. A thread attempting to unlock an unlocked mutex shall return with an error.*/
    ARSAL_MUTEX_TYPE_RECURSIVE, /**< A thread attempting to relock this mutex without first unlocking it shall succeed in locking the mutex. The relocking deadlock which can occur with mutexes of type PTHREAD_MUTEX_NORMAL cannot occur with this type of mutex. Multiple locks of this mutex shall require the same number of unlocks to release the mutex before another thread can acquire the mutex. A thread attempting to unlock a mutex which another thread has locked shall return with an error. A thread attempting to unlock an unlocked mutex shall return with an error. */
    ARSAL_MUTEX_MAX, /**< The maximum of enum, do not use !*/

} eARSAL_MUTEX_TYPE;

/**
 * @brief Initializes a mutex.
 *
 * @param mutex The mutex to initialize
 * @retval On success, ARSAL_Mutex_Init() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Mutex_Init(ARSAL_Mutex_t *mutex);

/**
 * @brief Initializes a mutex with type.
 *
 * @param mutex The mutex to initialize
 * @param type The mutex type
 * @retval On success, ARSAL_Mutex_InitWithType() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Mutex_InitWithType(ARSAL_Mutex_t *mutex, eARSAL_MUTEX_TYPE type);

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
