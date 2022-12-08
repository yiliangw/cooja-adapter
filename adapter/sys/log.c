/*
 * Copyright (c) 2017, RISE SICS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Header file for the logging system
 * \author
 *         Simon Duquennoy <simon.duquennoy@ri.se>
 */

/** \addtogroup sys
 * @{ */

/**
 * \defgroup log Per-module, per-level logging
 * @{
 *
 * The log module performs per-module, per-level logging
 *
 */

#include <stdint.h>
#include <string.h>

#include "sys/log.h"

int curr_log_level_rpl = LOG_CONF_LEVEL_RPL;
int curr_log_level_tcpip = LOG_CONF_LEVEL_TCPIP;
int curr_log_level_ipv6 = LOG_CONF_LEVEL_IPV6;
int curr_log_level_6lowpan = LOG_CONF_LEVEL_6LOWPAN;
int curr_log_level_nullnet = LOG_CONF_LEVEL_NULLNET;
int curr_log_level_mac = LOG_CONF_LEVEL_MAC;
int curr_log_level_framer = LOG_CONF_LEVEL_FRAMER;
int curr_log_level_6top = LOG_CONF_LEVEL_6TOP;
int curr_log_level_coap = LOG_CONF_LEVEL_COAP;
int curr_log_level_snmp = LOG_CONF_LEVEL_SNMP;
int curr_log_level_lwm2m = LOG_CONF_LEVEL_LWM2M;
int curr_log_level_main = LOG_CONF_LEVEL_MAIN;

struct log_module all_modules[] = {
  {"rpl", &curr_log_level_rpl, LOG_CONF_LEVEL_RPL},
  {"tcpip", &curr_log_level_tcpip, LOG_CONF_LEVEL_TCPIP},
  {"ipv6", &curr_log_level_ipv6, LOG_CONF_LEVEL_IPV6},
  {"6lowpan", &curr_log_level_6lowpan, LOG_CONF_LEVEL_6LOWPAN},
  {"nullnet", &curr_log_level_nullnet, LOG_CONF_LEVEL_NULLNET},
  {"mac", &curr_log_level_mac, LOG_CONF_LEVEL_MAC},
  {"framer", &curr_log_level_framer, LOG_CONF_LEVEL_FRAMER},
  {"6top", &curr_log_level_6top, LOG_CONF_LEVEL_6TOP},
  {"coap", &curr_log_level_coap, LOG_CONF_LEVEL_COAP},
  {"snmp", &curr_log_level_snmp, LOG_CONF_LEVEL_SNMP},
  {"lwm2m", &curr_log_level_lwm2m, LOG_CONF_LEVEL_LWM2M},
  {"main", &curr_log_level_main, LOG_CONF_LEVEL_MAIN},
  {NULL, NULL, 0},
};


/*---------------------------------------------------------------------------*/
void
log_bytes(const void *data, size_t length)
{
  const uint8_t *u8data = (const uint8_t *)data;
  size_t i;
  for(i = 0; i != length; ++i) {
    LOG_OUTPUT("%02x", u8data[i]);
  }
}
/*---------------------------------------------------------------------------*/
#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif
void
log_set_level(const char *module, int level)
{
  if(level >= LOG_LEVEL_NONE && level <= LOG_LEVEL_DBG) {
    int i = 0;
    int module_all = !strcmp("all", module);
    while(all_modules[i].name != NULL) {
      if(module_all || !strcmp(module, all_modules[i].name)) {
        *all_modules[i].curr_log_level = MIN(level, all_modules[i].max_log_level);
      }
      i++;
    }
  }
}
/*---------------------------------------------------------------------------*/
int
log_get_level(const char *module)
{
  int i = 0;
  if(module == NULL) {
    return -1;
  }
  while(all_modules[i].name != NULL) {
    if(!strcmp(module, all_modules[i].name)) {
      return *all_modules[i].curr_log_level;
    }
    i++;
  }
  return -1;
}
/*---------------------------------------------------------------------------*/
const char *
log_level_to_str(int level)
{
  switch(level) {
    case LOG_LEVEL_NONE:
      return "None";
    case LOG_LEVEL_ERR:
      return "Errors";
    case LOG_LEVEL_WARN:
      return "Warnings";
    case LOG_LEVEL_INFO:
      return "Info";
    case LOG_LEVEL_DBG:
      return "Debug";
    default:
      return "N/A";
  }
}
/** @} */
/** @} */