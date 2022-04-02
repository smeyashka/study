#!/bin/bash

E_NODEVICE=65
E_NOFSTAB=66

per_flash="flash"

mount_dev()
{
    local a=`sed -n "/$1/p" /etc/mtab | awk '{ print $2 }'`
    if [ -z "$a" ]; then
	local b=`grep "\<$1\>" /etc/fstab | awk '{ print $2 }'`
	if [ -z "$b" ]; then
	    echo $E_NOFSTAB
	else 
	    local c=`mount $b 2>&1`
	    if [ -n "$c" ]; then
		echo $E_NODEVICE
	    else 
		echo $b
	    fi
	fi
    else
	echo $a
    fi
}

cd
file=${1:-~/tmp/spisok_dom_kat.txt}

echo "содержимое домашнего каталога будет записано в ${file%%.*}.tar.gz"
find . -type f > $file
cd tmp/
file=`basename $file`
tar -cvvzf ${file%%.*}.tar.gz $file
rm $file

echo -n "хотите записать?(флэш - f, дискета - d, нет - n) "
read answer
case "$answer" in
    [fFdD]) 
    if [ $answer = "f" -o $answer = "F" ]; then
	dev=`mount_dev flash`
    else
	dev=`mount_dev floppy`
    fi

    case "$dev" in
	$E_NODEVICE)
	    echo "невозможно примонтировать устройство $b. Проверьте настройки /etc/fstab"
	    echo "файл не записан"
	    exit 0
	    ;;
	$E_NOFSTAB)
	    echo "устройство $b не найдено в /etc/fstab. Обратитесь к администратору"
	    echo "файл не записан"
	    exit 0
	    ;;
	*)
	    cp ${file%%.*}.tar.gz $dev
	    echo "файл записан"
	    echo "хотите отмонтировать устройство?(y/n)"
	    read answer
	    if [ $answer = "Y" -o $answer = "y" ]; then
		umount $dev
		echo "устройство отмонтировано"
	    fi    
	    echo "хотите удалить с жесткого диска файл"  ~/tmp/${file%%.*}.tar.gz "(y/n)"
	    read answer
	    if [ $answer = "Y" -o $answer = "y" ]; then
		rm ${file%%.*}.tar.gz
		echo "файл удален"
	    fi 
	    ;;
    esac    
    ;;

    *) 
    echo "файл не записан"  
    ;;
esac 


exit 0