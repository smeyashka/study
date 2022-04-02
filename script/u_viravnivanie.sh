#!/bin/bash

# Выравнивание

# Текст вводится с устройства stdin или из файла. Его необходимо вывести на stdout, с выравниванием по ширине, используя задаваемую пользователем ширину строк.

echo "введите ширину"
read width_text

if [ $# -ge 1 ]; then
    cat "$1" | fold -s --width=$width_text
else
    echo "введите текст"
    read text
    echo
    echo "$text" | fold -s --width=$width_text
fi

exit 0