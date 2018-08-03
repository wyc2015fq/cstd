@echo off

set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\
set WindowsKits=C:\Program Files (x86)\Windows Kits\10\
set WindowsSdkDir=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A
set CodeDir=D:\code
set NvidiaDir=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v9.2
set path="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\";%path%
set path="%VCINSTALLDIR%\bin\amd64\";%path%

set cc=cl.exe /nologo
::call vsvars64.bat

set INCLUDE=%VCINSTALLDIR%ATLMFC\INCLUDE;%VCINSTALLDIR%INCLUDE;%WindowsKits%Include\10.0.10150.0\ucrt;%WindowsSdkDir%\Include
set LIB=%VCINSTALLDIR%ATLMFC\LIB\arm64;%VCINSTALLDIR%\lib\amd64;%WindowsKits%Lib\10.0.10150.0\ucrt\x64;%WindowsSdkDir%\Lib\x64

set INCLUDE=%INCLUDE%;.\;%CodeDir%\git\cstd\include;%CodeDir%\git\cstd;%CodeDir%\git\cstd\3rd
set INCLUDE=%INCLUDE%;%CodeDir%\git\cstd\3rd\DirectShow\include;%CodeDir%\git\cstd\3rd;%CodeDir%\include;
set INCLUDE=%INCLUDE%;%CodeDir%\git\cstd\include\ocr\dnn
set INCLUDE=%INCLUDE%;%CodeDir%\lib\OpenBLAS\include;%CodeDir%\include\HDF5;%CodeDir%\include\LMDB
set INCLUDE=%INCLUDE%;%NvidiaDir%\include
set INCLUDE=%INCLUDE%;C:\ProgramData\Anaconda3\include;C:\ProgramData\Anaconda3\Lib\site-packages\numpy\core\include
set INCLUDE=%INCLUDE%;C:\local\boost_1_65_0
set LIB=%CodeDir%\lib\x64;%LIB%;%NvidiaDir%\lib\x64;D:\code\git\boost_1_65_0\stage\lib;C:\local\boost_1_65_0\lib64-msvc-14.0

::set LIB=%LIB%;

set cc=%cc% /GS /GL /W3 /Gy /Gd /Oi /MD /Zc:wchar_t /bigobj /Zi /Gm- /O2 /sdl /Zc:inline /fp:precise /EHsc /nologo
set cc=%cc% /D "_SCL_SECURE_NO_WARNINGS" /D "_CRT_SECURE_NO_WARNINGS" /D "_MBCS"
::/Fd"x64\Release\vc140.pdb" /errorReport:prompt /WX- /Zc:forScope /Fa"x64\Release\" /Fo"x64\Release\" /Fp"x64\Release\testc.pch"
