# vs2013编译ffmpeg之二十八 snappy、soxr、speex - 枫林港的博客 - CSDN博客





2018年07月10日 23:24:28[枫林港](https://me.csdn.net/u013601132)阅读数：158








# snappy

对应ffmpeg configure选项–enable-libsnappy。

快速压缩库，注重速度。

## msys编译

其没有对应的vs工程，用git bash下载代码：git clone [https://github.com/google/snappy.git](https://github.com/google/snappy.git)。其代码里面没有说明如何编译在msys下编译命令：
`mkdir msys_build; ./configure --prefix=$(pwd)/msys_build; make; make install`
编译出来的.a文件有问题，在configure ffmpeg的时候报下面的错误：
`libsnappy.a(snappy.o) : fatal error LNK1143: 无效或损坏的文件:  没有 COMDAT 0x14 节的符号`
这个错误没有找到原因，可能是需要用lib工具转换一下，这个未作测试：

```python
lib  /machine:ix86  /def:xxx.def  /out:xxx.lib
```

## vs2013编译

从snappy的编译打印看，snappy的库文件只包含4个文件，所以直接用vs2013编译。创建一个新的Win32 Console Application，工程设置为空，将snappy.cc、snappy-c.cc、snappy-sinksource.cc、snappy-stubs-internal.cc加入工程。HAVE_CONFIG_H加入宏定义中，config.h里面需要修改，将：

```
#define HAVE_BUILTIN_CTZ 1

#define HAVE_BUILTIN_EXPECT 1
```

改成：

```
#ifdef WIN32

#else

#define HAVE_BUILTIN_CTZ 1

#endif

/* Define to 1 if the compiler supports __builtin_expect. */

#ifdef WIN32

#else

#define HAVE_BUILTIN_EXPECT 1

#endif
```

将输出的文件名改成snappy.lib。编译成功后就可以configure ffmpeg了。

# soxr

对应ffmpeg configure选项–enable-libsoxr。

从[https://sourceforge.net/projects/soxr/files/](https://sourceforge.net/projects/soxr/files/)上下载soxr-0.1.2-Source.tar.xz。

soxr是重采样的库。

源代码下有soxr-0.1.2-Source\msvc\libsoxr.vcproj，MSVC2013建立个空的解决方案libsoxr.sln，手动加入libsoxr.vcproj再编译。

# speex/speexdsp

对应ffmpeg configure选项–enable-libspeex。 

从[官网上](https://www.speex.org/downloads/)下载speex-1.2rc2、speexdsp-1.2rc3，两个模块的编译都在speex-1.2rc2\win32\VS2013\libspeex.sln里面（原本是VS2010，目录改成VS2013），将speexdsp-1.2rc3里面的工程都包含进来。



