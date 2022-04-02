#!/bin/bash

echo

echo "Проверяется \"0\""
if [ 0 ] #ноль
then
    echo "0 - это истина."
else
    echo "0 - это ложь"
fi

echo

echo "Прооверяется \"1\""
if [ 1 ] #единица
then 
    echo "1- это истина"
else
    echo "1- это ложь"
fi

echo

echo "Проверяется \"-1\""
if [ -1 ] # минус один
then
    echo "-1 - это истина"
else
    echo "-1 - это ложь"
fi

echo

echo "Проверяется \"NULL\""
if [ ]
then
    echo "NULL - это истина"
else
    echo "NULL - это ложь"
fi

echo

echo "Проверяется \"xyz\""
if [ xyz ] 
then
    echo "Случайная строка - это истина"
else
    echo "Случайная строка - это ложь"
fi

echo

echo "Проверяется \"\$xyz\""
if [ $xyz ] 
then
    echo "Неинициализированная переменная - это истина"
else
    echo "Неинициализированная переменная - это ложь"
fi

echo

echo "Проверяется \"-n \$xyz\""
if [ -n "$xyz" ]
then
    echo "Неинициализ-я переменная - это истина"
else
    echo "Неиниц-я переменная - это ложь"
fi

echo

xyz=

echo "Проверяется \"-n \$xyz\""
if [ -n "$xyz" ] 
then
    echo "Пустая переменная - это истина"
else
    echo "Пустая перем-я - это ложь"
fi

echo

echo "Проверяется \"false\""
if [ "false" ] # минус один
then
    echo "\"false\" - это истина"
else
    echo "\"false\" - это ложь"
fi

echo

echo "Проверяется \"\false\""
if [ "$false" ]
then
    echo "\"\$false\" - это истина"
else
    echo "\"\$false\" - это ложь"
fi

echo

exit 0