#ifndef UTIL_INTERNAL_H_INCLUDED_
#define UTIL_INTERNAL_H_INCLUDED_

#include "lib/assert.h"
#include "sys/rtimer.h"

#include "coojaa/event2/util.h"

#include <bits/types/struct_timeval.h>

#define EVUTIL_FALLTHROUGH __attribute__((fallthrough))

#define EVUTIL_ASSERT(cond) assert(cond)
#define EVUTIL_UNLIKELY(p) (p)
#define EVUTIL_FAILURE_CHECK(cond) EVUTIL_UNLIKELY(cond)

#define EVUTIL_UPCAST(ptr, type, field)				\
	((type *)(((char*)(ptr)) - evutil_offsetof(type, field)))

int clocktime_to_timeval(rtimer_clock_t clocktime, struct timeval *tv);
rtimer_clock_t timeval_to_clocktime(struct timeval *tv);

#endif /* UTIL_INTERNAL_H_INCLUDED_ */