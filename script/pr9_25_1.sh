#!/bin/bash

RANDOM=$$

STOR=2
MAXTHROWS=600
throw=0

orel=0
reshka=0

print_result()
{
echo
echo "орел=$orel"
echo "решка=$reshka"
echo
}

while [ "$throw" -lt "$MAXTHROWS" ]
do
  let "die1=RANDOM % $STOR"
  if [ $die1 -eq 1 ]
      then
      let "reshka+=1"
  else
      let "orel+=1"
  fi
  let "throw+=1"
done

print_result

exit 0