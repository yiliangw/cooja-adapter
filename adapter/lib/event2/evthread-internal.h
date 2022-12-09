#ifndef EVTHREAD_INTERNAL_H_INCLUDED_
#define EVTHREAD_INTERNAL_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

#include "util-internal.h"


/** Acquire a lock. */
#define EVLOCK_LOCK(lockvar,mode) do {(void) lockvar; (void) mode; } while (0)

/** Release a lock */
#define EVLOCK_UNLOCK(lockvar,mode) do {(void) lockvar; (void) mode; } while (0)

/** Lock an event_base, if it is set up for locking.  Acquires the lock
    in the base structure whose field is named 'lockvar'. */
#define EVBASE_ACQUIRE_LOCK(base, lockvar) do {				\
		EVLOCK_LOCK((base)->lockvar, 0);			\
	} while (0)

/** Unlock an event_base, if it is set up for locking. */
#define EVBASE_RELEASE_LOCK(base, lockvar) do {				\
		EVLOCK_UNLOCK((base)->lockvar, 0);			\
	} while (0)

/** If lock debugging is enabled, and lock is non-null, assert that 'lock' is
 * locked and held by us. */
#define EVLOCK_ASSERT_LOCKED(lock)					\
	do {								\
		if ((lock) && evthread_lock_debugging_enabled_) {	\
			EVUTIL_ASSERT(evthread_is_debug_lock_held_(lock)); \
		}							\
	} while (0)


#ifdef __cplusplus
}
#endif

#endif /* EVTHREAD_INTERNAL_H_INCLUDED_ */
