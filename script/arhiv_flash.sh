#!/bin/bash

E_NODEVICE=65
E_NOFSTAB=66

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


if [ $# -eq 0 ]; then
    file=cc
    cd ~/progi
else
    file=$1
fi

rm -f ${file}_old.tar.gz
if [ -e ${file}.tar.gz ]; then
#find $(file) -mtime -1 > spisok.tmp
# перву строку пропустить, while read line; do tar ;done < spis.tmp
    mv ${file}.tar.gz ${file}_old.tar.gz    
    tar -cvvzf ${file}.tar.gz $file
else 
    tar -cvvzf ${file}.tar.gz $file
fi

dev=`mount_dev flash`

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
	cp ${file}.tar.gz /mnt/flash/
	echo "файл записан"
	    echo "хотите отмонтировать устройство?(y/n)"
	    read answer
	    if [ $answer = "Y" -o $answer = "y" ]; then
		echo "пожалуйста, подождите..."
		umount $dev
		echo "устройство отмонтировано"
	    fi    
	    
	    ;;
esac

exit 0
