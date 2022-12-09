#ifndef MINHEAP_INTERNAL_H_INCLUDED_
#define MINHEAP_INTERNAL_H_INCLUDED_

#include "coojaa/event2/event_struct.h"
#include "coojaa/event2/util.h"

#include "mm-internal.h"

typedef struct min_heap
{
	struct event** p;
	unsigned n, a;
} min_heap_t;

void 			min_heap_ctor_(min_heap_t* s);
int	     		min_heap_empty_(min_heap_t* s);
struct event*  	min_heap_top_(min_heap_t* s);
int	     		min_heap_elt_is_top_(const struct event *e);
int	     		min_heap_push_(min_heap_t* s, struct event* e);
void	     	min_heap_shift_up_(min_heap_t* s, unsigned hole_index, struct event* e);
void	     	min_heap_shift_up_unconditional_(min_heap_t* s, unsigned hole_index, struct event* e);
void	     	min_heap_shift_down_(min_heap_t* s, unsigned hole_index, struct event* e);
int	     		min_heap_reserve_(min_heap_t* s, unsigned n);
unsigned	    min_heap_size_(min_heap_t* s);
int 			min_heap_erase_(min_heap_t* s, struct event* e);
void 			min_heap_elem_init_(struct event* e);

#endif /* MINHEAP_INTERNAL_H_INCLUDED_ */
