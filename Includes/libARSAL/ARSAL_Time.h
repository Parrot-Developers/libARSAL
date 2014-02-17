/**
 * @file libARSAL/ARSAL_Time.h
 * @brief This file contains headers about time abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
 */
#ifndef _ARSAL_TIME_H_
#define _ARSAL_TIME_H_
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

/**
 * @brief Convert second to millisecond
 */
#define SEC_TO_MSEC(sec)    (sec * 1000)

/**
 * @brief Convert second to microsecond
 */
#define SEC_TO_USEC(sec)    (sec * 1000000)

/**
 * @brief Convert second to nanosecond
 */
#define SEC_TO_NSEC(sec)    (sec * 1000000000)

/**
 * @brief Convert millisecond to second
 */
#define MSEC_TO_SEC(msec)    (msec / 1000)

/**
 * @brief Convert millisecond to microsecond
 */
#define MSEC_TO_USEC(msec)    (msec * 1000)

/**
 * @brief Convert millisecond to nanosecond
 */
#define MSEC_TO_NSEC(msec)    (msec * 1000000)

/**
 * @brief Convert microsecond to second
 */
#define USEC_TO_SEC(usec)    (usec / 1000000)

/**
 * @brief Convert microsecond to millisecond
 */
#define USEC_TO_MSEC(usec)    (usec / 1000)

/**
 * @brief Convert microsecond to nanosecond
 */
#define USEC_TO_NSEC(usec)    (usec * 1000)

/**
 * @brief Convert nanosecond to second
 */
#define NSEC_TO_SEC(nsec)    (nsec / 1000000000)

/**
 * @brief Convert nanosecond to millisecond
 */
#define NSEC_TO_MSEC(nsec)    (nsec / 1000000)

/**
 * @brief Convert nanosecond to microsecond
 */
#define NSEC_TO_USEC(nsec)    (nsec / 1000)


#ifndef TIMEVAL_TO_TIMESPEC
/**
 * @brief Convert timeval structure to timespec structure
 */
#define TIMEVAL_TO_TIMESPEC(tv, ts)                 \
    do                                              \
    {                                               \
        (ts)->tv_sec = (tv)->tv_sec;                \
        (ts)->tv_nsec = USEC_TO_NSEC((tv)->tv_usec);\
    } while(0)
#endif

#ifndef TIMESPEC_TO_TIMEVAL
/**
 * @brief Convert timespec structure to timeval structure
 */
#define TIMESPEC_TO_TIMEVAL(ts, tv)                 \
    do                                              \
    {                                               \
        (tv)->tv_sec = (ts)->tv_sec;                \
        (tv)->tv_usec = NSEC_TO_USEC((ts)->tv_nsec);\
    } while(0)
#endif


/**
 * @brief Checks the equality of two timeval
 *
 * @param t1 First time to compare
 * @param t2 Second time to compare
 *
 * @return 1 if t1 and t2 represent the same value
 * @return 0 in all other cases
 */
int ARSAL_Time_TimevalEquals (struct timeval *t1, struct timeval *t2);

/**
 * @brief Computes the difference between two timeval
 *
 * This function returns the time, in miliseconds, between the two
 * given timevals.
 *
 * @param start Start of the time interval to compute
 * @param end End of the time interval to compute
 * @return The number of ms between the two timeval. A negative number is an error
 * @warning Make sure that "end" is after "start"
 */
int32_t ARSAL_Time_ComputeMsTimeDiff (struct timeval *start, struct timeval *end);

#endif // _ARSAL_TIME_H_
