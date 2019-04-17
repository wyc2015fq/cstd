# vs2013编译ffmpeg之二十六 opus、shine - 枫林港的博客 - CSDN博客





2018年07月09日 23:36:10[枫林港](https://me.csdn.net/u013601132)阅读数：199








# opus

对应ffmpeg configure选项–enable-libopus。

官网下载[opus-1.1.tar.gz](http://www.opus-codec.org/downloads/)版本

解压后在opus-1.1\win32\VS2010下面有sln文件，打开就可以编译。

Opus工程里面的属性设置需要修改，Project properties->Configuration properties->C/C++->Preprocessor->Preprocessor Definition里面需要将_CONSOLE定义去掉，否则configure的时候会有下面的错误，其他的库也是一样。

```
LIBCMTD.lib(dbgheap.obj) : error LNK2005: __heap_alloc already defined in LIBCMT.lib(malloc.obj)

LIBCMTD.lib(dbgheap.obj) : error LNK2005: __recalloc already defined in LIBCMT.lib(recalloc.obj)

LIBCMTD.lib(dbgheap.obj) : error LNK2005: __msize already defined in LIBCMT.lib(msize.obj)

LIBCMTD.lib(dbghook.obj) : error LNK2005: __crt_debugger_hook already defined in LIBCMT.lib(dbghook.obj)

LIBCMTD.lib(isctype.obj) : error LNK2005: __isctype_l already defined in LIBCMT.lib(isctype.obj)

LIBCMTD.lib(isctype.obj) : error LNK2005: __isctype already defined in LIBCMT.lib(isctype.obj)

   Creating library test.lib and object test.exp

LINK : warning LNK4098: defaultlib 'LIBCMTD' conflicts with use of other libs; use /NODEFAULTLIB:library
```

# shine

对应ffmpeg configure选项–enable-libshine。

[https://sourceforge.net/projects/libshine-fxp/](https://sourceforge.net/projects/libshine-fxp/)

[https://github.com/toots/shine](https://github.com/toots/shine)

定点MP3编码库。第一个链接最后的版本是libshine-fxp_20070104.tgz。第二个最后的版本是3.1.0，2014年的，用的是第二个版本，第一个链接的代码，ffmpeg configure的时候会失败。其源代码下面的readme文件里面的官网是[http://www.everett9981.freeserve.co.uk](http://www.everett9981.freeserve.co.uk)，但打不开。

编译方法：
`mkdir msys_build; ./configure --prefix=$(pwd)/msys_build; make; make install`
其代码比较少，应该很容易建立vs2013工程。

下面的错误：

```
libmp3lame-static.lib(takehiro.obj) : error LNK2005: _slen1_tab 已经在 libshine.a(tables.o) 中定义

libmp3lame-static.lib(takehiro.obj) : error LNK2005: _slen2_tab 已经在 libshine.a(tables.o) 中定义
```

两个库里面的符号名字冲突，没有好的解决办法，直接修改shine的代码，下面4个文件中需要修改，将slen1_tab、slen2_tab重命名。

```
./src/lib/l3bitstream.c:130:          unsigned slen1 = slen1_tab[ gi->scalefac_compress ];

./src/lib/l3loop.c:290:    slen1 = slen1_tab[ gi->scalefac_compress ];

./src/lib/tables.c:10:const int slen1_tab[16] = { 0, 0, 0, 0, 3, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4 };

./src/lib/tables.h:6:extern const int slen1_tab[16];
```



