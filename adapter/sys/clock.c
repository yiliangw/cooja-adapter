#include "internal/clock.h"

int clocktime_to_timeval(clock_time_t clocktime, struct timeval *tv)
{
    tv->tv_sec = clocktime / CLOCK_SECOND;
    tv->tv_usec = (unsigned) (clocktime % CLOCK_SECOND / CLOCK_USECOND);
    return 0;
}

clock_time_t timeval_to_clocktime(struct timeval *tv)
{
    return (clock_time_t) (tv->tv_sec * CLOCK_SECOND + tv->tv_usec * CLOCK_USECOND);
}
