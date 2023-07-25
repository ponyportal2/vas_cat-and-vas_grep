# Vas Grep, Vas Cat

Here are the alternatives I wrote from scratch to the original system utilities cat and grep.

The code in this project demonstrates my programming skills at the beginning of my journey in the C language :) , so the code may be a bit brute-force. However, the functionality works as intended and has passed an auto-check at Sber's School of Programming 21 (School 42 alternative), auto-check (as well as my own tests) also included valgrind test, and no leaks were discovered.

To test and run:

Navigate to the grep or cat folder in the terminal, run make to build the vas_cat or vas_grep file. They work identically to similar system utilities on Unix systems. For example, run:

```
./vas_cat Makefile
```
or
```
./vas_grep int vas_grep.c
```
The library has been tested and works on Linux, Mac, and WSL.

Tests were also written by me, and support quiet mode and changing parameters for custom testing. To test with custom tests that check the output equality with the original system functions, run "make test".

-----------------------------------------

Здесь написанные мной с нуля альтернативы оригинальным системным утилитам cat и grep.

Код в этом проекте показывает мои навыки программирования на начале пути в языке С :) , поэтому здесь кошмарный брутфорсный код. Однако функционал работает как надо и прошёл авто-проверку в Школе Программирования 21 от Сбера, авто-проверка (как и мои личные тесты) включала в себя тест на valgrind и ликов не было обнаружено.

Для тестирования и запуска:

Заходим в терминале в папку src и потом в grep или cat соответственно, пишем make чтобы забилдился файл vas_cat или vas_grep, они работают идентично подобным системным утилитам на unix системах, например пишем 
```
./vas_cat Makefile
```
или
```
./vas_grep int vas_grep.c
```
Библиотека протестирована и работает на Linux, Mac и WSL.

Также мной были написаны тесты, они поддерживают тихий режим, а также изменение параметров для настраиваемого тестирования. Для тестирования самописными тестами которые проверяют идентичность вывода с оригинальными функциями в системе вводим make test.

