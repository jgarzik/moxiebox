#!/bin/sh

TFN=SHA256-TEST.tmp$$
BFN=random.data.sum

rm -f $FN

../src/sandbox -e sha256-test -d random.data -o $TFN
if [ $? -ne 0 ]; then
	exit 1
fi

cmp -s $TFN $BFN
RET=$?

rm -f $TFN

if [ $RET -ne 0 ]; then
	exit 1
fi

exit 0
