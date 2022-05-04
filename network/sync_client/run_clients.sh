#!/bin/bash

cnt=${1:-1000}

echo `pwd`

rm -f ./build/clients.log

for a in `seq $cnt`; do
    ./build/crazy_client -n "cl_"$a  >> ./build/clients.log 2>&1 &
done

echo "Log will be written in ./build/clients.log"

exit 0
