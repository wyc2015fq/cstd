@echo off
echo #include "face/facetrack.inl">aaa.cpp
echo #include "img/imgio.inl">>aaa.cpp
echo #include "draw/imdrawaa.inl">>aaa.cpp
echo #include "facedetect/test_rongyinet.inl">>aaa.cpp
echo #include "lib/curl/curl.inl">libcurl.c
echo #include "cap/libcap.inl">>aaa.cpp
echo #include "sys/sys_linux.inl">>aaa.cpp
::echo #include "cstd.h">aaa.cpp
echo int main() {return 0;}>>aaa.cpp
F:\pub\A20-Clover\work\arm-2014.05\bin\arm-none-linux-gnueabi-gcc.exe aaa.cpp libcurl.c -IE:\code\cstd\include -IE:\code\cstd -IE:\code\cstd\lib -IE:\code\cstd\lib\curl -lpthread -lm   -Wno-unused-variable -Wno-unused-function -Wall -Wunused-parameter -Wno-unused-parameter 
:: -Wmissing-field-initializers -Wunused-parameter
del aaa.cpp libcurl.c