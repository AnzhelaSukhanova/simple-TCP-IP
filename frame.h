#ifndef STACK_FRAME_H
#define STACK_FRAME_H

#include <arpa/inet.h>
#include <stdint.h>

struct frame {
    unsigned char preamble[4];
    unsigned char da[6];
    unsigned char sa[6];
    uint16_t type;
    unsigned char payload[]; //46-1500 + fcs(4)
}__attribute__((packed));

struct frame* init_frame(char* buf);

#endif //STACK_FRAME_H
