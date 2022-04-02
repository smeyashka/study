#!/bin/bash 
if [ -n $string ]; then echo "строка \"$string1\" не пустая"; else echo "строка \"$string1\" пустая"; fi
#строка "" не пустая

if [ -n "$string" ]; then echo "строка \"$string1\" не пустая"; else echo "строка \"$string1\" пустая"; fi
#строка "" пустая

if [ $string ]; then echo "строка \"$string1\" не пустая"; else echo "строка \"$string1\" пустая"; fi
#строка "" пустая

string1=initialized

if [ $string1 ]; then echo "строка \"$string1\" не пустая"; else echo "строка \"$string1\" пустая"; fi
#строка "initialized" не пустая

 if [ $string1 ]; then echo "строка \"$string1\" не пустая"; else echo "строка \"$string1\" пустая"; fi
#строка "a = b" пустая

exit 0


