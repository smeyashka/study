#!/bin/bash

# Счастливый билет

# "Счастливым" считается такой билет, в котором последовательное сложение цифр номера дает число 7. Например, 62431 -- номер "счастливого" билета (6 + 2 + 4 + 3 + 1 = 16, 1 + 6 = 7). Найдите все "счастливые" номера, располагающиеся в диапазоне 1000 - 10000.

E_BADARGS=65

if [ $# -ne 1 ]; then
	echo "порядок использования: `basename $0` <число>"
	exit $E_BADARGS=65
fi

num=$1
sum=$num

while [ ${#sum} -gt 1 ]; do
    sum=0
    index=0
    count=${#num}

    while [ $index -lt $count ]; do
	cifra=${num:$index:1}
	let "sum=$sum+$cifra"
	let "index=$index+1"
    done

    num=$sum  
done

echo "сумма цифр равна $sum"

exit 0