#!/usr/bin/bash

set -ex

if [ "$1" = "" ]; then
    if [ ! -e /sys/class/net/tap0 ]; then
        ip tuntap add dev tap0 mode tap
    else 
        ip addr flush dev tap0
    fi
    ip link set tap0 up
    ip addr add 10.0.0.1/32 dev tap0
    ip route add 10.0.0.2/32 dev tap0
else
    ip link delete tap0
fi
