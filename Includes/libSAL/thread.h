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
typedef void* sal_thread_param_t;
typedef void* (*sal_thread_routine) (sal_thread_param_t);

#endif // _THREAD_H_

