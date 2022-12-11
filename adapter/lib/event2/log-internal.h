#ifndef LOG_INTERNAL_H_INCLUDED_
#define LOG_INTERNAL_H_INCLUDED_

/*---------------------------------------------------------------------------*/
/* Log configuration for libevent */
#include "internal/log.h"
#define LOG_MODULE "libevent"
#define LOG_LEVEL LOG_LEVEL_LIBEVENT
/*---------------------------------------------------------------------------*/

#define event_errx(eval, ...) do {	\
	LOG_ERR(__VA_ARGS__); 			\
	LOG_ERR("\n");					\
} while (0)
#define event_err(eval, ...)	event_errx(eval, __VA_ARGS__)

#define event_warnx(...) do {	\
	LOG_WARN(__VA_ARGS__); 		\
	LOG_WARN("\n");				\
} while (0)
#define event_warn(...)			event_warnx(__VA_ARGS__)

#define event_debugx_(...) do {	\
	LOG_DBG(__VA_ARGS__); 		\
	LOG_DBG("\n");				\
} while (0)
#define event_debug(x) do {	\
	event_debugx_ x;		\
} while (0)

#endif /* LOG_INTERNAL_H_INCLUDED_ */