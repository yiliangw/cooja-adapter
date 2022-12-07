#include "contiki.h"
#include "contiki-net.h"
#include "sys/node-id.h"
#include "sys/platform.h"
#include "sys/energest.h"
#include "sys/stack-check.h"
#include "dev/watchdog.h"

#include "net/queuebuf.h"

#include <stdio.h>
#include <stdint.h>
/*---------------------------------------------------------------------------*/
/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "Main"
#define LOG_LEVEL LOG_LEVEL_MAIN
/*---------------------------------------------------------------------------*/
int
main(void)
{
  platform_init_stage_one();

  clock_init();
  rtimer_init();
  process_init();
  ctimer_init();
  watchdog_init();

  platform_init_stage_two();

  queuebuf_init();
  netstack_init();
  node_id_init();

  platform_init_stage_three();

  watchdog_start();

  platform_main_loop();

  return 0;
}

