/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

#ifndef PLATFORM_CONF_H_
#define PLATFORM_CONF_H_

/* include the project config */
#ifdef PROJECT_CONF_PATH
#include PROJECT_CONF_PATH
#endif /* PROJECT_CONF_PATH */

#ifdef INCLUDE_SUBPLATFORM_CONF
#include "subplatform-conf.h"
#endif /* INCLUDE_SUBPLATFORM_CONF */

#define PLATFORM_CONF_PROVIDES_MAIN_LOOP 1

#define LOG_CONF_ENABLED 1

#define COOJA 1

#define ASSERT_CONF_RETURNS  1

#ifndef EEPROM_CONF_SIZE
#define EEPROM_CONF_SIZE				1024
#endif

#define w_memcpy memcpy

/* Radio setup */
#define NETSTACK_CONF_RADIO cooja_radio_driver

#define CC_CONF_VA_ARGS                1
#define CC_CONF_INLINE inline

/* These names are deprecated, use C99 names. */
#include <inttypes.h>
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef int32_t s32_t;

typedef unsigned short uip_stats_t;

#define CLOCK_CONF_SECOND 1000L
typedef unsigned long clock_time_t;

/* Use 64-bit rtimer (default in Contiki-NG is 32) */
#define RTIMER_CONF_CLOCK_SIZE 8

/* 1 len byte, 2 bytes CRC */
#define RADIO_PHY_OVERHEAD         3
/* 250kbps data rate. One byte = 32us */
#define RADIO_BYTE_AIR_TIME       32
#define RADIO_DELAY_BEFORE_TX 0
#define RADIO_DELAY_BEFORE_RX 0
#define RADIO_DELAY_BEFORE_DETECT 0

#define UIP_ARCH_IPCHKSUM        1

#define CFS_CONF_OFFSET_TYPE	long

#define PLATFORM_CONF_SUPPORTS_STACK_CHECK  0

/*---------------------------------------------------------------------------*/
/* Support for the new GPIO HAL */
#define GPIO_HAL_CONF_ARCH_HDR_PATH      "coojaa/dev/gpio-hal-arch.h"
#define GPIO_HAL_CONF_ARCH_SW_TOGGLE     1
#define GPIO_HAL_CONF_PORT_PIN_NUMBERING 0
#define GPIO_HAL_CONF_PIN_COUNT          4

/* Virtual LED pins 0, 1, 2 (Green, Red, Yellow) */
#define COOJA_LED_GREEN_PIN              0
#define COOJA_LED_RED_PIN                1
#define COOJA_LED_YELLOW_PIN             2

/* Virtual button on pin 3 */
#define COOJA_BTN_PIN                    3

#define BUTTON_HAL_CONF_DEBOUNCE_DURATION 0

/* Notify various examples that we have Buttons */
#define PLATFORM_HAS_BUTTON    1
#define PLATFORM_SUPPORTS_BUTTON_HAL 1
/*---------------------------------------------------------------------------*/
/* Virtual LED colors */
#define LEDS_CONF_COUNT                  3
#define LEDS_CONF_GREEEN                 1
#define LEDS_CONF_RED                    2
#define LEDS_CONF_YELLOW                 4
/*---------------------------------------------------------------------------*/
#endif /* PLATFORM_CONF_H_ */
