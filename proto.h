#ifndef STACK_PROTO_H
#define STACK_PROTO_H

#include <stdint.h>
#include <net/if_arp.h>
#include "dev.h"
#include "frame.h"
#include "tools.h"

#define FRAME_HDR_LEN 18

struct arp_ipv4 {
    uint16_t hardware;
    uint16_t protocol;
    uint8_t hlen;
    uint8_t plen;
    uint16_t operation;
    uint8_t smac[6];
    uint32_t sip;
    uint8_t dmac[6];
    uint32_t dip;
}__attribute__((packed));

struct ipv4 {
    uint8_t ihl : 4;
    uint8_t version : 4;
    uint8_t type;
    uint16_t len;
    uint16_t id;
    uint16_t flags_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check_sum;
    uint32_t sip;
    uint32_t dip;
    uint8_t payload[];
} __attribute__((packed));

struct icmp {
    uint8_t type;
    uint8_t code;
    uint16_t check_sum;
    uint32_t info;
    uint8_t payload[];
}__attribute__((packed));

struct udp {
    uint16_t sport;
    uint16_t dport;
    uint16_t len;
    uint16_t check_sum;
    uint8_t payload[];
}__attribute__((packed));

/*struct ipv6{
    uint8_t version : 4;
    uint8_t traf_class;
    uint32_t flow_label : 20;
    uint16_t len;
    uint8_t protocol;
    uint8_t ttl;
    unsigned __int128 sip;
    unsigned __int128 dip;
    uint8_t payload[];
}__attribute__((packed));*/

void arp(struct dev *dev, struct frame *frame, struct arp_ipv4 *packet, unsigned int frame_len);
void icmp(struct dev *dev, struct frame *frame, struct ipv4 *packet, unsigned int frame_len);
void udp(struct dev *dev, struct frame *frame, struct ipv4 *ip_pac, unsigned int frame_len);

#endif //STACK_PROTO_H