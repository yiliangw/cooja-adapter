#ifndef MM_INTERNAL_H_INCLUDED_
#define MM_INTERNAL_H_INCLUDED_

#include <stdlib.h>

#define mm_malloc(sz) malloc(sz)
#define mm_calloc(count, size) calloc((count), (size))
#define mm_realloc(p, sz) realloc((p), (sz))
#define mm_free(p) free(p)

#endif /* MM_INTERNAL_H_INCLUDED_ */