#!/bin/bash

# arrow-detect.sh: Обнаружение нажатия на курсорные клавиши, и не только...
# Спасибо Sandro Magi за то что показал мне -- как.

# --------------------------------------------
# Коды клавиш.
arrowup='\[A'
arrowdown='\[B'
arrowrt='\[C'
arrowleft='\[D'
insert='\[2'
delete='\[3'
# --------------------------------------------

SUCCESS=0
OTHER=65

echo -n "нажмите а клавишу.."
# Может потребоваться нажать на ENTER, если была нажата клавиша
# не входящая в список выше.
read -n3 key  # Прочитать 3 символа.

echo -n "$key" | grep "$arrowup"  #Определение нажатой клавиши.
if [ "$?" -eq $SUCCESS ]
then
  echo "Нажата клавиша \"."
  exit $SUCCESS
fi

echo -n "$key" | grep "$arrowdown"
if [ "$?" -eq $SUCCESS ]
then
    echo "Нажата клавиша \" "
    exit $SUCCESS
fi

echo -n "$key" | grep "$arrowrt"
if [ "$?" -eq $SUCCESS ]
then
echo "Нажата клавиша \"О\"."
exit $SUCCESS
fi

echo -n "$key" | grep "$arrowleft"
if [ "$?" -eq $SUCCESS ]
then
  echo "Нажата клавиша \"."
  exit $SUCCESS
fi

echo -n "$key" | grep "$insert"
if [ "$?" -eq $SUCCESS ]
then
  echo "Нажата клавиша \"Insert\"."
  exit $SUCCESS
fi

echo -n "$key" | grep "$delete"
if [ "$?" -eq $SUCCESS ]
then
  echo "Нажата клавиша \"Delete\"."
  exit $SUCCESS
fi

echo " Нажата какая-то другая клавиша."

exit $OTHER