/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
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
 */

#include "platform/dev/radio.h"
#include "coojaa/dev/radio.h"

#include <stdio.h>
#include <string.h>

#include "coojaa.h"

#include "platform/cooja_mt.h"
#include "platform/simEnvChange.h"
#include "internal/rtimer.h"

#include "sys/energest.h"

/*---------------------------------------------------------------------------*/
/* Log configuration for foo-radio */
#include "internal/log.h"
#define LOG_MODULE "Radio"
#define LOG_LEVEL LOG_LEVEL_RADIO
/*---------------------------------------------------------------------------*/

/*
 * The maximum number of bytes this driver can accept from the MAC layer for
 * transmission or will deliver to the MAC layer after reception. Includes
 * the MAC header and payload, but not the FCS.
 */
#ifdef COOJA_RADIO_CONF_BUFSIZE
#define COOJA_RADIO_BUFSIZE COOJA_RADIO_CONF_BUFSIZE
#else
#define COOJA_RADIO_BUFSIZE 4096
#endif

#define CCA_SS_THRESHOLD -95

const struct simInterface radio_interface;



/* The radio driver can provide Cooja these values.
 * But at present, Cooja ignores and overrides them.
 * */
enum {
    /*
     * Tmote Sky (with CC2420 radio) gives value -100dB
     * CC1310 gives about -110dB
    */
    RSSI_NO_SIGNAL = -110,

    /*
     * Tmote Sky (with CC2420 radio) gives value 105
     * CC1310 gives about 100
    */
    LQI_NO_SIGNAL  = 100,
};

/* COOJA */
char simReceiving = 0;
char simInDataBuffer[COOJA_RADIO_BUFSIZE];
int simInSize = 0;
rtimer_clock_t simLastPacketTimestamp = 0;
char simOutDataBuffer[COOJA_RADIO_BUFSIZE];
int simOutSize = 0;
char simRadioHWOn = 1;
int simSignalStrength       = RSSI_NO_SIGNAL;
int simLastSignalStrength   = RSSI_NO_SIGNAL;
char simPower = 100;
int simRadioChannel = 26;
int simLQI      = LQI_NO_SIGNAL;
int simLastLQI  = LQI_NO_SIGNAL;



static const void *pending_data;

/* If we are in the polling mode, poll_mode is 1; otherwise 0 */
static int poll_mode = 0; /* default 0, disabled */
static int auto_ack = 0; /* AUTO_ACK is not supported; always 0 */
static int addr_filter = 0; /* ADDRESS_FILTER is not supported; always 0 */
static int send_on_cca = (COOJA_TRANSMIT_ON_CCA != 0);


/*---------------------------------------------------------------------------*/
int
radio_send_prepared()
{
  return simOutSize == 0;
}
/*---------------------------------------------------------------------------*/
int
radio_read_prepared()
{
  if (simInSize > 0)
    LOG_DBG("(%s) simInSize = %d", __func__, simInSize);
  return simInSize > 0;
}
/*---------------------------------------------------------------------------*/
static void
set_send_on_cca(uint8_t enable)
{
  send_on_cca = enable;
}
/*---------------------------------------------------------------------------*/
static void
set_frame_filtering(int enable)
{
  addr_filter = enable;
}
/*---------------------------------------------------------------------------*/
static void
set_auto_ack(int enable)
{
  auto_ack = enable;
}
/*---------------------------------------------------------------------------*/
static void
set_poll_mode(int enable)
{
  poll_mode = enable;
}
/*---------------------------------------------------------------------------*/
void
radio_set_channel(int channel)
{
  simRadioChannel = channel;
}
/*---------------------------------------------------------------------------*/
void
radio_set_txpower(unsigned char power)
{
  /* 1 - 100: Number indicating output power */
  simPower = power;
}
/*---------------------------------------------------------------------------*/
int
radio_signal_strength_last(void)
{
  return simLastSignalStrength;
}
/*---------------------------------------------------------------------------*/
int
radio_signal_strength_current(void)
{
  return simSignalStrength;
}
/*---------------------------------------------------------------------------*/
int
radio_LQI(void)
{
  return simLQI;
}

static
int radio_lqi_last(void)
{
  return simLastLQI;
}

/*---------------------------------------------------------------------------*/
static int
radio_on(void)
{
  ENERGEST_ON(ENERGEST_TYPE_LISTEN);
  simRadioHWOn = 1;
  return 1;
}
/*---------------------------------------------------------------------------*/
static int
radio_off(void)
{
  ENERGEST_OFF(ENERGEST_TYPE_LISTEN);
  simRadioHWOn = 0;
  return 1;
}
/*---------------------------------------------------------------------------*/
static void
doInterfaceActionsBeforeTick(void)
{
  if(!simRadioHWOn) {
    LOG_DBG("Radio off");
    simInSize = 0;
    return;
  }
  if(simReceiving) {
    simLastSignalStrength = simSignalStrength;
    simLastLQI              = simLQI;
    return;
  }

  if(simInSize > 0)
    LOG_INFO("New packet received");

}
/*---------------------------------------------------------------------------*/
static void
doInterfaceActionsAfterTick(void)
{
}
/*---------------------------------------------------------------------------*/
static int
radio_read(void *buf, size_t bufsize)
{
  int tmp = simInSize;

  if(simInSize == 0) {
    return 0;
  }
  if(bufsize < simInSize) {
    simInSize = 0; /* rx flush */
    return 0;
  }

  memcpy(buf, simInDataBuffer, simInSize);
  simInSize = 0;

  return tmp;
}
/*---------------------------------------------------------------------------*/
static int
channel_clear(void)
{
  if(simSignalStrength > CCA_SS_THRESHOLD) {
    return 0;
  }
  return 1;
}
/*---------------------------------------------------------------------------*/
static int
radio_send(const void *payload, unsigned short payload_len)
{
  int result;
  int radio_was_on = simRadioHWOn;

  if(payload_len > COOJA_RADIO_BUFSIZE) {
    return RADIO_TX_ERR;
  }
  if(payload_len == 0) {
    return RADIO_TX_ERR;
  }
  if(simOutSize > 0) {
    return RADIO_TX_ERR;
  }

  if(radio_was_on) {
    ENERGEST_SWITCH(ENERGEST_TYPE_LISTEN, ENERGEST_TYPE_TRANSMIT);
  } else {
    /* Turn on radio temporarily */
    simRadioHWOn = 1;
    ENERGEST_ON(ENERGEST_TYPE_TRANSMIT);
  }

#if COOJA_SIMULATE_TURNAROUND
  simProcessRunValue = 1;
  cooja_mt_yield();
  if(payload_len > 3) {
    simProcessRunValue = 1;
    cooja_mt_yield();
  }
#endif /* COOJA_SIMULATE_TURNAROUND */

  /* Transmit on CCA */
  if(COOJA_TRANSMIT_ON_CCA && send_on_cca && !channel_clear()) {
    result = RADIO_TX_COLLISION;
  } else {
    /* Copy packet data to temporary storage */
    memcpy(simOutDataBuffer, payload, payload_len);
    simOutSize = payload_len;

    LOG_INFO("Packet to send. simOutsize = %d", simOutSize);
    /* Transmit */
    while(simOutSize > 0) {
      cooja_mt_yield();
    }

    LOG_INFO("Packet sent");

    result = RADIO_TX_OK;
  }

  if(radio_was_on) {
    ENERGEST_SWITCH(ENERGEST_TYPE_TRANSMIT, ENERGEST_TYPE_LISTEN);
  } else {
    ENERGEST_OFF(ENERGEST_TYPE_TRANSMIT);
  }

  simRadioHWOn = radio_was_on;
  return result;
}
/*---------------------------------------------------------------------------*/
static int
prepare_packet(const void *data, unsigned short len)
{
  if(len > COOJA_RADIO_BUFSIZE) {
    return RADIO_TX_ERR;
  }
  pending_data = data;
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
transmit_packet(unsigned short len)
{
  int ret = RADIO_TX_ERR;
  if(pending_data != NULL) {
    ret = radio_send(pending_data, len);
  }
  return ret;
}
/*---------------------------------------------------------------------------*/
static int
receiving_packet(void)
{
  return simReceiving;
}
/*---------------------------------------------------------------------------*/
static int
pending_packet(void)
{
  return !simReceiving && simInSize > 0;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
static int
init(void)
{
  return 1;
}
/*---------------------------------------------------------------------------*/
static radio_result_t
get_value(radio_param_t param, radio_value_t *value)
{
  switch(param) {
  case RADIO_PARAM_RX_MODE:
    *value = 0;
    if(addr_filter) {
      *value |= RADIO_RX_MODE_ADDRESS_FILTER;
    }
    if(auto_ack) {
      *value |= RADIO_RX_MODE_AUTOACK;
    }
    if(poll_mode) {
      *value |= RADIO_RX_MODE_POLL_MODE;
    }
    return RADIO_RESULT_OK;
  case RADIO_PARAM_TX_MODE:
    *value = 0;
    if(send_on_cca) {
      *value |= RADIO_TX_MODE_SEND_ON_CCA;
    }
    return RADIO_RESULT_OK;
  case RADIO_PARAM_LAST_RSSI:
    *value = radio_signal_strength_last();
    return RADIO_RESULT_OK;

  case RADIO_PARAM_LAST_LINK_QUALITY:
    *value = radio_lqi_last();
    return RADIO_RESULT_OK;

  case RADIO_PARAM_RSSI:
    *value = radio_signal_strength_current();
    return RADIO_RESULT_OK;

  case RADIO_CONST_MAX_PAYLOAD_LEN:
    *value = (radio_value_t)COOJA_RADIO_BUFSIZE;
    return RADIO_RESULT_OK;
  default:
    return RADIO_RESULT_NOT_SUPPORTED;
  }
}
/*---------------------------------------------------------------------------*/
static radio_result_t
set_value(radio_param_t param, radio_value_t value)
{
  switch(param) {
  case RADIO_PARAM_RX_MODE:
    if(value & ~(RADIO_RX_MODE_ADDRESS_FILTER |
        RADIO_RX_MODE_AUTOACK | RADIO_RX_MODE_POLL_MODE)) {
      return RADIO_RESULT_INVALID_VALUE;
    }

    /* Only disabling is acceptable for RADIO_RX_MODE_ADDRESS_FILTER */
    if ((value & RADIO_RX_MODE_ADDRESS_FILTER) != 0) {
      return RADIO_RESULT_NOT_SUPPORTED;
    }
    set_frame_filtering((value & RADIO_RX_MODE_ADDRESS_FILTER) != 0);

    /* Only disabling is acceptable for RADIO_RX_MODE_AUTOACK */
    if ((value & RADIO_RX_MODE_ADDRESS_FILTER) != 0) {
      return RADIO_RESULT_NOT_SUPPORTED;
    }
    set_auto_ack((value & RADIO_RX_MODE_AUTOACK) != 0);

    set_poll_mode((value & RADIO_RX_MODE_POLL_MODE) != 0);
    return RADIO_RESULT_OK;
  case RADIO_PARAM_TX_MODE:
    if(value & ~(RADIO_TX_MODE_SEND_ON_CCA)) {
      return RADIO_RESULT_INVALID_VALUE;
    }
    set_send_on_cca((value & RADIO_TX_MODE_SEND_ON_CCA) != 0);
    return RADIO_RESULT_OK;
  case RADIO_PARAM_CHANNEL:
    /* With channel value < 0 Cooja matches any channels:
     *  - send packets on a negative channel -> to any receiver's channels.
     *  - receive on a negative channel <- get packets from any sender's channels.
     * So, negative channel are useful for wide-band noise generation.
     * Or for wide-band sniffing.
     * */
    radio_set_channel(value);
    return RADIO_RESULT_OK;
  default:
    return RADIO_RESULT_NOT_SUPPORTED;
  }
}
/*---------------------------------------------------------------------------*/
static radio_result_t
get_object(radio_param_t param, void *dest, size_t size)
{
  if(param == RADIO_PARAM_LAST_PACKET_TIMESTAMP) {
    if(size != sizeof(rtimer_clock_t) || !dest) {
      return RADIO_RESULT_INVALID_VALUE;
    }
    *(rtimer_clock_t *)dest = (rtimer_clock_t)simLastPacketTimestamp;
    return RADIO_RESULT_OK;
  }
  return RADIO_RESULT_NOT_SUPPORTED;
}
/*---------------------------------------------------------------------------*/
static radio_result_t
set_object(radio_param_t param, const void *src, size_t size)
{
  return RADIO_RESULT_NOT_SUPPORTED;
}
/*---------------------------------------------------------------------------*/
const struct radio_driver cooja_radio_driver =
{
    init,
    prepare_packet,
    transmit_packet,
    radio_send,
    radio_read,
    channel_clear,
    receiving_packet,
    pending_packet,
    radio_on,
    radio_off,
    get_value,
    set_value,
    get_object,
    set_object
};

/*---------------------------------------------------------------------------*/
SIM_INTERFACE(radio_interface,
              doInterfaceActionsBeforeTick,
              doInterfaceActionsAfterTick);
