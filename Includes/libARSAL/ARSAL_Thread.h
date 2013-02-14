/**
 * @file libARSAL/ARSAL_Thread.h
 * @brief This file contains headers about thread abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
 */
#ifndef _ARSAL_THREAD_H_
#define _ARSAL_THREAD_H_

/**
 * @brief Define a thread type.
 */
typedef void* ARSAL_Thread_t;

/**
 * @brief Define a callback function type.
 */
typedef void* (*ARSAL_Thread_Routine_t) (void *);

/**
 * @brief Create a new thread
 *
 * @param thread The thread to create
 * @param routine The routine to invoke by thread
 * @param arg The argument passed to routine()
 * @retval On success, ARSAL_Thread_create() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Thread_Create(ARSAL_Thread_t *thread, ARSAL_Thread_Routine_t routine, void *arg);

/**
 * @brief Join a thread
 *
 * @param thread The thread to join
 * @param retval If this pointer is not NULL, the exit status is placed into *retval
 * @retval On success, ARSAL_Thread_join() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Thread_Join(ARSAL_Thread_t thread, void **retval);

/**
 * @brief Destroy a thread
 *
 * @param thread The thread to destroy
 * @retval On success, ARSAL_Thread_create() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int ARSAL_Thread_Destroy(ARSAL_Thread_t *thread);

#endif // _ARSAL_THREAD_H_
