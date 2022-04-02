#!/bin/bash
#length.sh

E_NO_ARGS=65

if [ $# -eq 0 ] ; then
    echo "нужны параметры командной строки"
    exit $E_NO_ARGS
fi

var01=abcdEFG28ij

echo "var01=${var01}"
echo "length of var01=${#var01}"

echo "кол-во вх. параметров=${#@}"
echo "кол-во вх. параметров=${#*}"
echo "кол-во вх. параметров=$#"

exit 0