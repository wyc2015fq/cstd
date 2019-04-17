# vs2013编译ffmpeg之三十三 X265 - 枫林港的博客 - CSDN博客





2018年07月10日 23:33:23[枫林港](https://me.csdn.net/u013601132)阅读数：167








# X265

对应ffmpeg configure选项–enable-libx265。

x265编译需要yasm，yasm一直用的是1.2.0版本（x265 2.6之后的版本用的是nasm，为了编译速度），此版本的yasm编译x265的时候会异常退出，原因是里面有个hash表定义的太小，需要打补丁才能编译x265。即yasm-1.3.0\modules\preprocs\nasm\nasm-pp.c里面：
`#define NHASH 31`
改成：
`#define NHASH 4096`
重新编译yasm并生成yasm.exe，并替换掉C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin下面的yasm.exe。

下载x265源代码：

```php
hg clone https://bitbucket.org/multicoreware/x265
```

hg工具需要下载[tortoisehg-2.11.2-hg-2.9.2-x86.msi](https://tortoisehg.bitbucket.io/download/index.html)，并安装。

在~/build/vc10-x86下面执行make-solutions.bat，但会报错：

```
./make-solutions.bat: line 1: @echo: command not found

./make-solutions.bat: line 2: ::: command not found

./make-solutions.bat: line 3: ::: command not found

./make-solutions.bat: line 4: ::: command not found

./make-solutions.bat: line 5: ::: command not found

CMake Error: The source directory "H:/MinGW/msys/1.0/home/AA/ffmpeg_MSVC/contribute/MSVC/x265_20160531/x265/build/vc10-x86/....source" does not exist.
```

make-solutions.bat是msys下执行的，有两个错误，@和:: 在msys下都不能识别，1~5行都用#注释掉或删除。路径里面的“\”不能识别，应该用双斜杠“\\”。直接用windows的cmd窗口应该可以执行，未测试。

或者直接在msys里面执行make-solutions.bat里面的cmake命令：

```bash
cmake -G "Visual Studio 10" ..\..\source && cmake-gui ..\..\source
```

最后会弹出cmake-gui的界面，点击上面的Configure、Genare，就会生成sln文件。用MSVC打开sln文件，然后编译。同样x265各工程里面要设置成/MTd。编译结果在build\vc10-x86\Debug下面，编译ffmpeg用的是x265-static.lib。

## 编译错误一

用MSVC2013编译ffmpeg-3.0会有下面这个错误，用dumpbin -SYMBOLS  x265-static.lib将符号表导出来，对应的符号是x265_api_get_84，少了下划线。

```
1>libavdevice.a(dshow.o) : error LNK2019: unresolved external symbol impSHCreateStreamOnFileA@12 referenced in function _dshow_open_device

1>libavformat.a(tls_schannel.o) : error LNK2019: unresolved external symbol _FreeContextBuffer@4 referenced in function _tls_shutdown_client

1>libavformat.a(tls_schannel.o) : error LNK2019: unresolved external symbol _InitializeSecurityContextA@48 referenced in function _tls_shutdown_client

1>libavformat.a(tls_schannel.o) : error LNK2019: unresolved external symbol impApplyControlToken@8 referenced in function _tls_shutdown_client

1>libavformat.a(tls_schannel.o) : error LNK2019: unresolved external symbol impFreeCredentialsHandle@4 referenced in function _tls_close

1>libavformat.a(tls_schannel.o) : error LNK2019: unresolved external symbol impDeleteSecurityContext@4 referenced in function _tls_close

1>libavformat.a(tls_schannel.o) : error LNK2019: unresolved external symbol _AcquireCredentialsHandleA@36 referenced in function _tls_open

1>libavformat.a(tls_schannel.o) : error LNK2019: unresolved external symbol _DecryptMessage@16 referenced in function _tls_read

1>libavformat.a(tls_schannel.o) : error LNK2019: unresolved external symbol _EncryptMessage@16 referenced in function _tls_write

1>libavformat.a(tls_schannel.o) : error LNK2019: unresolved external symbol _QueryContextAttributesA@12 referenced in function _tls_write

1>libavcodec.a(libx265.o) : error LNK2019: unresolved external symbol _x265_api_get_84 referenced in function _libx265_encode_init

1>H:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\MSVC_ffmpeg\ffmpeg-3.0\msvc\MSVC_ffmpeg-3.0\Debug\ffplay.exe : fatal error LNK1120: 11 unresolved externals
```

FreeContextBuffer 对应的库是Secur32.lib；SHCreateStreamOnFileA在Shlwapi.lib里面，链接的时候需要加上这两个库。缺x265_api_get_84是因为工程里面的x265的路径设置错了，路径是拷贝的，和实际的不符，改了就没有问题了。

## 编译错误二

Win7 64位系统，安装的是cmake-3.6.0-rc3-win64-x64.msi，在编译x265的时候，cmake命令必须用：cmake -G “Visual Studio 12 2013” ..\..\source，且在windows的cmd窗口下执行(MSVC的prompt concsole，msys的终端没有测试过)，2013后面不能带Win64，这样产生的库都是32位的。不然带Win64的库后面编译会有问题。

## 编译错误三

一个引用ffmpeg库的工程在编译时出现了下面的错误：
`1>libavcodec.a(libx265.o) : error LNK2019: unresolved external symbol _x265_api_get_84 referenced in function _libx265_encode_init`
出现这个问题，是因为编译ffmpeg的时候用的x265版本，和编译引用ffmpeg库的工程不是同一个x265。将工程里面的x265路径弄成和ffmpeg一样就解决了这个问题。

## 编译错误四

> 
error LNK2019: unresolved external symbol _x265_encoder_open_52 referenced in function _libx265_encode_init


这个错误查了好几天，把x265.h里面的#define x265_encoder_open x265_encoder_glue2(x265_encoder_open_, X265_BUILD)宏去掉，也不能解决问题。重新编译x265的库也不行。最后发现是ffplay的工程里面设置的问题，工程里面linker的路径设置不对。X265由于要用最新的，重新下载放在新的目录下了，在建立ffplay的工程的时候，由于路径都是拷贝的，没有注意到x265库的路径不对，恰好错误的路径下有x265-static.lib。将路径改正确就没有问题了。



