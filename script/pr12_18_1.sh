#!/bin/bash

# crypto-quote.sh: Ограниченное шифрование

key=йфяюжхцычздбсвуьлщмакепитошргнъэё
key1=абвгдеёжзийклмнопрстуфхцчшщъыьэюя

# Для дешифрации можно использовать следующую комбинацию:
cat "$@" | tr "$key" "$key1"