#!/bin/bash
#им-ия бросания кубика

RANDOM=$$

PIPS=6
MAXTHROWS=600
throw=0

zeroes=0
ones=0
twos=0
threes=0
fours=0
fives=0
sixes=0

print_result()
{
echo
echo "единиц=$ones"
echo "двоек=$twos"
echo "троек=$threes"
echo "четверок=$fours"
echo "пятерок=$fives"
echo "шестерок=$sixes"
echo
}

update_count()
{
case "$1" in
    0) let "ones+=1";;
    1) let "twos+=1";;
    2) let "threes+=1";;
    3) let "fours+=1";;
    4) let "fives+=1";;
    5) let "sixes+=1";;
esac
}

echo

while [ "$throw" -lt "$MAXTHROWS" ]
do
  let "die1=RANDOM % $PIPS"
  update_count $die1
  let "throw+=1"
done

print_result

exit 0