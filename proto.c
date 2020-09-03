#include "proto.h"

void arp(struct dev *dev, struct frame *frame, struct arp_ipv4 *packet, unsigned int frame_len) {
    if (dev->ip != packet->dip)
        printf("ARP-request is not for this dev\n");
    if (packet->operation == ARPOP_REQUEST) {
        packet->operation = htons(ARPOP_REPLY);
        memcpy(packet->dmac, packet->smac, 6);
        packet->dip = packet->sip;
        memcpy(packet->smac, dev->mac, 6);
        packet->sip = dev->ip;
        memcpy(frame->payload, packet, packet->plen);
        memcpy(frame->sa, dev->mac, 6);
        memcpy(frame->da, packet->dmac, 6);
        frame->type = htons(frame->type);
        tap_write(frame, frame_len);
    }
    else printf("Unsupported operation\n");
}

void icmp(struct dev *dev, struct frame *frame, struct ipv4 *ip_pac, unsigned int frame_len) {
    struct icmp *icmp_pac = (struct icmp *) ip_pac->payload;
    if (icmp_pac->type == 8) {
        if (icmp_pac->code == 0) {
            icmp_pac->type = 0;
            uint32_t for_swap = ip_pac->sip;
            ip_pac->sip = ip_pac->dip;
            ip_pac->dip = for_swap;
            memcpy(frame->da, frame->sa, 6);
            memcpy(frame->sa, dev->mac, 6);
            unsigned int icmp_len = ntohs(ip_pac->len) - ip_pac->ihl * 4;
            icmp_pac->check_sum = 0;
            ip_pac->check_sum = 0;
            icmp_pac->check_sum = htons(check_sum(icmp_pac, icmp_len));
            ip_pac->check_sum = htons(check_sum(ip_pac, ip_pac->ihl * 4));
            memcpy(ip_pac->payload, icmp_pac, icmp_len);
            memcpy(frame->payload, ip_pac, frame_len - FRAME_HDR_LEN);
            frame->type = ntohs(frame->type);
            tap_write(frame, frame_len);
        }
        else printf("Unsupported message code\n");
    }
    else printf("Unsupported message type\n");
}

void udp(struct dev *dev, struct frame *frame, struct ipv4 *ip_pac, unsigned int frame_len) {
    struct udp *udp_pac = (struct udp *) ip_pac->payload;
}