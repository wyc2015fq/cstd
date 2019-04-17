# vs2013编译ffmpeg之十 libbluray、libbs2b、Libcaca - 枫林港的博客 - CSDN博客





2018年07月07日 22:49:55[枫林港](https://me.csdn.net/u013601132)阅读数：84








# libbluray

ShiftMediaProject上下载的[libbluray-master](https://github.com/ShiftMediaProject/libbluray)。MSVC2013打开就能编译。

# libbs2b

libbs2b只能用msys编译，下载的libbs2b-3.1.0自带sndfile的库(忘记是在哪里下载的了)，其根目录下建个sndfile.pc文件，其内容如下：

```bash
# sndfile.pc
prefix=./
exec_prefix=${prefix}
libdir=${exec_prefix}/win32/sndfile
includedir=${prefix}/win32/sndfile
Name: sndfile
Description: sndfile
Version: 1.0.20
Libs: -L${libdir} libsndfile-1.lib
Cflags: -I${includedir}
```

然后：

```bash
makedir msys_build
export PKG_CONFIG_PATH=./; ./configure --prefix=$(pwd)/msys_build
```

修改src/Makefile，不知道为何pkg-config没有将sndfile.pc里面的路径加到Makefile里面。CFLAGS里面加上-I ../win32/sndfile，LDFLAGS里面加上-L../win32/sndfile，-lsndfile改成-lsndfile-1。然后：
`make; make install`
编译结果在msys_build目录下。 

libbs2b和openal有几个函数冲突：

```
libbs2b.a(bs2b.o) : error LNK2005: _bs2b_set_level 已经在 OpenAL32.lib(bs2b.obj) 中定义
libbs2b.a(bs2b.o) : error LNK2005: _bs2b_get_level 已经在 OpenAL32.lib(bs2b.obj) 中定义
libbs2b.a(bs2b.o) : error LNK2005: _bs2b_set_srate 已经在 OpenAL32.lib(bs2b.obj) 中定义
libbs2b.a(bs2b.o) : error LNK2005: _bs2b_get_srate 已经在 OpenAL32.lib(bs2b.obj) 中定义
libbs2b.a(bs2b.o) : error LNK2005: _bs2b_clear 已经在 OpenAL32.lib(bs2b.obj) 中定义
```

直接将openal里面的函数改名。

# libcaca

下载[libcaca-0.99.beta19.tar](http://caca.zoy.org/wiki/libcaca)（最新的在[git上](https://github.com/cacalabs/libcaca)），解压后里面有sln文件，直接用MSVC2013打开，删除加载失败的工程，编译libcaca工程就可以生成libcaca.lib。要手动建立caca.pc，再执行ffmpeg-3.3的configure。 

下面这个错误：
`error LNK2019: 无法解析的外部符号 __imp__caca_create_canvas，该符号在函数 _check_caca_create_canvas 中被引用`
这个错误，关键点在caca.h里面：

```
#undef __extern
#if defined _DOXYGEN_SKIP_ME
#elif defined _WIN32 && defined __LIBCACA__ && defined DLL_EXPORT
#   define __extern extern __declspec(dllexport)
#elif defined _WIN32 && !defined __LIBCACA__ && !defined CACA_STATIC
#   define __extern extern __declspec(dllimport)
#else
#   define __extern extern
#endif
```

原始的libcaca工程里面的设置，定义了**LIBCACA**和DLL_EXPORT，编译的是动态库，手动改成静态库后，要定义CACA_STATIC，其他几个工程也需要定义。在caca.pc里面，Cflags也加上-DCACA_STATIC就可以解决这个问题。



