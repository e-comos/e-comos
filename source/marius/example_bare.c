#include "dhcp_bare.h"

extern void network_send(uint8_t *buf, int len);
extern int network_recv(uint8_t *buf, int max_len);

void dhcp_main() {
    uint8_t mac[6] = {0x52, 0x54, 0x00, 0x12, 0x34, 0x56};
    dhcp_server_init(0xC0A80101, 0xC0A80164, 0xC0A801C8, mac);
    
    uint8_t rx_buf[2048];
    uint8_t tx_buf[2048];
    
    while (1) {
        int len = network_recv(rx_buf, sizeof(rx_buf));
        if (len > 0) {
            int tx_len = dhcp_server_process(rx_buf, len, tx_buf);
            if (tx_len > 0) {
                network_send(tx_buf, tx_len);
            }
        }
    }
}
