#!/bin/bash
#сравнение строк

match_string()
{
MATCH=0
NOMATCH=90
PARAMS=2 # функция требует 2 вх. параметра
BAD_PARAMS=91

[ $# -eq $PARAMS ] || return $BAD_PARAMS
case "$1" in
    "$2") return $MATCH;;
    *   ) return $NOMATCH;;
esac
}

a=one
b=two
c=three
d=two

match_string $a # неверное число аргументов
echo $? # 91

match_string $a $b # не равны
echo $? # 90

match_string $b $d # равны
echo $? # 0

exit 0