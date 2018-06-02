@echo off
::pclip|finds ": error"|finds "undeclared identifier"|sed "s/_//g">aaa.txt
pclip>>aaa.txt
cat aaa.txt|finds "undeclared identifier"|sed "s/_//g">bbb1.txt
cat aaa.txt|finds "unresolved external symbol"|sed "s/f2c_//g">bbb2.txt
del aa.txt
for /f "tokens=2 delims=''" %%i in (bbb1.txt) do (
 echo %%i>>aa.txt
)
for /f "tokens=9 delims=(): " %%i in (bbb2.txt) do (
 echo %%i>>aa.txt
)
cat aa.txt|sort|uniq>aa1.txt
::del bbb1.txt
::del bbb2.txt


set p=E:\code\cstd\lib\clapack-3.2.1-CMAKE\SRC
md out
for /f %%i in (aa1.txt) do (
	if exist "%p%\%%i.c" (
		echo #ifndef __%%~ni__>out\%%i.inl
		echo #define __%%~ni__>>out\%%i.inl
		for /f "tokens=3 delims=[]= " %%a in ('finds "static" "%p%\%%i.c"') do (
			echo #define %%a %%a_%%i>>out\%%i.inl
		)
		echo #include "%%i.c">>out\%%i.inl
		for /f "tokens=3 delims=[]= " %%a in ('finds "static" "%p%\%%i.c"') do (
			echo #undef %%a>>out\%%i.inl
		)
		echo #endif // __%%~ni__>>out\%%i.inl
	)
)


del clapack.inl
for %%i in (out\*.inl) do (
  echo #include "out\%%~ni.inl">>clapack.inl
)
cat clapack.inl

