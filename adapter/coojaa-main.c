#include "coojaa.h"
#include "sys/node-id.h"
#include "sys/platform.h"
#include "sys/energest.h"
#include "sys/stack-check.h"
#include "dev/watchdog.h"


#include <stdio.h>
#include <stdint.h>
/*---------------------------------------------------------------------------*/
/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "Main"
#define LOG_LEVEL LOG_LEVEL_MAIN
/*---------------------------------------------------------------------------*/
int node_entry(void);
int
coojaa_main(void)
{
  platform_init_stage_one();

  rtimer_init();
  watchdog_init();

  platform_init_stage_two();

  node_id_init();

  platform_init_stage_three();

  watchdog_start();

  node_entry();

  return 0;
}

