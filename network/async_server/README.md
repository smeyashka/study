Асинхронный сервер.
Читает конфиг (json), принимает команды на запрос/изменение параметра, периодически сохраняет в файл, выводит статистику.

Для сборки:

mkdir build
cd build
cmake ..
make

Для запуска с параметрами по умолчанию:
./build/crazy_server

При запуске можно указать (см. ./build/crazy_server -h)
- порт
- путь к конфигурационному файлу
- количество потоков для обработки клиентов

Конфигурационный файл при сборке копируется в './build/config.h'.
Формат json, значения ключей могут быть только целыми или строковыми.
Запросы с несуществующим ключом (в статистике сервера non-existent) будут возвращены как ошибочные.
При попытке записать в параметр с числовым значением строковое, будет возвращено текущее значение параметра.