#!/bin/bash
# isalpha.sh: Использование "case" для анализа строк.

SUCCESS=0
FAILURE=-1

isalpha() #проверка - является ли первый символ строки символом алфавита
{
if [ -z "$1" ] # Вызов функции без входного аргумента?
then 
return $FAILURE
fi

case "$1" in
    [a-zA-Z]* ) return $SUCCESS;; #первый символ - буква?
    *         ) return $FAILURE;; 
esac
}

isalpha2() # Проверка - состоит ли вся строка только из символов алфавита.
{
[ $# -eq 1 ] || return $FAILURE

case $1 in
    *[!a-zA-Z]*|"") return $FAILURE;;
    *             ) return $SUCCESS;;
esac
}

isdigit()# Проверка - состоит ли вся строка только из цифр.
{ # Другими словами - является ли строка целым числом.
[ $# -eq 1 ] || return $FAILURE
case $1 in
    *[!0-9]*|"") return $FAILURE;;
    *          ) return $SUCCESS;;
esac
}

isfloat() #проверка - явл-ся ли строка вещественным числом
{
    [ $# -eq 1 ] || return $FAILURE
    case $1 in
	*[!0-9.]*|"") return $FAILURE;;
	*            ) return $SUCCESS;;
    esac
}

check_var() #Интерфейс к isalpha
{
if isalpha "$@"
    then
    echo "\"$*\" начинается с алфавитного символа"
    if isalpha2 "$@"
	then # Дальнейшая проверка не имеет смысла, если первй символ не буква.
	echo "\"$*\" содержит только алфавитные символы"
    else
	echo "\"$*\" содержит по меньшей мере один неалфавитный символ"
    fi
else
    echo "\"$*\" начинается с не алфавитного символа"
 #  Если функция вызвана без входного параметра,
 #+ то считается, что строка содержит "не алфавитной" символ.
fi
    
echo
}

digit_chek() # Интерфейс к isdigit ().
{
if isdigit "$@"
then 
    echo "\"$*\" содержит только цифры [0-9]"
else
    echo "\"$*\" содержит по меньшей мере один не цифровой символ"
fi
echo
}

float_chek() # интерфейс к isfloat()
{
    if isdigit "$@"
	then 
	echo "\"$*\" содержит целое число"
    else
	if isfloat "$@" 
	    then
	    echo "\"$*\" содержит вещественное число"
	else 
	    echo "\"$*\" содержит по меньшей мере один не цифровой символ"
	fi
    fi
}

a=23skidoo
b=H3ll0
c=-What?
d=What?
e=`echo $b`  # Подстановка команды.
f=AbcDef
g=27234
h=27a34
i=27.34

check_var $a
check_var $b
check_var $c
check_var $d
check_var $e
check_var $f
check_var 

# digit_chek $g
# digit_chek $h
# digit_chek $i
digit_chek
float_chek $i
float_chek $h
float_chek $g

exit 0