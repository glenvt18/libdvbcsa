#!/bin/sh

OUT=/tmp/tests
SCRIPT_OUT=/tmp

rm -rf $OUT
mkdir -p $OUT

[ -f $SCRIPT_OUT/run-bench.sh ] || cp run-bench.sh $SCRIPT_OUT

make clean

for br in $(git branch | grep -o 'bench-.*'); do
    git checkout $br
    ./bootstrap
    for bsopt in uint32 uint64 neon; do
        #./configure --enable-$bsopt --with-sysroot=/usr/arm-linux-gnueabihf --disable-shared --enable-static --host=arm-linux-gnueabihf
        ./configure --enable-$bsopt --with-sysroot=/usr/aarch64-linux-gnu --disable-shared --enable-static --host=aarch64-linux-gnu
        make -j2
        mkdir -p $OUT/$br/$bsopt 
        cp test/benchbitslice $OUT/$br/$bsopt
        make clean
    done
done


