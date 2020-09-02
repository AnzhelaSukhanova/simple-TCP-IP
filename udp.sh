#!/usr/bin/bash

set -ex

nc -u -l 1234 &
nc -u 10.0.0.2 1234 &
./main & id=$!

wait -n
kill $id
