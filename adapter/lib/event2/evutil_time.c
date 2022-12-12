#include "coojaa/event2/event.h"
#include "platform-conf.h"
#include "util-internal.h"

#include "internal/clock.h"

struct timezone
{
  int tz_minuteswest;
  int tz_dsttime;
};

int evutil_gettimeofday(struct timeval *tv, struct timezone *tz)
{
	(void) tz;
  clock_time_t ct = clock_time();
	clocktime_to_timeval(ct, tv);
	return 0;
}

