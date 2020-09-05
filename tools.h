#ifndef STACK_TOOLS_H
#define STACK_TOOLS_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "frame.h"

void print_hex(char *str, int len);
uint16_t check_sum(void *addr, int count);
void set_tap_fd(int fd);
int tap_write(struct frame *frame, unsigned int len);
int tap_read(struct frame *frame, unsigned int len);
int broadcast(uint8_t *mac);
int multicast(uint8_t *mac);

#endif //STACK_TOOLS_H