#!/bin/bash
# Управление дисковым пространством

# Сценарий должен отыскать в домашнем каталоге пользователя /home/username файлы, имеющие размер больше 100K. Каждый раз предоставляя пользователю возможность удалить или сжать этот файл, затем переходить к поиску следующего файла.

E_BADARGS=65

del_or_zip()
{
kol=$#
echo "кол-во $kol"

index=1
while [ "$index" -le "$kol" ]
do
  echo "хотите удалить или заархивировать `basename $1` (d-удалить, z-заархивировать, n-пропустить"
  read key
  case $key in
      [dD])
       rm $1
          ;;
      [zZ])
       tar -cvvzf ${1%%.*}.tar.gz $1
      ;;*) :
      esac
  shift
let "index=$index+1"
done


return 0
}

directory=${1:-$HOME}

del_or_zip `find $directory -type f -size +1k`

exit 0