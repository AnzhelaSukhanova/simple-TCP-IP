#include "dev.h"

int tap_fd;

void init_dev (struct dev *dev, const char *ip, unsigned char *mac) {
    inet_pton(AF_INET, ip, &dev->ip);
    memcpy(&dev->mac, mac, 6);
}
