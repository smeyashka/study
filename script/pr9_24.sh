#!/bin/bash
#pick-card.sh

#выбор случайной карты из колоды (случ. эл-т из массива)

Suites="Треф
Бубей
Червей
Пик"

Denominations="2
3
4
5
6
7
8
9
10
Валет
Дама
Король
Туз"

suite=($Suites) #иниц-я массива
denomination=($Denominations)

num_suites=${#suite[*]} #кол-во элтов
num_denomination=${#denomination[*]}

echo -n "${denomination[$((RANDOM%num_denomination))]} "
echo ${suite[$((RANDOM%num_suites))]}

exit 0