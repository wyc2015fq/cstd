@echo off
echo #include "test_CLM.inl">aaa.cpp
::echo #include "cstd.h">aaa.cpp
::echo #define test_cpu main>aaa.cpp
::echo #include "sys/test_cpu.inl">>aaa.cpp
::echo #include "lib.inl">>aaa.cpp

F:\pub\A20-Clover\work\arm-2014.05\bin\arm-none-linux-gnueabi-g++.exe -w -std=c++11 aaa.cpp -IE:\code\c\AI\dlib\dlib-19.4 -ID:\code\CLM-framework-master\lib\3rdParty\OpenCV3.0\include -IE:\code\cstd\include -I. -IE:\code\cstd -IE:\code\cstd\lib -IE:\code\cstd\lib\curl -lpthread -lm   -Wno-unused-variable -Wno-unused-function -Wall -Wunused-parameter -Wno-unused-parameter 
:: -Wmissing-field-initializers -Wunused-parameter
::del aaa.cpp
