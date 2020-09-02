#ifndef STACK_TOOLS_H
#define STACK_TOOLS_H

#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

void print_hex(char *str, int len);
uint16_t check_sum(void *addr, int count);

#endif //STACK_TOOLS_H
