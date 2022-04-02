#!/bin/bash
# от 1 до 6 пятью различными способами

n=1; echo -n "$n "

let "n=$n+1"
echo -n "$n "

: $((n=$n+1))
echo -n "$n "

n=$(($n+1))
echo -n "$n "

: $[n=$n+1]
echo -n "$n "

n=$[ $n+1 ]
echo -n "$n "; echo 

exit 0