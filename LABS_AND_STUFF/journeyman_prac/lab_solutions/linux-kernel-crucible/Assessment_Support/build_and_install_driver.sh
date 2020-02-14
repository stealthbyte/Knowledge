#!/bin/bash

if [[ ${UID} -eq 0 ]]; then
    echo "Must not be run as root!"
    exit
fi

sudo -v
sudo rm /dev/vlnmodule
sudo rmmod vlnmodule
make clean
make
sudo insmod vlnmodule.ko
sudo mknod /dev/vlnmodule c 511 411
sudo chmod a+rw /dev/vlnmodule
