#!/bin/bash
# Список планет.
#Замена цикла for циклами while и until

for planet in Меркурий Венера Земля Марс Юпитер Сатурн Уран Нептун Плутон
do
  echo $planet
done

echo

# Если 'список аргументов' заключить в кавычки, то он будет восприниматься как единственный аргумент .
for planet in "Меркурий Венера Земля Марс Юпитер Сатурн Уран Нептун Плутон"
do
  echo $planet
done

spis_plan=( Меркурий Венера Земля Марс Юпитер Сатурн Уран Нептун Плутон )

index=0
col_elem=${#spis_plan}

echo
echo "цикл while"
while [ "$index" -le "$col_elem" ]; do
    echo ${spis_plan[$index]}
    let "index=$index+1"
done

echo
echo "цикл until"
index=0
until [ "$index" -gt "$col_elem" ]; do
    echo ${spis_plan[$index]}
    let "index=$index+1"
done

exit 0