#!/bin/bash

# Лототрон
# Сценарий должен имитировать работу лототрона -- извлекать 5 случайных неповторяющихся чисел в диапазоне 1 - 50. Сценарий должен предусматривать как вывод на stdout, так и запись чисел в файл, кроме того, вместе с числами должны выводиться дата и время генерации данного набора.

DIAP=50
MAXCOUNT=8

count=0

while [ "$count" -le "$MAXCOUNT" ]
  do
  let " count1=$count-1"

  let "val[$count]=$RANDOM%DIAP"

  while [ "$count1" -ge 0 ]
    do
    if [ ${val[count]} -eq ${val[count1]} ]; then
	let "val[$count]=$RANDOM%DIAP"
	let "count1=$count-1"
	#echo "val$count= ${val[$count]}"
	echo "+"
    fi
    let "count1=$count1-1"
  done

#  echo "val$count= ${val[$count]}"
  let "count=$count+1"
done

data_vr=`date +%T"  "%d.%m.%Y`

echo "$data_vr"
echo "${val[@]} "

echo "хотите записать в файл?(y/n)"
read key

if [ "$key" = "Y" -o "$key" = "y" ]; then
    echo "введите имя файла"
    read file
    echo "$data_vr">>$file
    echo "${val[@]}">>$file
fi

exit 0