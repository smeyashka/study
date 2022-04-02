#!/bin/bash
## script-detector.sh: Отыскивает файлы сценариев в каталоге.

TESTCHARS=2    # Проверяются первые два символа.
SHABANG='#!' # Сценарии как правило начинаются с "sha-bang."

for file in * # Обход всех файлов в каталоге.
do
  if [[ `head -c$TESTCHARS "$file"` = "$SHABANG" ]]
  #      head -c2                      #!
  #  Ключ '-c' в команде "head" выводит заданное
  #+ количество символов, а не строк.
      then 
      echo "файл \"$file\" - сценарий"
  else 
      echo "файл \"$file\" - не является сценарием"
  fi
done

exit 0