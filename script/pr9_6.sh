#!/bin/bash
#вызвать сценарий с несколькими аргументами, напр., "один, два, три"

E_BADARGS=65

if [ ! -n "$1" ]
then
    echo "порядок исп-я:`basename $0` arg1 arg2 arg3"
    exit $E_BADARGS
fi

echo

index=1

echo "список аргументов в переменной \"\$*\":"
for arg in "$*"
do
  echo "аргумент #$index = $arg"
  let "index+=1"
done
echo "полн. список арг-ов выглядит как одна строка"

echo

index=1

echo "список арг-тов в переменной \"\$@\":"
for arg in "$@"
do
  echo "арг-т #$index = $arg"
  let "index+=1"
done
echo "список арг-ов выглядит как набор разл-х строк(слов)"

echo

exit 0