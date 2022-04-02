#!/bin/bash

MAX=10000

for ((nr=1; nr<$MAX; nr++))
do

let "t1=nr % 5"
if [ "$t1" -ne 3 ]; then
    continue
fi

let "t2=nr % 7"
if [ "$t2" -ne 4 ]; then
    continue
fi

let "t3= nr % 9"
if [ "$t3" -ne 5 ]; then
    continue
fi

break

done

echo "число = $nr"
echo $t1 $t2 $t3


exit 0