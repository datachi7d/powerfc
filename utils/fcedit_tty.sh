#!/bin/bash

WINE_COM=${2:-/home/$USER/.wine/dosdevices/com9}

/usr/bin/socat -d -d -d -d pty,raw,echo=0,link=$WINE_COM pty,raw,echo=0,link=$1
