#!/bin/bash
#Команда "continue N" передает управление в начало внешнего цикла, отстоящего от текущего на N уровней.

for outer in I II III IV V #внешний цикл
do
  echo ; echo -n "Группа $outer: "

  for inner in 1 2 3 4 5 6 7 8 9 10 #вложенный цикл
    do
    if [ "$inner" -eq 7 ]
	then
	continue 2 # Передача управления в начало цикла 2-го уровня.
                   # попробуйте убрать параметр 2 команды "continue"
    fi
    echo -n "$inner " # 7 8 9 10 никогда не будут напечатаны
  done
done

echo; echo
exit 0