#!/bin/sh

../src/sandbox -e exit0
if [ $? -ne 0 ]; then
	exit 1
fi

../src/sandbox -e exit1
if [ $? -ne 1 ]; then
	exit 1
fi

