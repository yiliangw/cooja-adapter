#ifndef LOG_INTERNAL_H_INCLUDED_
#define LOG_INTERNAL_H_INCLUDED_

/*---------------------------------------------------------------------------*/
/* Log configuration for libevent */
#include "internal/log.h"
#define LOG_MODULE "libevent"
#define LOG_LEVEL LOG_LEVEL_LIBEVENT
/*---------------------------------------------------------------------------*/

#define event_errx(eval, ...)	LOG_ERR(__VA_ARGS__)
#define event_err(eval, ...)	event_errx(eval, __VA_ARGS__)

#define event_warnx(...) 		LOG_WARN(__VA_ARGS__)
#define event_warn(...)			event_warnx(__VA_ARGS__)

#define event_msgx(...)			LOG_INFO(__VA_ARGS__)

#define event_debugx_(...) 		LOG_DBG(__VA_ARGS__)
#define event_debug(x) do { \
	event_debugx_ x;		\
} while (0)

#endif /* LOG_INTERNAL_H_INCLUDED_ */