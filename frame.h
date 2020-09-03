#ifndef STACK_FRAME_H
#define STACK_FRAME_H

#include <arpa/inet.h>
#include <stdint.h>

struct frame {
    uint8_t da[6];
    uint8_t sa[6];
    uint16_t type;
    uint8_t payload[]; //46-1500 + fcs(4)
}__attribute__((packed));

#endif //STACK_FRAME_H