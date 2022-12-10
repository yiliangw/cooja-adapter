#ifndef COOJAA_PLATFORM_EVENT2_H_
#define COOJAA_PLATFORM_EVENT2_H_

struct event_base;
struct timeval;

int platform_coojaa_dispatch(struct event_base *base, struct timeval *tv);

#endif /* COOJAA_PLATFORM_EVENT2_H_ */
