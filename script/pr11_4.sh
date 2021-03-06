#!/bin/bash

echo

echo "Введите строку завершающуюся символом \\, и нажмите ENTER"
echo "затем введите вторую строку, и снова нажмите Enter."
read var1 # При чтении, символ "\" экранирует перевод строки.
              #     первая строка \
              #     вторая строка

echo "var1=$var1"
#     var1 = первая строка вторая строка

# После ввода каждой строки, завершающейся символом "\",
# вы можете продолжать ввод на другой строке.

echo; echo

echo "Введите другую строку, завершающуюся символом \\, и нажмите ENTER."
read -r var2  # Ключ -r заставляет команду "read" воспринимать "\"
              # как обычный символ. первая строка \


echo "var2=$var2"
# var2 = первая строка \
# Ввод данных прекращается сразу же после первого нажатия на клавишу ENTER.

echo 

# Чтение данных, не дожидаясь нажатия на клавишу ENTER.
read -s -n1 -p "нажмите клывишу " keypress
echo; echo "была нажата клавиша \"$keypress\""
# -s   -- подавляет эхо-вывод, т.е. ввод с клавиатуры не отображается на экране.
# -n N -- ввод завершается автоматически, сразу же после ввода N-го символа.
# -p   -- задает вид строки подсказки - приглашения к вводу (prompt).

exit 0