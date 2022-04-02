#!/bin/bash
# wf.sh: "Сырой" анализ частоты встречаемости слова в текстовом файле.


ARGS=1
E_BADARGS=65
E_NOFILE=66

if [ $# -ne "$ARGS" ] # Файл для анализа задан?
then
    echo "порядок исп-ия : `basename $0` filename"
    exit $E_BADARGS
fi

if [ ! -f "$1" ]; then # Проверка существования файла.
    echo "файл \"$1\" не найден"
    exit $E_NOFILE
fi

# main()
sed -e 's/\.//g' -e 's/ //g' "$1" | tr 'A-Z' 'a-z' | sort | uniq -c | sort -nr