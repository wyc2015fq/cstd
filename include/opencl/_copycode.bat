@echo off

set out=Z:\yiwei\git\rongyi_term_driver\android\opencl\
mkdir %out%
copy /y *.h  %out%
copy /y *.txt  %out%
copy /y *.inl  %out%

echo #define test_cl_add main>%out%\test_cl.cpp
echo #include "test_cl_add.inl">>%out%\test_cl.cpp
::echo #include "sys.inl">>%out%\test_cl.cpp
echo #include "sys/sys_linux.inl">>%out%\test_cl.cpp
