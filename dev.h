#ifndef STACK_DEV_H
#define STACK_DEV_H

#include <stdint.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

extern int tap_fd;

struct dev {
    uint32_t ip; //IPv4
    unsigned char mac[6];
};

void init_dev (struct dev *dev, const char *ip, unsigned char *mac);

#endif //STACK_DEV_H
