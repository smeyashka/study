#!/bin/bash

# string.bash --- эмуляция библиотеки функций string(3)
# Автор: Noah Friedman <friedman@prep.ai.mit.edu>
# ==>     Используется с его разрешения.
# Дата создания: 1992-07-01
# Дата последней модификации: 1993-09-29
# Public domain

# Преобразование в синтаксис bash v2 выполнил Chet Ramey

# Комментарий:
# Код:

#:docstring strcat:
# Порядок использования: strcat s1 s2
#
# Strcat добавляет содержимое переменной s2 к переменной s1.
#
# Пример:
#    a="foo"
#    b="bar"
#    strcat a b
#    echo $a
#    => foobar
#
#:end docstring:

###;;;autoload

function strcat()
{
local s1_val s2_val

s1_val=${!1} # косвенная ссылка
s2_val=${!2}
eval "$1"=\'"${s1_val}${s2_val}"\'
# ==> eval $1='${s1_val}${s2_val}' во избежание проблем,
# ==> если одна из переменных содержит одиночную кавычку.
}

#:docstring strncat:
# Порядок использования: strncat s1 s2 $n
#
# Аналог strcat, но добавляет не более n символов из
# переменной s2. Результат выводится на stdout.
#
# Пример:
#    a=foo
#    b=barbaz
#    strncat a b 3
#    echo $a
#    => foobar
#
#:end docstring:

###;;;autoload

function strncat()
{
local s1="$1"
local s2="$2"
local -i n="$3"
local s1_val s2_val

s1_val=${!s1}  # ==> косвенная ссылка
s2_val=${!s2}

if [ ${s2_val} -gt ${n} ]; then
    s2_val=${s2_val:0:$n}   # ==> выделение подстроки
fi

eval "$s1"=\'"${s1_val}${s2_val}"\'
# ==> eval $1='${s1_val}${s2_val}' во избежание проблем,
# ==> если одна из переменных содержит одиночную кавычку.

}

#:docstring strcmp:
# Порядок использования: strcmp $s1 $s2
#
# Strcmp сравнивает две строки и возвращает число меньше, равно
# или больше нуля, в зависимости от результатов сравнения.
#:end docstring:

###;;;autoload
function strcmp()
{
echo "$1 $2"

[ "${!1}" = "${!2}" ] && { echo w; return 0; }

[ "${!1}" '<' "${!2}" ] > /dev/null && return -1

return 1
}

#:docstring strncmp:
# Порядок использования: strncmp $s1 $s2 $n
#
# Подобна strcmp, но сравнивает не более n символов
#:end docstring:

###;;;autoload
function strncmp()
{
if [ -z "$3" -o "$3" -le "0" ]; then
    return 0
fi

if [ ${3} -ge ${#1} -a ${3} -ge ${#2} ]; then
    echo "$1 $2"
    strcmp "${!1}" "${!2}"
    return $?
else
    echo "${!1} ${!2}"
    s1=${!1:0:$3}
    s2=${!2:0:$3}
    echo  $s1 $s2
    strcmp s1 s2
    return $?
fi
}

#:docstring strlen:
# Порядок использования: strlen s
#
# возвращает количество символов в строке s.
#:end docstring:

###;;;autoload
function strlen()
{
eval echo "\${#${1}}"
# ==> Возвращает длину переменной,
# ==> чье имя передается как аргумент.
}

#:docstring strspn:
# Порядок использования: strspn $s1 $s2
#
# Strspn возвращает максимальную длину сегмента в строке s1,
# который полностью состоит из символов строки s2.
#:end docstring:

###;;;autoload
function strspn()
{
# Сброс содержимого переменной IFS позволяет обрабатывать пробелы как обычные символы.
local IFS=
local result="${1%%[!$2]}"
echo ${#result}
}

#:docstring strcspn:
# Порядок использования: strcspn $s1 $s2
#
# Strcspn возвращает максимальную длину сегмента в строке s1,
# который полностью не содержит символы из строки s2.
#:end docstring:

###;;;autoload
function strcspn()
{
# Сброс содержимого переменной IFS позволяет обрабатывать пробелы как обычные символы.
local IFS=
local result="${1%%[$2]*}"

echo ${#result}
}

#:docstring strstr:
# Порядок использования: strstr s1 s2
# Strstr выводит подстроку первого вхождения строки s2
# в строке s1, или ничего не выводит, если подстрока s2 в строке s1 не найдена.
# Если s2 содержит строку нулевой длины, то strstr выводит строку s1.
#:end docstring:

###;;;autoload
function strstr()
{
# Если s2 -- строка нулевой длины, то вывести строку s1
[ ${#2} -eq 0 ] && { echo "$1"; return 0; }

# не выводить ничего, если s2 не найдена в s1
case "$1" in
*$2*) ;;
*) return 1;;
esac

# использовать шаблон, для удаления всех несоответствий после s2 в s1
first=${1/$2*/}
# Затем удалить все несоответствия с начала строки
echo "${1##$first}"

}

#:docstring strtok:
# Порядок использования: strtok s1 s2
#
# Strtok рассматривает строку s1, как последовательность из 0, или более,
# лексем (токенов), разделенных символами строки s2
# При первом вызове (с непустым аргументом s1)
# выводит первую лексему на stdout.
# Функция запоминает свое положение в строке s1 от вызова к вызову,
# так что последующие вызовы должны производиться с пустым первым аргументом,
# чтобы продолжить выделение лексем из строки s1.
# После вывода последней лексемы, все последующие вызовы будут выводить на stdout
# пустое значение. Строка-разделитель может изменяться от вызова к вызову.
#:end docstring:
###;;;autoload
function strtok ()
{
 :
}

#:docstring strtrunc:
# Порядок использования: strtrunc $n $s1 {$s2} {$...}
#
# Используется многими функциями, такими как strncmp, чтобы отсечь "лишние" символы.
# Выводит первые n символов в каждой из строк s1 s2 ... на stdout.
#:end docstring:
###;;;autoload
function strtrunc()
{
n=$1; shift
for z; do
    echo "${z:0:$n}"
done
}
# provide string

# string.bash конец библиотеки


# ========================================================================== #
# ==> Все, что находится ниже, добавлено автором документа.

# ==> Чтобы этот сценарий можно было использовать как "библиотеку", необходимо
# ==> удалить все, что находится ниже и "source" этот файл в вашем сценарии.

#strcat
string0=one
string1=two
echo
echo "Проверка функции \"strcat\" :"
echo "Изначально \"string0\" = $string0"
echo "\"string1\" = $string1"
strcat string0 string1
echo "Теперь \"string0\" = $string0"
echo

# strlen
echo
echo "Проверка функции  \"strlen\" :"
str=123456789
echo "\"str\" = $str"
echo -n "Длина строки \"str\" = "
strlen str
echo

#strcmp
string0=oneo
string1=onew
echo
echo "Проверка функции \"strcmp\" :"
echo "\"string0\" = $string0"
echo "\"string1\" = $string1"
strcmp string0 string1
res=$?
if [ $res -eq 0 ]; then
    echo "равны"
elif [ $res -eq 1 ]; then
    echo  $res
    echo "$string0 > $string1"
else
    echo "$string0 < $string1"
fi

echo

#strncmp
string0=oneo
string1=onew
echo
echo "Проверка функции \"strncmp\" :"
echo "\"string0\" = $string0"
echo "\"string1\" = $string1"
echo "n=2"
strncmp string0 string1 4
res=$?
if [ $res -eq 0 ]; then
    echo "равны"
elif [ $res -eq 1 ]; then
    echo "$string0 > $string1"
else
    echo "$string0 < $string1"
fi
echo

#strspn
string0="wstw one wr"
string1=one
echo
echo "Проверка функции \"strspn\" :"
echo "\"string0\" = $string0"
echo "\"string1\" = $string1"
strcspn "$string0" $string1


#strstr
string0="weroneeron"
string1="one"
echo
echo "Проверка функции \"strtrunc\" :"
echo "\"string0\" = $string0"
echo "\"string1\" = $string1"
strstr $string0 $string1

#strtrunc
string0="wstw"
string1="werwr"
echo
echo "Проверка функции \"strtrunc\" :"
echo "\"string0\" = $string0"
echo "\"string1\" = $string1"
strtrunc 3 $string0 $string1


exit 0
