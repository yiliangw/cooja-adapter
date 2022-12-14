#include "coojaa.h"
#include "sys/node-id.h"
#include "sys/platform.h"
#include "sys/energest.h"
#include "sys/stack-check.h"
#include "coojaa/dev/watchdog.h"
#include "internal/clock.h"
#include "internal/rtimer.h"


#include <stdio.h>
#include <stdint.h>
/*---------------------------------------------------------------------------*/
/* Log configuration */
#include "internal/log.h"
#define LOG_MODULE "Main"
#define LOG_LEVEL LOG_LEVEL_MAIN
/*---------------------------------------------------------------------------*/

int node_entry(void);
int
coojaa_main(void)
{
  platform_init_stage_one();

  clock_init();
  rtimer_init();
  watchdog_init();

  platform_init_stage_two();

  node_id_init();

  platform_init_stage_three();

  watchdog_start();

  return node_entry();
}

