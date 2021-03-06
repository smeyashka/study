#!/bin/bash

# Сценарий выводит себя сам на stdout дважды, нумеруя строки сценария.

# 'nl' вставит для этой строки номер 3, поскольку она не нумерует пустые строки.
# 'cat -n' вставит для этой строки номер 5.

nl `basename $0`
echo; echo # А теперь попробуем вывести текст сценария с помощью 'cat -n'

cat -n `basename $0`
# Различия состоят в том, что 'cat -n' нумерует все строки.
# Обратите внимание: 'nl -ba' -- сделает то же самое.

exit 0