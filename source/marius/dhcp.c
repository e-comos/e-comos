#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define close closesocket
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

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
} DHCPPacket;

typedef struct {
    uint32_t ip;
    uint8_t mac[6];
    int used;
} Lease;

Lease leases[100];
int lease_count = 0;

uint32_t ip_start, ip_end, server_ip, subnet_mask, gateway;

void init_network() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif
}

uint32_t str_to_ip(const char *str) {
    return inet_addr(str);
}

void ip_to_str(uint32_t ip, char *buf) {
    struct in_addr addr;
    addr.s_addr = ip;
    strcpy(buf, inet_ntoa(addr));
}

uint32_t allocate_ip(uint8_t *mac) {
    for (int i = 0; i < lease_count; i++) {
        if (memcmp(leases[i].mac, mac, 6) == 0)
            return leases[i].ip;
    }
    
    uint32_t ip = ntohl(ip_start);
    uint32_t end = ntohl(ip_end);
    
    for (uint32_t i = ip; i <= end; i++) {
        uint32_t test_ip = htonl(i);
        int found = 0;
        for (int j = 0; j < lease_count; j++) {
            if (leases[j].ip == test_ip) {
                found = 1;
                break;
            }
        }
        if (!found) {
            leases[lease_count].ip = test_ip;
            memcpy(leases[lease_count].mac, mac, 6);
            leases[lease_count].used = 1;
            lease_count++;
            return test_ip;
        }
    }
    return 0;
}

void create_response(DHCPPacket *resp, DHCPPacket *req, uint8_t msg_type, uint32_t yiaddr) {
    memset(resp, 0, sizeof(DHCPPacket));
    resp->op = 2;
    resp->htype = 1;
    resp->hlen = 6;
    resp->xid = req->xid;
    resp->yiaddr = yiaddr;
    resp->siaddr = server_ip;
    memcpy(resp->chaddr, req->chaddr, 16);
    resp->magic[0] = 0x63; resp->magic[1] = 0x82;
    resp->magic[2] = 0x53; resp->magic[3] = 0x63;
    
    int pos = 0;
    resp->options[pos++] = 53; resp->options[pos++] = 1; resp->options[pos++] = msg_type;
    resp->options[pos++] = 54; resp->options[pos++] = 4; memcpy(&resp->options[pos], &server_ip, 4); pos += 4;
    resp->options[pos++] = 51; resp->options[pos++] = 4; uint32_t lease = htonl(86400); memcpy(&resp->options[pos], &lease, 4); pos += 4;
    resp->options[pos++] = 1; resp->options[pos++] = 4; memcpy(&resp->options[pos], &subnet_mask, 4); pos += 4;
    resp->options[pos++] = 3; resp->options[pos++] = 4; memcpy(&resp->options[pos], &gateway, 4); pos += 4;
    resp->options[pos++] = 255;
}

void run_server() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
    
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(67);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    printf("[Server] Started\n");
    
    while (1) {
        DHCPPacket req, resp;
        struct sockaddr_in client;
        int len = sizeof(client);
        
        int n = recvfrom(sock, (char*)&req, sizeof(req), 0, (struct sockaddr*)&client, &len);
        if (n < 240) continue;
        
        uint8_t msg_type = 0;
        for (int i = 0; i < 300 && req.options[i] != 255; i++) {
            if (req.options[i] == 53 && req.options[i+1] == 1) {
                msg_type = req.options[i+2];
                break;
            }
        }
        
        if (msg_type == 1) {
            uint32_t ip = allocate_ip(req.chaddr);
            if (ip) {
                create_response(&resp, &req, 2, ip);
                client.sin_port = htons(68);
                client.sin_addr.s_addr = INADDR_BROADCAST;
                sendto(sock, (char*)&resp, sizeof(resp), 0, (struct sockaddr*)&client, sizeof(client));
                char buf[20];
                ip_to_str(ip, buf);
                printf("[Server] OFFER: %s\n", buf);
            }
        } else if (msg_type == 3) {
            uint32_t ip = allocate_ip(req.chaddr);
            if (ip) {
                create_response(&resp, &req, 5, ip);
                client.sin_port = htons(68);
                client.sin_addr.s_addr = INADDR_BROADCAST;
                sendto(sock, (char*)&resp, sizeof(resp), 0, (struct sockaddr*)&client, sizeof(client));
                char buf[20];
                ip_to_str(ip, buf);
                printf("[Server] ACK: %s\n", buf);
            }
        }
    }
    close(sock);
}

void run_client() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
    
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(68);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    
    struct timeval tv = {5, 0};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
    
    DHCPPacket pkt = {0};
    pkt.op = 1; pkt.htype = 1; pkt.hlen = 6;
    pkt.xid = rand();
    for (int i = 0; i < 6; i++) pkt.chaddr[i] = rand() % 256;
    pkt.magic[0] = 0x63; pkt.magic[1] = 0x82; pkt.magic[2] = 0x53; pkt.magic[3] = 0x63;
    pkt.options[0] = 53; pkt.options[1] = 1; pkt.options[2] = 1; pkt.options[3] = 255;
    
    struct sockaddr_in dest = {0};
    dest.sin_family = AF_INET;
    dest.sin_port = htons(67);
    dest.sin_addr.s_addr = INADDR_BROADCAST;
    
    printf("[Client] Sending DISCOVER...\n");
    sendto(sock, (char*)&pkt, sizeof(pkt), 0, (struct sockaddr*)&dest, sizeof(dest));
    
    DHCPPacket resp;
    int len = sizeof(dest);
    if (recvfrom(sock, (char*)&resp, sizeof(resp), 0, (struct sockaddr*)&dest, &len) > 0) {
        if (resp.xid == pkt.xid) {
            char buf[20];
            ip_to_str(resp.yiaddr, buf);
            printf("[Client] Received OFFER: %s\n", buf);
            
            pkt.options[2] = 3;
            int pos = 3;
            pkt.options[pos++] = 50; pkt.options[pos++] = 4; memcpy(&pkt.options[pos], &resp.yiaddr, 4); pos += 4;
            pkt.options[pos++] = 54; pkt.options[pos++] = 4; memcpy(&pkt.options[pos], &resp.siaddr, 4); pos += 4;
            pkt.options[pos++] = 255;
            
            printf("[Client] Sending REQUEST...\n");
            sendto(sock, (char*)&pkt, sizeof(pkt), 0, (struct sockaddr*)&dest, sizeof(dest));
            
            if (recvfrom(sock, (char*)&resp, sizeof(resp), 0, (struct sockaddr*)&dest, &len) > 0) {
                if (resp.xid == pkt.xid) {
                    ip_to_str(resp.yiaddr, buf);
                    printf("[Client] IP: %s\n", buf);
                }
            }
        }
    } else {
        printf("[Client] Timeout\n");
    }
    close(sock);
}

int main(int argc, char *argv[]) {
    init_network();
    
    if (argc < 2) {
        printf("Usage: dhcp [server|client]\n");
        return 1;
    }
    
    if (strcmp(argv[1], "server") == 0) {
        ip_start = str_to_ip("192.168.1.100");
        ip_end = str_to_ip("192.168.1.200");
        server_ip = str_to_ip("192.168.1.1");
        subnet_mask = str_to_ip("255.255.255.0");
        gateway = str_to_ip("192.168.1.1");
        run_server();
    } else if (strcmp(argv[1], "client") == 0) {
        run_client();
    }
    
    return 0;
}
