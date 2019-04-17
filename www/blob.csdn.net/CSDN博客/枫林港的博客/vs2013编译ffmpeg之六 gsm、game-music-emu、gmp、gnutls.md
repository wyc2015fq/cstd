# vs2013编译ffmpeg之六 gsm、game-music-emu、gmp、gnutls - 枫林港的博客 - CSDN博客





2018年07月07日 22:45:00[枫林港](https://me.csdn.net/u013601132)阅读数：154标签：[ffmpeg																[编译																[vs2013																[gsm																[gnutils](https://so.csdn.net/so/search/s.do?q=gnutils&t=blog)
个人分类：[vs2013编译ffmpeg](https://blog.csdn.net/u013601132/article/category/7785737)





# gsm

gsm音频编解码库。 
[http://www.imagemagick.org/download/delegates/ffmpeg/](http://www.imagemagick.org/download/delegates/ffmpeg/)下载gsm-1.0-pl13，用msys编译的。 

在其根目录下执行：`make addtst; make V=1`
会报错，但不用管，已经生成了libgsm.a。

```
./src/toast.o:toast.c:(.text+0xb57): undefined reference to `fchmod'
./src/toast.o:toast.c:(.text+0xbcc): undefined reference to `fchown'
```

# game-music-emu

ShiftMediaProject上下载的[game-music-emu-master](https://github.com/ShiftMediaProject/game-music-emu)，MSVC2013打开sln文件就可以编译。

# gmp

ShiftMediaProject上下载的[gmp-master](https://github.com/ShiftMediaProject/gmp)，编译没有报错。

# gnutls

ShiftMediaProject上下载的[gnutls-master](https://github.com/ShiftMediaProject/gnutls)。 

编译libgnutls需要yasm、vsyasm、libgcc_s_dw2-1.dll、libiconv-2.dll、libintl-8.dll，放到C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin下面，后三个文件在/MinGW/bin下面；vsyasm.props、vsyasm.targets、vsyasm.xml，放到C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\BuildCustomizations下面，这三个文件在yasm的源代码下面。

下面这个错误：
`C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\BuildCustomizations\vsyasm.targets(45,5): error MSB3721: The command ""C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\"vsyasm.exe -Xvc -f Win32 -d "__NASM_VERSION_ID__=0" -o "K:\MinGW\msys\1.0\home\AA\contribute\MSVC\gnutls-master_20160630\SMP\obj\Debug\Win32\libgnutls\\" -rnasm -pnasm   "lib\accelerated\x86\nasm\aes-ssse3-x86.asm"  "lib\accelerated\x86\nasm\aesni-x86.asm"  "lib\accelerated\x86\nasm\cpuid-x86.asm"  "lib\accelerated\x86\nasm\e_padlock-x86.asm"  "lib\accelerated\x86\nasm\sha1-ssse3-x86.asm"  "lib\accelerated\x86\nasm\sha256-ssse3-x86.asm"" exited with code -1073741515.`
vsyasm.exe的位置要放正确，按前面描述的拷贝就不会有这个错误。命令里面包含vsyasm.exe及其参数，这个命令行是在vsyasm.props里面定义的。不只是需要vsyasm，还需要yasm、libgcc_s_dw2-1.dll、libiconv-2.dll、libintl-8.dll，放到C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin下面。错误打印中的命令放到脚本中执行则没有问题，因为/MinGW/bin下面已经有yasm了。 

工程里面yasm的设置如下： 
![这里写图片描述](https://img-blog.csdn.net/20180702002749191?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下面这个错误：
`fatal error LNK1241: resource file zlib1d.lib(zlib1.res) `
将“Link Time Code Generation”设置为yes可以解决该问题。](https://so.csdn.net/so/search/s.do?q=gsm&t=blog)](https://so.csdn.net/so/search/s.do?q=vs2013&t=blog)](https://so.csdn.net/so/search/s.do?q=编译&t=blog)](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)




