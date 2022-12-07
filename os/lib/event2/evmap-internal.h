#ifndef EVMAP_INTERNAL_H_INCLUDED_
#define EVMAP_INTERNAL_H_INCLUDED_

#include "event2_/event_struct.h"

struct event_base;
struct event;

#define event_io_map event_signal_map

int evmap_io_add_(struct event_base *base, evutil_socket_t fd, struct event *ev);
int evmap_io_del_(struct event_base *base, evutil_socket_t fd, struct event *ev);
int evmap_signal_add_(struct event_base *base, int sig, struct event *ev);
int evmap_signal_del_(struct event_base *base, int sig, struct event *ev);
void evmap_io_initmap_(struct event_io_map* ctx);
void evmap_signal_initmap_(struct event_signal_map *ctx);

#endif /* EVMAP_INTERNAL_H_INCLUDED_ */
