#!/bin/bash

KERNEL_VERSION=`uname -r`
SYSTEM_MAP_LOCATION=/boot/System.map-${KERNEL_VERSION}

if [ ! -f ${SYSTEM_MAP_LOCATION} ]
then
	echo "System map file ${SYSTEM_MAP_LOCATION} not found"
	exit -1
fi


function get_address {
	cat ${SYSTEM_MAP_LOCATION} | grep "\<$1\>" | awk '{print "0x"$1}'
}

function check_address {
	if [ "x"$2 = "x" ]
	then
		echo "Error: $1 does not exit in ${SYSTEM_MAP_LOCATION}"
		exit -1
	fi
}

ADDR_DIVIDE_ERROR=`get_address "divide_error"`
check_address divide_error $ADDR_DIVIDE_ERROR

ADDR_DO_DIVIDE_ERROR=`get_address "do_divide_error"`
check_address do_divide_error $ADDR_DO_DIVIDE_ERROR

ADDR_ERROR_ENTRY=`get_address "error_entry"`
check_address error_entry $ADDR_ERROR_ENTRY

ADDR_ERROR_EXIT=`get_address "error_exit"`
check_address error_exit $ADDR_ERROR_EXIT

insmod my_module.ko\
	addr_divide_error=$ADDR_DIVIDE_ERROR\
	addr_do_divide_error=$ADDR_DO_DIVIDE_ERROR\
	addr_error_entry=$ADDR_ERROR_ENTRY\
	addr_error_exit=$ADDR_ERROR_EXIT
