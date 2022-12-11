#ifndef COOJAA_PLATFORM_RADIO_H_
#define COOJAA_PLATFORM_RADIO_H_

struct radio_driver;
extern const struct radio_driver cooja_radio_driver;
#define PLATFORM_RADIO() cooja_radio_driver

#ifdef COOJA_CONF_SIMULATE_TURNAROUND
#define COOJA_SIMULATE_TURNAROUND COOJA_CONF_SIMULATE_TURNAROUND
#else
#define COOJA_SIMULATE_TURNAROUND (!(MAC_CONF_WITH_TSCH))
#endif

#ifdef COOJA_CONF_TRANSMIT_ON_CCA
#define COOJA_TRANSMIT_ON_CCA COOJA_CONF_TRANSMIT_ON_CCA
#else
#define COOJA_TRANSMIT_ON_CCA 1
#endif

int
radio_send_prepared();

int
radio_read_prepared();


/**
 * Set radio channel.
 */
void
radio_set_channel(int channel);

/**
 * Set transmission power of transceiver.
 *
 * \param p The power of the transceiver, between 1 and 100.
 */
void
radio_set_txpower(unsigned char p);

/**
 * The signal strength of the last received packet
 */
int
radio_signal_strength_last(void);

/**
 * This current signal strength.
 */
int
radio_signal_strength_current(void);

/**
 * Link quality indicator of last received packet.
 */
int
radio_LQI(void);


#endif /* COOJAA_PLATFORM_RADIO_H_ */