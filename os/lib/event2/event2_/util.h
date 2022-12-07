/*
 *
 */

#ifndef EVENT2_UTIL_H_INCLUDED_
#define EVENT2_UTIL_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <bits/types/struct_timeval.h>

#define evutil_socket_t int

#define ev_uint64_t uint64_t
#define ev_int64_t int64_t
#define ev_uint32_t uint32_t
#define ev_int32_t int32_t
#define ev_uint16_t uint16_t
#define ev_int16_t  int16_t
#define ev_uint8_t uint8_t
#define ev_int8_t int8_t
#define ev_uintptr_t uintptr_t
#define ev_intptr_t intptr_t
#define ev_ssize_t ssize_t
#define ev_off_t ev_int64_t

#define EV_UINT64_MAX UINT64_MAX
#define EV_INT64_MAX  INT64_MAX
#define EV_INT64_MIN  INT64_MIN
#define EV_UINT32_MAX UINT32_MAX
#define EV_INT32_MAX  INT32_MAX
#define EV_INT32_MIN  INT32_MIN
#define EV_UINT16_MAX UINT16_MAX
#define EV_INT16_MIN  INT16_MIN
#define EV_INT16_MAX  INT16_MAX
#define EV_UINT8_MAX  UINT8_MAX
#define EV_INT8_MAX   INT8_MAX
#define EV_INT8_MIN   INT8_MIN

#define EV_MONOT_PRECISE  1
#define EV_MONOT_FALLBACK 2

#define evutil_timeradd(tvp, uvp, vvp)					\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;       \
		if ((vvp)->tv_usec >= 1000000) {			\
			(vvp)->tv_sec++;				\
			(vvp)->tv_usec -= 1000000;			\
		}							\
	} while (0)

#define	evutil_timersub(tvp, uvp, vvp)					\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
		if ((vvp)->tv_usec < 0) {				\
			(vvp)->tv_sec--;				\
			(vvp)->tv_usec += 1000000;			\
		}							\
	} while (0)

#define	evutil_timerclear(tvp)	(tvp)->tv_sec = (tvp)->tv_usec = 0

/** Return true iff the tvp is related to uvp according to the relational
 * operator cmp.  Recognized values for cmp are ==, <=, <, >=, and >. */
#define	evutil_timercmp(tvp, uvp, cmp)					\
	(((tvp)->tv_sec == (uvp)->tv_sec) ?				\
	 ((tvp)->tv_usec cmp (uvp)->tv_usec) :				\
	 ((tvp)->tv_sec cmp (uvp)->tv_sec))

#define	evutil_timerisset(tvp)	((tvp)->tv_sec || (tvp)->tv_usec)

#define evutil_offsetof(type, field) ((off_t)(&((type *)0)->field))

#ifdef __cplusplus
}
#endif

#endif /* EVENT1_EVUTIL_H_INCLUDED_ */
