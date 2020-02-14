#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Usage: $0 <vm name>" > /dev/stderr
	exit -1
fi

VBoxManage controlvm "$1" keyboardputscancode 1d 38 54 22 a2 d4 b8 9d
