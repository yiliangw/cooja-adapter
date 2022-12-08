#include "contiki-conf.h"
#include "event2_/event.h"
#include "util-internal.h"

#ifdef PLATFORM_NATIVE
#include <sys/time.h>
int evutil_gettimeofday(struct timeval *tv, struct timezone *tz)
{
    return gettimeofday(tv, tz);
}

#elif defined(PLATFORM_COOJA)

#include <bits/types/struct_timeval.h>
#include "sys/rtimer.h"
struct timezone
  {
    int tz_minuteswest;
    int tz_dsttime;
  };

int evutil_gettimeofday(struct timeval *tv, struct timezone *tz)
{
	(void) tz;
    rtimer_clock_t ct = rtimer_arch_now();
	clocktime_to_timeval(ct, tv);
	return 0;
}
#endif

#define RTIMER_USECOND ((double)RTIMER_SECOND / 1000000) 
int clocktime_to_timeval(rtimer_clock_t clocktime, struct timeval *tv)
{
    tv->tv_sec = clocktime / RTIMER_SECOND;
    tv->tv_usec = (unsigned) (clocktime % RTIMER_SECOND / RTIMER_USECOND);
    return 0;
}

rtimer_clock_t timeval_to_clocktime(struct timeval *tv)
{
    return (rtimer_clock_t) (tv->tv_sec * RTIMER_SECOND + tv->tv_usec * RTIMER_USECOND);
}
