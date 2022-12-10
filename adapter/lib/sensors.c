/*
 * Copyright (c) 2009, Swedish Institute of Computer Science
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

#include <string.h>

#include "coojaa.h"

#include "lib/sensors.h"

extern const struct sensors_sensor *sensors[];
extern unsigned char sensors_flags[];

#define FLAG_CHANGED    0x80


static unsigned char num_sensors;


/*---------------------------------------------------------------------------*/
static int
get_sensor_index(const struct sensors_sensor *s)
{
  int i;
  for(i = 0; i < num_sensors; ++i) {
    if(sensors[i] == s) {
      return i;
    }
  }
  return i;
}
/*---------------------------------------------------------------------------*/
const struct sensors_sensor *
sensors_first(void)
{
  return sensors[0];
}
/*---------------------------------------------------------------------------*/
const struct sensors_sensor *
sensors_next(const struct sensors_sensor *s)
{
  return sensors[get_sensor_index(s) + 1];
}
/*---------------------------------------------------------------------------*/
void
sensors_changed(const struct sensors_sensor *s)
{
  sensors_flags[get_sensor_index(s)] |= FLAG_CHANGED;
}
/*---------------------------------------------------------------------------*/
const struct sensors_sensor *
sensors_find(const char *prefix)
{
  int i;
  unsigned short len;

  /* Search through all processes and search for the specified process
     name. */
  len = strlen(prefix);

  for(i = 0; i < num_sensors; ++i) {
    if(strncmp(prefix, sensors[i]->type, len) == 0) {
      return sensors[i];
    }
  }
  return NULL;
}
