#!/bin/bash -x

./utils/socat.sh &
sleep 1
./build/src/tools/pfc_sim_dump  --serial /tmp/sim1 --data ./src/tests/test_memory_dump2.dat