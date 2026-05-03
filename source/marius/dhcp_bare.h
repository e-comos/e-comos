#ifndef DHCP_BARE_H
#define DHCP_BARE_H

#include <stdint.h>

void dhcp_server_init(uint32_t srv_ip, uint32_t pool_start, uint32_t pool_end, uint8_t *mac);
int dhcp_server_process(uint8_t *rx_buf, int rx_len, uint8_t *tx_buf);

#endif
