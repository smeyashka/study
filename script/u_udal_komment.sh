#!/bin/bash

# Удаление комментариев

# Удалите все комментарии из сценария, имя которого задается с командной строки. При этом, строка "#! /bin/bash" не должна удаляться.


E_BADARGS=65

ARGS=1

if [ $# -lt 1 ]; then
    echo "порядок использования: `basename $0` <filename>"
    exit $E_BADARGS
fi

file=$1
file1=`basename ${file%%.*}.komm`
echo "результата будет записан в $file1"
echo "" > $file1

while read line
  do
  if echo "$line" | grep -v \".*# | grep -v \#! > /dev/null
      then
      echo $line | sed -e "s/#.*$//"
  else 
      echo $line
  fi
  
 #  echo "$line" | grep -v \" | grep -v \#! | sed -e "s/#.*$//"
#   echo "$line" | sed -n "/#*\"*/p"
#grep \#!
done < $file  >> $file1

#sed -n /#/p $file | grep -v \" | 

#sed -e "2,\$s/#.*$//" $file

#не учтено, что символ может быть в "" и #!

exit 0