#ifndef UTIL_INTERNAL_H_INCLUDED_
#define UTIL_INTERNAL_H_INCLUDED_

#include "lib/assert.h"
#include "event2_/util.h"

#define EVUTIL_FALLTHROUGH __attribute__((fallthrough))

#define EVUTIL_ASSERT(cond) assert(cond)
#define EVUTIL_UNLIKELY(p) (p)
#define EVUTIL_FAILURE_CHECK(cond) EVUTIL_UNLIKELY(cond)

#define EVUTIL_UPCAST(ptr, type, field)				\
	((type *)(((char*)(ptr)) - evutil_offsetof(type, field)))

#endif /* UTIL_INTERNAL_H_INCLUDED_ */