#!/bin/bash
# random2.sh: Генерация псевдослучайных чисел в диапазоне 0 - 1.
# Используется функция rand() из awk.

#AWKSCRIPT=' { srand('$a'); print rand() } '
echo -n "Случайное число в диапазоне от 0 до 1="
for ((a=0; a<10; a++))
do
#echo | awk "$AWKSCRIPT"
echo | awk '{srand('$a'); print rand() }'
done

exit 0 