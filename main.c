#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <errno.h>
#include <net/if_arp.h>
#include <net/ethernet.h>
#include <netinet/in.h>

#include "proto.h"

#define BUFSIZE 2000

void handle_frame(struct dev *dev, struct frame *frame, unsigned int frame_len) {
    frame->type = ntohs(frame->type);
    switch (frame->type) {
        case ETH_P_ARP:
            printf("ARP\n");
            struct arp_ipv4 *arp_pac = (struct arp_ipv4 *) frame->payload;
            if (ntohs(arp_pac->hardware) == ARPHRD_ETHER && ntohs(arp_pac->protocol) == ETHERTYPE_IP) {
                arp_pac->operation = ntohs(arp_pac->operation);
                arp(dev, frame, arp_pac, frame_len);
            }
            else printf("Unsupported hardware type or/and protocol\n");
            break;
        case ETH_P_IP:
            {
            printf("IPv4\n");
            struct ipv4 *ip_pac = (struct ipv4 *) frame->payload;
            if (ip_pac->protocol == IPPROTO_ICMP) {
                printf("ICMP\n");
                icmp(dev, frame, ip_pac, frame_len);
            }
            else if (ip_pac->protocol == IPPROTO_UDP) {
                printf("UDP\n");
                udp(dev, frame, ip_pac, frame_len);
            }
            else printf("Unsupported protocol\n");
            }
            break;
        case ETH_P_IPV6:
            printf("IPv6\n");
            break;
        default:
            printf("Unrecognized EtherType %x\n", frame->type);
            break;
    }
}

int tap_alloc(char *name) //taken from Documentation/networking/tuntap.txt
{
    struct ifreq ifr;
    int fd, err;

    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
        exit(1);

    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if( *name )
        strncpy(ifr.ifr_name, name, IFNAMSIZ);

    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
        close(fd);
        return err;
    }
    strcpy(name, ifr.ifr_name);
    return fd;
}

int main() {
    char buffer[BUFSIZE];
    memset(&buffer, 0, BUFSIZE);
    char tap_name[IFNAMSIZ];
    strcpy(tap_name, "tap0");
    set_tap_fd(tap_alloc(tap_name));

    //get mac
    FILE *fp;
    unsigned char mac[6];
    if ((fp = fopen("/sys/class/net/tap0/address", "r")) == NULL)
        printf("\nERROR: reading from /sys/class/net/tap0/address: %s\n", strerror(errno));
    else {
        fscanf(fp, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
               &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
        fclose(fp);
    }

    struct dev dev;
    mac[5] = ~mac[5];
    init_dev(&dev, "10.0.0.2", mac);
    //printf("%x:%x:%x:%x:%x:%x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    while(1) {
        int nread = tap_read((struct frame *)buffer, BUFSIZE);
        if (nread < 0) {
            printf("\nERROR: reading from tap_fd: %s\n", strerror(errno));
        }
        else printf("\nRead %d bytes from device %s\n", nread, tap_name);
        handle_frame(&dev, (struct frame *)buffer, nread);
    }
}