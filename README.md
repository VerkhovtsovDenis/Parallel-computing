# Установка и запуск

1. Скачать и установить msys64 (https://www.msys2.org/)
2. На ucrt64 установить C++ и make:
2.1. pacman -S mingw-w64-ucrt-x86_64-gcc (https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-gcc?repo=ucrt64)
2.2. pacman -S mingw-w64-ucrt-x86_64-make (https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-make)
3. Запустить ucrt64 и перейти в папку с проектом (с помощью команды cd)
4. Выполнить проект командой mingw32-make