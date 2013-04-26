/**
 * @file libARSAL/ARSAL_Time.c
 * @brief This file contains sources about time abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
 */
#include <config.h>
#include <libARSAL/ARSAL_Time.h>

// TO CHECK
/*#if defined(HAVE_MACH_MACH_TIME_H)
  #include <mach/mach_time.h>
  int gettimeofday (struct timeval *tp, void *tz)
  {
  mach_timebase_info_data_t sTimebaseInfo;
  uint64_t mtime = mach_absolute_time();
  uint64_t useconds;

  mach_timebase_info(&sTimebaseInfo);
  useconds = (mtime * (sTimebaseInfo.numer / sTimebaseInfo.denom) / 1000);

  tp->tv_sec = (long)(useconds / 1000000);
  tp->tv_usec = (long)(useconds % 1000000);

  return 0;
  }
  #endif
*/

int32_t ARSAL_Time_ComputeMsTimeDiff (struct timeval *start, struct timeval *end)
{
    int32_t result = -1;
    struct timeval diff;
    if (start == NULL || end == NULL)
    {
        return result;
    }

    diff.tv_sec = end->tv_sec - start->tv_sec;
    if (start->tv_usec > end->tv_usec)
    {
        diff.tv_sec--;
        diff.tv_usec = 1000000 - (start->tv_usec - end->tv_usec);
    }
    else
    {
        diff.tv_usec = end->tv_usec - start->tv_usec;
    }

    result = (diff.tv_sec * 1000) + (diff.tv_usec / 1000);

    return result;
}
