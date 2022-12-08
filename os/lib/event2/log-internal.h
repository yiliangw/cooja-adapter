#ifndef LOG_INTERNAL_H_INCLUDED_
#define LOG_INTERNAL_H_INCLUDED_

#include <stdio.h>

#define __error(...) do { \
	printf("[Libevent Error](%s:%i):", __FILE__, __LINE__);	\
	printf(__VA_ARGS__); \
	printf("\n"); \
} while(0)

#define __warn(...) do { \
	printf("[Libevent Warning](%s:%i):", __FILE__, __LINE__);	\
	printf(__VA_ARGS__); \
	printf("\n"); \
} while(0)

#define __debug(...) do { \
	printf("[Libevent Debug](%s:%i):", __FILE__, __LINE__);	\
	printf(__VA_ARGS__); \
	printf("\n"); \
} while(0)

#define event_debugx_(...) __debug(__VA_ARGS__)

#define event_warnx(...)		__warn(__VA_ARGS__)
#define event_errx(eval, ...)	__error(__VA_ARGS__)
#define event_warn(...)			__warn(__VA_ARGS__)
#define event_debug(x) do {	\
	event_debugx_ x;		\
} while (0)

#endif /* LOG_INTERNAL_H_INCLUDED_ */