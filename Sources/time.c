/**
 * @file libSAL/time.c
 * @brief This file contains sources about time abstraction layer
 * @date 05/18/2012
 * @author frederic.dhaeyer@parrot.com
 */
#include <config.h>

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
