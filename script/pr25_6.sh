#!/bin/bash
# bubble.sh: "Пузырьковая" сортировка.

#  На каждом проходе по сортируемому массиву,
#+ сравниваются два смежных элемента, и, если необходимо, они меняются местами.
#  В конце первого прохода, самый "тяжелый" элемент "опускается" в конец массива.
#  В конце второго прохода, следующий по "тяжести" элемент занимает второе место снизу.
#  И так далее.
#  Каждый последующий проход требует на одно сравнение меньше предыдущего.
#  Поэтому вы должны заметить ускорение работы сценария на последних проходах.


exchange()
{
#поменять местами два элемента массива
local temp=${Countries[$1]} #временная переменная
Countries[$1]=${Countries[$2]}
Countries[$2]=$temp
return
}

declare -a Countries #  Объявление массива,
                      #+ необязательно, поскольку он явно инициализируется ниже.

Countries=(Нидерланды Украина Заир Турция Россия Йемен Сирия \
Бразилия Аргентина Никарагуа Япония Мексика Венесуэла Греция Англия \
Израиль Перу Канада Оман Дания Уэльс Франция Кения \
Занаду Катар Лихтенштейн Венгрия)

# "Занаду" -- это мифическое государство, где, согласно Coleridge,
#+ Kubla Khan построил величественный дворец.

clear #очистка экрана

echo "0: ${Countries[*]}" # Список элементов несортированного массива.

number_of_elements=${#Countries[@]}
let "comparsions=$number_of_elements-1"

count=1 #номер прохода

while [ "$comparsions" -gt 0 ] # Начало внешнего цикла
  do

  index=0 # Сбросить индекс перед началом каждого прохода.

  while [ "$index" -lt "$comparsions" ] #начало внутреннего цикла
    do
    if [ ${Countries[$index]} \> ${Countries[`expr $index+1`]} ]
     #  Если элементы стоят не по порядку...
     #  Оператор \> выполняет сравнение ASCII-строк
     #+ внутри одиночных квадратных скобок.
     #  if [[ ${Countries[$index]} > ${Countries[`expr $index + 1`]} ]]
     #+ дает тот же результат.
	then
	exchange $index `expr $index+1` #Поменять местами.
    fi
    let "index+=1"    
  done

  let "comparsions-=1" #  Поскольку самый "тяжелый" элемент уже "опустился" на дно,
                       #+ то на каждом последующем проходе нужно выполнять на одно сравнение меньше.
  echo
echo "$count: ${Countries[@]}" # Вывести содержимое массива после каждого прохода.
echo
let "count+=1" # Увеличить счетчик проходов.
done # Конец внешнего цикла

exit 0