#!/bin/bash
#имитируем бросание 2-х кубиков

SPOTS=7
ZERO=0
die1=0
die2=0

while [ "$die1" -eq $ZERO ]
do
  let "die1=$RANDOM % $SPOTS"
done

while [ "$die2" -eq $ZERO ]
do
  let "die2=$RANDOM % $SPOTS"
done

let "throw=$die1+$die2"
echo "1-й : $die1"
echo "2-й : $die2"
echo "Рез-т броска кубиков=$throw"
echo

exit 0