@echo off

set p=E:\code\cstd\lib\clapack-3.2.1-CMAKE\SRC
md out
for %%i in (%p%\*.c) do (
	echo %%~ni
	if not exist "out\%%~ni.inl" (
		echo #ifndef __%%~ni__>out\%%~ni.inl
		echo #define __%%~ni__>>out\%%~ni.inl
		for /f "tokens=3 delims=[]=,;* " %%a in ('finds "static" "%p%\%%~ni.c"') do (
			echo #define %%a %%a_%%~ni>>out\%%~ni.inl
		)
		echo #include "%%~ni.c">>out\%%~ni.inl
		for /f "tokens=3 delims=[]=,;* " %%a in ('finds "static" "%p%\%%~ni.c"') do (
			echo #undef %%a>>out\%%~ni.inl
		)
		echo #endif // __%%~ni__>>out\%%~ni.inl
	)
)

