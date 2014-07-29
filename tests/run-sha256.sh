#!/bin/sh

TFN=SHA256-TEST.tmp$$
BFN=$TOP_SRCDIR/tests/random.data.sum

rm -f $FN

../src/sandbox -e sha256-test -d $TOP_SRCDIR/tests/random.data -o $TFN
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
