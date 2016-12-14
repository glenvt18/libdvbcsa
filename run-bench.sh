#!/bin/sh

IN=tests

for file in $(find $IN -type f | LC_ALL=C sort); do
    echo "-- $file"
    echo ======================================================================
    ./$file
    ./$file
done

echo ====================== Done =============================
