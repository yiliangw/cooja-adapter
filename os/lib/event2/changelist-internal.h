#ifndef CHANGELIST_INTERNAL_H_INCLUDED_
#define CHANGELIST_INTERNAL_H_INCLUDED_

#include "event2_/util.h"

struct event_change {
	evutil_socket_t fd;
	short old_events;

	ev_uint8_t read_change;
	ev_uint8_t write_change;
	ev_uint8_t close_change;
};

void event_changelist_init_(struct event_changelist *changelist);

#endif /* CHANGELIST_INTERNAL_H_INCLUDED_ */