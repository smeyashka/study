#!/bin/sh
grep $1 /etc/passwd > /dev/null
if [ $? -ne 0 ]
then 
echo пользователь $1 не зарегестрирован
fi