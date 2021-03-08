#!/bin/bash

COM1=${1:-/tmp/sim1}
COM2=${2:-/tmp/sim2}

/usr/bin/socat -d -d -d -d pty,raw,echo=0,link=$COM1 pty,raw,echo=0,link=$COM2