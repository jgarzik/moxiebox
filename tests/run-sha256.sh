#!/bin/sh

TFN=SHA256-TEST.tmp$$
BFN=random.data.sum

rm -f $FN

../src/sandbox -e sha256-test -d random.data -o $TFN
if [ $? -ne 0 ]; then
	exit 1
fi

exec cmp -q $TFN $BFN

