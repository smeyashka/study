#!/bin/bash
#seeding-random.sh переустановка переменной RANDOM

MAXCOUNT=25 # Длина генерируемой последовательности чисел.

random_numbers()
{
count=0
while [ "$count" -lt "$MAXCOUNT" ]
do
  number=$RANDOM
  echo -n "$number "
  let "count+=1"
done
}

echo; echo

RANDOM=1
random_numbers

echo; echo

RANDOM=1
random_numbers  
echo; echo

RANDOM=2
random_numbers

echo; echo

#RANDOM=$$  в качестве начального числа выбирается PID процесса-сценария.
# Вполне допустимо взять в качестве начального числа результат работы команд 'time' или 'date'.

SEED=$(head -1 /dev/urandom | od -N 1 | awk '{ print $2 }')
RANDOM=$SEED
random_numbers

echo; echo

exit 0