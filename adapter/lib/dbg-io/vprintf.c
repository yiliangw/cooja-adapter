#include "coojaa.h"
#include "lib/dbg-io/dbg.h"

#include <stdio.h>
#include <string.h>
#include "strformat.h"
#undef vprintf

static strformat_result
write_str(void *user_data, const char *data, unsigned int len)
{
  if(len > 0) {
    dbg_send_bytes((unsigned char *)data, len);
  }
  return STRFORMAT_OK;
}

static strformat_context_t ctxt =
{
  write_str,
  NULL
};

int
vprintf(const char *fmt, va_list va)
{
  int res;
  res = format_str_v(&ctxt, fmt, va);
  return res;
}