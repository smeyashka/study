#!/bin/bash

ARGS=1
E_BADARGS=65
E_NOFILE=66

if [ $# -ne "$ARGS" ]; then
    echo "порядок использования : `basename $0` filename"
    exit $E_BADARGS
fi

if [ ! -f "$1" ]; then
    echo "файл \"$1\" не существует"
    exit $E_NOFILE
fi

cat $1

tr -c "A31" + <$1