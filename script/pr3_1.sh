#!/bin/bash
#чтение строк из файла /etc/fstab

File=/etc/fstab
{
read line1
read line2
} < $File
echo "первая строка $File"
echo "$line1"
echo "вторая строка $File"
echo "$line2"
exit 0


