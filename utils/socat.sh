#!/bin/bash

COM1=${1:-/tmp/sim1}
COM2=${2:-/tmp/sim2}
DEBUG="-d -d -d -d"

/usr/bin/socat pty,raw,echo=0,link=$COM1 pty,raw,echo=0,link=$COM2