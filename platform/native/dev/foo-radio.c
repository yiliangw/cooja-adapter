#include "platform/dev/radio.h"

#include <stdio.h>
#include <string.h>

#include "coojaa.h"
#include "coojaa/dev/radio.h"

#define FOO_RADIO_BUFSIZE    128
static int have_packet = 0;
static char foo_radio_buffer[FOO_RADIO_BUFSIZE];

static int init(void);
static int prepare_packet(const void *payload, unsigned short payload_len);
static int transmit_packet(unsigned short transmit_len);
static int radio_send(const void *payload, unsigned short payload_len);
static int radio_read(void *buf, size_t buf_len);
static int channel_clear(void);
static int receiving_packet(void);
static int pending_packet(void);
static int radio_on(void);
static int radio_off(void);
static radio_result_t get_value(radio_param_t param, radio_value_t *value);
static radio_result_t set_value(radio_param_t param, radio_value_t value);
static radio_result_t get_object(radio_param_t param, void *dest, size_t size);
static radio_result_t set_object(radio_param_t param, const void *src, size_t size);

const struct radio_driver foo_radio_driver_ =
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
const struct radio_driver *foo_radio_driver = &foo_radio_driver_;
const struct radio_driver *platform_radio_driver = &foo_radio_driver_;

/* To act as if a new packet is received. */
int foo_radio_new_packet(const void *payload, unsigned short payload_len)
{
    if (have_packet)
        return -1;

    memcpy(foo_radio_buffer, payload, payload_len);
    have_packet = 1;
    return 0;
}

static int init(void)
 {
    return 0;
 }

static int prepare_packet(const void *payload, unsigned short payload_len)
{
    return 0;
}

static int transmit_packet(unsigned short transmit_len)
{
    return 0;
}

static int radio_send(const void *payload, unsigned short payload_len)
{
    printf("Message sent: %s\n", (char*) payload);
    return RADIO_TX_OK;
}

static int radio_read(void *buf, size_t buf_len)
{
    return 0;
}

static int channel_clear(void)
{
    return 0;
}

static int receiving_packet(void)
{
    return 0;
}

static int pending_packet(void)
{
    return 0;
}

static int radio_on(void)
{
    return 0;
}

static int radio_off(void)
{
    return 0;
}

static radio_result_t get_value(radio_param_t param, radio_value_t *value)
{
    return 0;
}

static radio_result_t set_value(radio_param_t param, radio_value_t value)
{
    return 0;
}

static radio_result_t get_object(radio_param_t param, void *dest, size_t size)
{
    return 0;
}

static radio_result_t set_object(radio_param_t param, const void *src, size_t size)
{
    return 0;
}

