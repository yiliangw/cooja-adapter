#ifndef EVSIGNAL_INTERNAL_H_INCLUDED_
#define EVSIGNAL_INTERNAL_H_INCLUDED_

#include "coojaa/event2/event_struct.h"

struct evsig_info {
	struct event ev_signal;
	evutil_socket_t ev_signal_pair[2];
	int ev_signal_added;
	int ev_n_signals_added;
	struct sigaction **sh_old;
	int sh_old_max;
};

#endif /* EVSIGNAL_INTERNAL_H_INCLUDED_ */