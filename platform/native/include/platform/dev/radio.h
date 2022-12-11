#ifndef COOJAA_PLATFORM_RADIO_H_
#define COOJAA_PLATFORM_RADIO_H_

#define FOO_RADIO_BUFSIZE    128

struct radio_driver;
extern const struct radio_driver *foo_radio_driver;
extern const struct radio_driver *platform_radio_driver;

int foo_radio_new_packet(const void *payload, unsigned short payload_len);
void foo_radio_send_available();

#endif /* COOJAA_PLATFORM_RADIO_H_ */