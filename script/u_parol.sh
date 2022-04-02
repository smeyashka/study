#!/bin/bash

# Пароли

# Сгенерируйте псевдослучайные 8-ми символьные пароли, используя символы из диапазона [0-9], [A-Z], [a-z]. Каждый пароль должен содержать не менее 2-х цифр.

MATRIX="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
LENGTH="8"
MATRIX_CIFR="0123456789"

pos1=$(( $RANDOM % 8 ))
pos2=$(($RANDOM%8))
if [ $pos1 -eq $pos2 ]; then
    pos2=$(($RANDOM%8))
fi

#формир-е пароля
while [ "${n:=1}" -le "$LENGTH" ]
  do
  if [ $n -eq $pos1 -o $n -eq $pos2 ]; then
      par="$par${MATRIX:$(($RANDOM%${#MATRIX_CIFR})):1}"  
  else
      par="$par${MATRIX:$(($RANDOM%${#MATRIX})):1}"  
  fi
  let "n+=1"
done

#проверка наличия двух цифр
par_tmp=${par//[0-9]/}
length_tmp=${#par_tmp}

if [ $length_tmp -gt 6 ]; then
    echo "чего-то плохо сработал скрипт.. может попробуете еще.. =)"
    echo $par
else
    echo $par
fi


exit 0
