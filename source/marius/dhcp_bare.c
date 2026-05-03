#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t op, htype, hlen, hops;
    uint32_t xid;
    uint16_t secs, flags;
    uint32_t ciaddr, yiaddr, siaddr, giaddr;
    uint8_t chaddr[16];
    uint8_t sname[64];
    uint8_t file[128];
    uint8_t magic[4];
    uint8_t options[312];
} __attribute__((packed)) DHCPPacket;

typedef struct {
    uint8_t dest[6];
    uint8_t src[6];
    uint16_t type;
} __attribute__((packed)) EthHeader;

typedef struct {
    uint8_t ver_ihl;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t flags_frag;
    uint8_t ttl;
    uint8_t proto;
    uint16_t checksum;
    uint32_t src;
    uint32_t dest;
} __attribute__((packed)) IPHeader;

typedef struct {
    uint16_t src_port;
    uint16_t dest_port;
    uint16_t len;
    uint16_t checksum;
} __attribute__((packed)) UDPHeader;

typedef struct {
    uint32_t ip;
    uint8_t mac[6];
} Lease;

static Lease leases[100];
static int lease_count = 0;
static uint32_t server_ip = 0xC0A80101;
static uint32_t ip_pool_start = 0xC0A80164;
static uint32_t ip_pool_end = 0xC0A801C8;
static uint8_t server_mac[6] = {0x52, 0x54, 0x00, 0x12, 0x34, 0x56};

void *memset(void *s, int c, size_t n) {
    uint8_t *p = s;
    while (n--) *p++ = c;
    return s;
}

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;
    while (n--) *d++ = *s++;
    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = s1, *p2 = s2;
    while (n--) {
        if (*p1 != *p2) return *p1 - *p2;
        p1++; p2++;
    }
    return 0;
}

uint16_t htons(uint16_t x) {
    return ((x & 0xFF) << 8) | ((x >> 8) & 0xFF);
}

uint32_t htonl(uint32_t x) {
    return ((x & 0xFF) << 24) | ((x & 0xFF00) << 8) | 
           ((x >> 8) & 0xFF00) | ((x >> 24) & 0xFF);
}

uint16_t ip_checksum(void *data, int len) {
    uint32_t sum = 0;
    uint16_t *p = data;
    while (len > 1) {
        sum += *p++;
        len -= 2;
    }
    if (len) sum += *(uint8_t*)p;
    while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    return ~sum;
}

uint32_t allocate_ip(uint8_t *mac) {
    for (int i = 0; i < lease_count; i++) {
        if (memcmp(leases[i].mac, mac, 6) == 0)
            return leases[i].ip;
    }
    
    for (uint32_t ip = ip_pool_start; ip <= ip_pool_end; ip++) {
        int found = 0;
        for (int i = 0; i < lease_count; i++) {
            if (leases[i].ip == ip) {
                found = 1;
                break;
            }
        }
        if (!found && lease_count < 100) {
            leases[lease_count].ip = ip;
            memcpy(leases[lease_count].mac, mac, 6);
            lease_count++;
            return ip;
        }
    }
    return 0;
}

void create_dhcp_response(uint8_t *buf, DHCPPacket *req, uint8_t msg_type, uint32_t yiaddr) {
    EthHeader *eth = (EthHeader*)buf;
    memset(eth->dest, 0xFF, 6);
    memcpy(eth->src, server_mac, 6);
    eth->type = htons(0x0800);
    
    IPHeader *ip = (IPHeader*)(buf + 14);
    memset(ip, 0, sizeof(IPHeader));
    ip->ver_ihl = 0x45;
    ip->len = htons(sizeof(IPHeader) + sizeof(UDPHeader) + sizeof(DHCPPacket));
    ip->ttl = 64;
    ip->proto = 17;
    ip->src = htonl(server_ip);
    ip->dest = 0xFFFFFFFF;
    ip->checksum = ip_checksum(ip, sizeof(IPHeader));
    
    UDPHeader *udp = (UDPHeader*)(buf + 14 + sizeof(IPHeader));
    udp->src_port = htons(67);
    udp->dest_port = htons(68);
    udp->len = htons(sizeof(UDPHeader) + sizeof(DHCPPacket));
    udp->checksum = 0;
    
    DHCPPacket *resp = (DHCPPacket*)(buf + 14 + sizeof(IPHeader) + sizeof(UDPHeader));
    memset(resp, 0, sizeof(DHCPPacket));
    resp->op = 2;
    resp->htype = 1;
    resp->hlen = 6;
    resp->xid = req->xid;
    resp->yiaddr = htonl(yiaddr);
    resp->siaddr = htonl(server_ip);
    memcpy(resp->chaddr, req->chaddr, 16);
    resp->magic[0] = 0x63; resp->magic[1] = 0x82;
    resp->magic[2] = 0x53; resp->magic[3] = 0x63;
    
    int pos = 0;
    resp->options[pos++] = 53; resp->options[pos++] = 1; resp->options[pos++] = msg_type;
    resp->options[pos++] = 54; resp->options[pos++] = 4;
    uint32_t sip = htonl(server_ip);
    memcpy(&resp->options[pos], &sip, 4); pos += 4;
    resp->options[pos++] = 51; resp->options[pos++] = 4;
    uint32_t lease = htonl(86400);
    memcpy(&resp->options[pos], &lease, 4); pos += 4;
    resp->options[pos++] = 1; resp->options[pos++] = 4;
    uint32_t mask = htonl(0xFFFFFF00);
    memcpy(&resp->options[pos], &mask, 4); pos += 4;
    resp->options[pos++] = 3; resp->options[pos++] = 4;
    uint32_t gw = htonl(server_ip);
    memcpy(&resp->options[pos], &gw, 4); pos += 4;
    resp->options[pos++] = 255;
}

int process_dhcp_packet(uint8_t *in_buf, uint8_t *out_buf) {
    EthHeader *eth = (EthHeader*)in_buf;
    if (eth->type != htons(0x0800)) return 0;
    
    IPHeader *ip = (IPHeader*)(in_buf + 14);
    if (ip->proto != 17) return 0;
    
    UDPHeader *udp = (UDPHeader*)(in_buf + 14 + sizeof(IPHeader));
    if (udp->dest_port != htons(67)) return 0;
    
    DHCPPacket *req = (DHCPPacket*)(in_buf + 14 + sizeof(IPHeader) + sizeof(UDPHeader));
    if (req->magic[0] != 0x63 || req->magic[1] != 0x82 ||
        req->magic[2] != 0x53 || req->magic[3] != 0x63) return 0;
    
    uint8_t msg_type = 0;
    for (int i = 0; i < 300 && req->options[i] != 255; i++) {
        if (req->options[i] == 53 && req->options[i+1] == 1) {
            msg_type = req->options[i+2];
            break;
        }
    }
    
    if (msg_type == 1) {
        uint32_t ip = allocate_ip(req->chaddr);
        if (ip) {
            create_dhcp_response(out_buf, req, 2, ip);
            return 14 + sizeof(IPHeader) + sizeof(UDPHeader) + sizeof(DHCPPacket);
        }
    } else if (msg_type == 3) {
        uint32_t ip = allocate_ip(req->chaddr);
        if (ip) {
            create_dhcp_response(out_buf, req, 5, ip);
            return 14 + sizeof(IPHeader) + sizeof(UDPHeader) + sizeof(DHCPPacket);
        }
    }
    return 0;
}

void dhcp_server_init(uint32_t srv_ip, uint32_t pool_start, uint32_t pool_end, uint8_t *mac) {
    server_ip = srv_ip;
    ip_pool_start = pool_start;
    ip_pool_end = pool_end;
    memcpy(server_mac, mac, 6);
    lease_count = 0;
}

int dhcp_server_process(uint8_t *rx_buf, int rx_len, uint8_t *tx_buf) {
    if (rx_len < 14 + sizeof(IPHeader) + sizeof(UDPHeader) + 240)
        return 0;
    return process_dhcp_packet(rx_buf, tx_buf);
}
