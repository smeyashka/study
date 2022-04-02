#!/bin/bash

while [ "$Keypress" != "X" ]
do
  echo; echo "нажмите клавишу, а затем return, для выхода \"X\""
  
  read Keypress
  
  case "$Keypress" in
      [a-z] ) echo "буква в нижнем регистре";;
      [A-Z] ) echo "буква в верхнем регистре";;
      [0-9] ) echo "цифра";;
      *     ) echo "знак пунктуации,пробел или что-то еще";;
  esac
done
exit 0