#!/bin/bash

# Изменение межстрочного интервала в текстовом файле

# Напишите сценарий, который будет читать текст из заданного файла, и выводить, построчно, на stdout, добавляя при этом дополнительные пустые строки так, чтобы в результате получился вывод с двойным межстрочным интервалом.

# Добавьте код, который будет выполнять проверку наличия файла, передаваемого как аргумент.

# Когда сценарий будет отлажен, измените его так, чтобы он выводил текстовый файл с тройным межстрочным интервалом.

# И наконец, напишите сценарий, который будет удалять пустые строки из заданного файла.

E_NOARGS=65
E_BADARGS=66

ARGS=1

if [ $# -ne $ARGS ]; then
    echo "порядок использования: `basename $0` file_name" >&2
    echo >&2
    exit $E_NOARGS
fi

if [ ! -e $1 -o ! -f $1 -o ! -r $1 ]; then
    echo "файл либо не существует, либо не является обычным, либо у вас нет прав на чтение этого файла" >&2
    echo >&2
    exit $E_BADARGS
    
fi

file=$1

#sed G filename
while read line
do
  echo $line
  echo
#  echo
done < $file > new$file

cat new$file

echo "двойной интервал"
echo "для продолжения нажмите return   "
read

sed -e '/^$/d' new$file
echo
echo "удалены пустые строки"
echo "для продолжения нажмите return"
read

rm new$file

exit 0