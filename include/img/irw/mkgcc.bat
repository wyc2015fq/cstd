@echo off
set CC=gcc.exe
set path=E:\soft\MinGW\bin;%path%
echo #define test_imgio main>test_imgio.c
echo #include "test_imgio.inl" >>test_imgio.c
gcc.exe test_imgio.c -o test_imgio.exe -IE:\code\cstd\include -IE:\code\cstd
:: arm-none-linux-gnueabi-gcc -o ssl_server_arm ssl_server.c -Wall -g -lssl -lcrypto -I/tmp/openssl-1.0.1g/include -L/mnt/hgfs/F/ssl
del test_imgio.c
