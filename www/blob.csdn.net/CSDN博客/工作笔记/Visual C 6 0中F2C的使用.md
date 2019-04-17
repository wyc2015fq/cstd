# Visual C 6.0中F2C的使用 - 工作笔记 - CSDN博客





2012年02月01日 10:01:52[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5569








The compiler produces C code that must be linked against the runtime libraries libf77.lib and libi77.lib. While the translator itself runs in native Win32 mode, it may be necessary to install the Downhill library available at [http://www.accurev.com/free/u2nt/](http://www.accurev.com/free/u2nt/)
 (a lightweight Un*x to Win32 compatibility library) if you get unresolved externals during linking.

本文是对[netlib](http://www.netlib.org/)中Fortran77到C转换程序F2C的改编。你想要按自己的方式用F2C，但是，原是文章唯一的偏离是你不能区分用a*,f文件。换言之，每次只能编译一个源码文件。这里对f2c.h做了一下轻微改良

Download:
[Binary](http://www.weihenstephan.de/~syring/f2c/f2c.zip):

    f2c.exe, f2c.h, libi77.lib, libf77.lib

[Source](http://www.weihenstephan.de/~syring/f2c/f2cSrc.zip):

    contains the "binaries" together with the source and a test program

Remarks on building:

A sample Fortran test program is provided with the source distribution. The Fortran source is translated to C by VC5 custom build step and the resulting C program is compiled with the -MD option to link with libf77.lib and libi77.lib.

There is a potential problem with the binary/text dichotomy of the file opening modes. To be on the safe side, I would recommend to link your program with binmode.obj to set the default file mode to binary. In general, this required if your Fortran program
 uses a BACKSPACE. 

Original location of this page: [http://www.weihenstephan.de/~syring/f2c/f2c.html](http://www.weihenstephan.de/~syring/f2c/f2c.html)

Extras

I have compiled the f2ced BLAS, LINPACK and LAPACK sources from netlib as a DLL. The file netdll.zip contains the libs together with the DLLs and header files required to build the dlls. These includes (blas.h, linpack.h and lapack.h) are included now automatically
 by a modified f2c.h. This f2c.h also links in libi77.lib and libf77.lib automatically via a #pragma comment, while you have to add blas.lib, linpack.lib and lapack.lib manually to your project. Remember that both linpack and lapack rely on blas.

Otherwise, there is little special except modified seconds/dsecnd functions in LAPACK adapted for win32 (secondw.c). I have not figured out what floating point masks should be used, so there is no call to _controlfp anywhere.

To rebuild the BLAS, LINPACK and LAPACK libraries yourself, install the f2c.h, blas.h,linpack.h and lapack.h header files and rebuild the sources with

for %1 in (*.f) do f2c -A %1

cl -c -MT -DF2CDLL *.c

link /dll /out: name.dll *.obj (blas.lib)

[Download netdll.zip](http://www.weihenstephan.de/~syring/f2c/netdll.zip)



