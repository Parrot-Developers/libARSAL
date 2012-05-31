/**
 * \file libSAL/thread.h
 * \brief This file contains headers about thread abstraction layer 
 * \date 05/18/2012 
 * \author frederic.dhaeyer@parrot.com 
*/
//------------------------------
#ifndef _THREAD_H_
#define _THREAD_H_

typedef void* sal_thread_t;
typedef void* (*sal_thread_routine) (void *);

/**
 * @fn int sal_thread_create(sal_thread_t *thread, sal_thread_routine *routine, void *arg)
 * @brief Create a new thread
 *
 * @param thread The thread to create
 * @param routine The routine to invoke by thread
 * @param arg The argument passed to routine()
 * @retval On success, sal_thread_create() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int sal_thread_create(sal_thread_t *thread, sal_thread_routine routine, void *arg);

/**
 * @fn int sal_thread_join(sal_thread_t thread, void **retval)
 * @brief Join a thread
 *
 * @param thread The thread to join
 * @param retval If this pointer is not NULL, the exit status is placed into *retval
 * @retval On success, sal_thread_join() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int sal_thread_join(sal_thread_t thread, void **retval);

/**
 * @fn int sal_thread_destroy(sal_thread_t *thread)
 * @brief Destroy a thread
 *
 * @param thread The thread to destroy
 * @retval On success, sal_thread_create() returns 0. Otherwise, it returns an error number (See errno.h)
 */
int sal_thread_destroy(sal_thread_t *thread);

#endif // _THREAD_H_
