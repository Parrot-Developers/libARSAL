/**
 * @file libARSAL/ARSAL_Time.c
 * @brief This file contains sources about time abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
 */
#include <config.h>
#include <libARSAL/ARSAL_Time.h>

int ARSAL_Time_TimevalEquals (struct timeval *t1, struct timeval *t2)
{
    int result = 0;
    if ((t1 == NULL) ||
        (t2 == NULL))
    {
        return result;
    }
    /* No else --> Args check (return 0) */
    
    if ((t1->tv_sec == t2->tv_sec) &&
        (t1->tv_usec == t2->tv_usec))
    {
        result = 1;
    }
    /* Else the Timevals are not equal ; result = 0 */
    
    return result;
}

int32_t ARSAL_Time_ComputeMsTimeDiff (struct timeval *start, struct timeval *end)
{
    int32_t result = -1;
    struct timeval diff;
    if ((start == NULL) ||
        (end == NULL))
    {
        return result;
    }
    /* No else --> Args check (return -1) */

    diff.tv_sec = end->tv_sec - start->tv_sec;
    if (start->tv_usec > end->tv_usec)
    {
        diff.tv_sec--;
        diff.tv_usec = SEC_TO_USEC(1) - (start->tv_usec - end->tv_usec);
    }
    else
    {
        diff.tv_usec = end->tv_usec - start->tv_usec;
    }

    result = SEC_TO_MSEC(diff.tv_sec) + USEC_TO_MSEC(diff.tv_usec);

    return result;
}
