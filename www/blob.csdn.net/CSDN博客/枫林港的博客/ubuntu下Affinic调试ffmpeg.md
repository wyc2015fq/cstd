# ubuntu下Affinic调试ffmpeg - 枫林港的博客 - CSDN博客





2018年07月26日 23:05:32[枫林港](https://me.csdn.net/u013601132)阅读数：69








# ubuntu下调试ffmpeg

在windows下用VS2013编译的ffmpeg-3.3+libzvbi，播放的时候出来的ttx都是乱码，为了对比，在ubuntu上搭建了ffmpeg+libzvbi环境。

用的IDE调试环境是Affinic 2.0.1破解版，Affinic是基于gdb的。

## ffmpeg编译

ffmpeg的完整编译方法请参考博主的[ubuntu17下编译ffmpeg](https://blog.csdn.net/u013601132/article/details/81177294)。

通常在ubuntu下编译是动态编译的，ffmpeg的二进制文件小于1M。要用gdb调试的话需要需要加-g编译，要静态编译。ffmpeg的configure的时候要加下列参数：

> 
–enable-debug或 –enable-debug=3

–disable-shared –enable-static  //加了这两个标志后，要make clean后再编译，否则结果可能还是动态编译的。


要去掉ffmpeg编译优化，在configure里面，在判断gcc编译器的时候，在下面这个语句后面：

> 
elif $_cc -v 2>&1 | grep -qi ^gcc; then


`注释`掉下面两句：

```bash
_cflags_speed='-O3'

        _cflags_size='-Os'
```

configure里面检测zvbi的规则要改下：

> 
enabled libzvbi           && require libzvbi libzvbi.h vbi_decoder_new -lzvbi &&


改成：

> 
enabled libzvbi           && require libzvbi libzvbi.h vbi_decoder_new -L../contribute/zvbi-0.2.35/src/.libs/ -lzvbi -lpng &&


最终的ffmpeg的configure命令如下：

```bash
./configure --prefix=$(pwd)/out --extra-version='1ubuntu1~trusty6' --build-suffix=-ffmpeg \

    --toolchain=hardened --extra-cxxflags= \

    --extra-cflags="-I../contribute/zvbi-0.2.35/src" \

    --extra-ldflags="-L../contribute/zvbi-0.2.35/src/.libs" \

    --enable-gpl --enable-shared --disable-stripping --enable-avresample --enable-avisynth \

    --enable-fontconfig --enable-gnutls --enable-ladspa --enable-libass --enable-libbluray \

    --enable-libbs2b --enable-libcaca --enable-libcdio --enable-libflite --enable-libfreetype \

    --enable-libfribidi --enable-libgme --enable-libgsm --enable-libmodplug --enable-libmp3lame \

    --enable-libopenjpeg --enable-libopus --enable-libpulse --enable-librtmp --enable-libschroedinger \

    --enable-libshine --enable-libspeex --enable-libssh --enable-libtheora --enable-libtwolame \

    --enable-libvorbis --enable-libvpx --enable-libwavpack --enable-libwebp --enable-opengl \

    --enable-libxvid --enable-libx265 --enable-libdc1394 --enable-libiec61883 \

    --enable-libzvbi --enable-libzmq --enable-frei0r --enable-libx264 --enable-libsoxr --enable-openal \

    --enable-libopencv --enable-libzvbi
```

注：configure参数不能随意加`--libdir`、`--shlibdir`、`--incdir`等参数，否则在定义了自己的–prefix情况下，库的install路径可能不是自己想要的。这三个参数定义了的话，头文件、库文件路径由这些参数所指定的路径来决定。

按前面的方法编译出来的静态的ffmpeg，ffmpeg运行前需要执行`export LD_LIBRARY_PATH=/usr/local/lib`，主要是为了找到x264、x265的so，或者直接用ldconfig命令将so库的路径放到环境变量LD_LIBRARY_PATH里面，参考“[ubuntu下编译ffmpeg](https://blog.csdn.net/u013601132/article/details/81177294)”。如果是动态编译，则还需要将ffmpeg自身的so路径也放到LD_LIBRARY_PATH里面。

## libzvbi编译

为了gdb能调试，需要加-g编译。没有研究其configure中如何去掉-O并加-g，直接在configure后src目录下的Makefile里面修改，去掉所有的-On，加上-g3。

下面这个错误，直接在g3后面加上-fPIC，ubuntu 64位需要加这个编译选项：

> 
/usr/bin/ld: ../../contribute/zvbi-0.2.35/src/.libs/libzvbi.a(cache.o): relocation R_X86_64_32 against `.rodata’ can not be used when making a shared object; recompile with -fPIC

../../contribute/zvbi-0.2.35/src/.libs/libzvbi.a: error adding symbols: 错误的值


libzvbi编译出来的so，不带符号，没法打断点，需要用静态编译，链接的时候要确保链接的是自己编译出来的.a。另外系统中可能已经安装了libzvbi的库，需要确保在/usr/lib、/usr/local/lib下没有libzvbi库。 

 configure命令如下：

```bash
./configure --enable-dvb --disable-shared --enable-static
```

## Affinic调试

ubuntu进xwindow，Affinic 2.0.1解压后在终端执行adg，就可以看到界面了，adg32是对应的32位的二进制文件。

Program->Open Program… 打开要调试的ffmpeg二进制文件。

点run按钮才能设置ffmpeg的参数。

Control菜单里面有很多gdb命令，右侧小窗口是变量、寄存器的值。

Affinic打开ffplay后，点运行按钮，会弹出提示框要求输入参数。并且有历史参数保留，比较方便。

要能看到libzvbi的代码，才能在libzvbi的代码里面打断点，并能断住。没有看到libzvbi的代码或断不住，则说明环境搭的不对。 
![这里写图片描述](https://img-blog.csdn.net/20180726230007343?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



