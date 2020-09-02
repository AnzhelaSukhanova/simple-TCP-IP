#include "proto.h"

extern int tap_fd;

void arp(struct dev *dev, struct frame *frame, struct arp_ipv4 *packet) {
    if (dev->ip != packet->dip)
        printf("ARP-request is not for this dev\n");
    if (packet->operation == 1) {
        packet->operation = htons(2);
        memcpy(packet->dmac, packet->smac, 6);
        packet->dip = packet->sip;
        memcpy(packet->smac, dev->mac, 6);
        packet->sip = dev->ip;
        memcpy(frame->payload, packet, 28);
        memcpy(frame->sa, dev->mac, 6);
        memcpy(frame->da, packet->dmac, 6);
        frame->type = ntohs(frame->type);
        write(tap_fd, frame, 46);
    }
    else printf("Unsupported operation\n");
}

void icmp(struct dev *dev, struct frame *frame, struct ipv4 *ip_pac) {
    struct icmp *icmp_pac = (struct icmp *) ip_pac->payload;
    if (icmp_pac->type == 8) {
        if (icmp_pac->code == 0) {
            icmp_pac->type = 0;
            uint32_t for_swap = ip_pac->sip;
            ip_pac->sip = ip_pac->dip;
            ip_pac->dip = for_swap;
            memcpy(frame->da, frame->sa, 6);
            memcpy(frame->sa, dev->mac, 6);
            icmp_pac->check_sum = 0;
            ip_pac->check_sum = 0;
            icmp_pac->check_sum = htons(check_sum(icmp_pac, 64));
            ip_pac->check_sum = htons(check_sum(ip_pac, 20));
            memcpy(ip_pac->payload, icmp_pac, 64);
            memcpy(frame->payload, ip_pac, 84);
            frame->type = ntohs(frame->type);
            printf("\n");
            write(tap_fd, frame, 102);
        }
        else printf("Unsupported message code\n");
    }
    else printf("Unsupported message type\n");
}

void udp(struct dev *dev, struct frame *frame, struct ipv4 *ip_pac) {
    struct udp *udp_pac = (struct udp *) ip_pac->payload;
}
