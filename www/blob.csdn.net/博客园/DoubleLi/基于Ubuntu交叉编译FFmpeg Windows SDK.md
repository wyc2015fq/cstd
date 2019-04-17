# 基于Ubuntu交叉编译FFmpeg Windows SDK - DoubleLi - 博客园






**写在前面**

**FFmpeg**是一个开源且跨平台的音视频解决方案，集采集、转码、流式化为一身，项目的**libavcodec**编解码模块和**libavformat**媒体格式模块，支持非常非常丰富的编解码格式和容器封装格式，是做媒体相关开发工作必须要掌握和借鉴的一个项目。定制和编译ffmpeg是做流媒体开发迟早要面对的，linux平台上相对简单，windows平台就比较麻烦了。本文的目的就是详细介绍下如何基于ubuntu交叉编译环境，编译和定制ffmpeg。



**一  概述**

　　ffmpeg主要是基于linux开发，当然它也支持windows，不过并不支持visual studio系列IDE（因为它使用了大量C99特性，而vs不支持C99），所以要想在windows上使用，必须要通过mingw或cygwin来编译，或者在linux上构建windows交叉编译环境，后者要简单些。这里有个网站[http://ffmpeg.zeranoe.com/](http://ffmpeg.zeranoe.com/)，作者提供了ffmpeg已经编译好的windows版本，包括静态连接的，动态连接的，以及sdk，基本上隔几天就编译一次。大家可以先上去看一下，如果它编译的符合你的需求(上次看的时候它好像没有加入libfaac，现在不知道了)，而你又懒得编译，就可以使用它的。对linux不太熟悉的朋友可能不太想继续往下看，请不要担心，ubuntu的安装和使用非常简单，直接在虚拟机里使用就行。编译的时候，你也只需要使用几个很简单的命令而已。



**二 环境准备**

　　1. **Ubuntu Linux**，本人使用的Ubuntu 11.10，安装在**VMware Workstation**中

　　2. **MinGW编译环境**，我们要非常感谢，zeranoe，它做了一个自动下载和编译ffmpeg所需要的mingw及其他工具链的脚本，请到[http://ffmpeg.zeranoe.com/blog/](http://ffmpeg.zeranoe.com/blog/)下载最新的“MingGW-w64 Build Script 2.8.2”。解压后新建一个目录把它放进去(本人的目录是**/home/haibindev/studio/mingw-w64/**)，然后打开终端，cd 到MinGW-w64所在目录，执行"chmod +x mingw-w64-build-2.8.2"，使这个脚本可以执行。之后运行"./mingw-w64-build-2.8.2"，就可以按照提示来做了。如果它提示你缺少依赖库，就按照它的提示先安装依赖库，方式是，执行"sudo apt-get install 库名字"。

![](https://pic002.cnblogs.com/images/2011/254714/2011120109400476.jpg)

　　3. 设置终端的环境变量

　　执行一下命令即可。（注意，如果你关闭了终端，或者新建一个终端，需要重新执行一下这个命令。）

```
export PATH="$PATH:/home/haibindev/studio/mingw-w64/mingw-w64-i686/bin"
```



**三 下载FFmpeg源码和你需要的编解码库的源码**

　　这点不用做什么介绍，先到ffmpeg官网，下载ffmpeg源代码，然后再Google你需要的编解码库，到相关网站下载即可。本人这次编译共下载了以下代码，以做大家参考。

```
libx264 libfaac libmp3lame libopenjpeg libogg libspeex libvorbis libtheora libxvid ffmpeg
```

　　下载完后解压，然后统一放到一个目录中，本人的目录是**/home/haibindev/studio/opensource/。**下图是全家福（build-ffmpeg是编译ffmpeg的工作目录，下面介绍，freetype我没有编译）。

![](https://pic002.cnblogs.com/images/2011/254714/2011120109513849.png)

**四 编译**

　　为了方便，我把所有库编译后的结果都存放到了一个统一的目录**/home/haibindev/opensdk，**方法请见下面的编译命令。

　　除了第一个命令以外，其余的都是configure命令，执行完之后，**还需要执行make命令，然后执行make install**。我就不再写出来了。

　　1. 先执行以下命令

```
ln -s "/usr/bin/pkg-config" /home/haibindev/studio/mingw-w64/mingw-w64-i686/bin/i686-w64-mingw32-pkg-config
```

　　2. **libx264**

```
./configure --enable-static --enable-win32thread --host=i686-w64-mingw32 --prefix=/home/haibindev/opensdk --cross-prefix=i686-w64-mingw32-
```

　　3. **libfaac**

```
./configure --host=i686-w64-mingw32 --prefix=/home/haibindev/opensdk --enable-static --disable-shared --with-mp4v2=no
```

　　4. **libmp3lame**

```
CFLAGS=-DFORCEINLINE= ./configure --host=i686-w64-mingw32 --prefix=/home/haibindev/opensdk --enable-static --disable-shared --disable-frontend
```

　　5. **libopenjpeg**

```
./configure --host=i686-w64-mingw32 --prefix=/home/haibindev/opensdk --enable-static --disable-shared
```

　　6. **libogg**

```
./configure --host=i686-w64-mingw32 --prefix=/home/haibindev/opensdk --enable-static --disable-shared
```

　　7. **libspeex**

```
./configure --host=i686-w64-mingw32 --prefix=/home/haibindev/opensdk --enable-static --disable-shared --disable-oggtest
```

　　8. **libvorbis**

```
./configure --host=i686-w64-mingw32 --prefix=/home/haibindev/opensdk --enable-static --disable-shared --disable-oggtest
```

　　9. **libtheora**

```
./configure --host=i686-w64-mingw32 --prefix=/home/haibindev/opensdk --enable-static --disable-shared --disable-oggtest --disable-vorbistest --disable-sdltest --with-ogg-includes=/home/haibindev/opensdk/include --with-ogg-libraries=/home/haibindev/opensdk/lib
```

　　10. **libxvid**

```
./configure --host=i686-w64-mingw32 --prefix=/home/haibindev/opensdk --disable-pthread
```

　　11. **ffmpeg**

　　先创建**/home/haibindev/studio/opensource/build-ffmpeg**，然后cd到该目录，执行

```
../ffmpeg-0.8.7/configure --disable-static --enable-shared --enable-version3 --enable-gpl --enable-nonfree --disable-pthreads --enable-w32threads --enable-runtime-cpudetect --enable-memalign-hack --enable-libfaac  --enable-libmp3lame --enable-libopenjpeg --enable-libspeex --enable-libtheora --enable-libvorbis --enable-libx264 --enable-libxvid --enable-zlib --enable-cross-compile --target-os=mingw32 --arch=x86 --prefix=/home/haibindev/opensdk/ffmpeg --cross-prefix=i686-w64-mingw32- --extra-cflags="-I/home/haibindev/opensdk/include" --extra-ldflags="-L/home/haibindev/opensdk/lib"
```

　　注意，编译xvid时需要先编辑configure文件，然后删除里面的“-mno-cygwin”字段，否则make会失败，提示-mno-cygwin无效。libmp3lame命中中最前面的“CFLAGS=-DFORCEINLINE= ”是为了防止mp3lame中的一个编译错误，要记得带上。

　　以上编译成功之后，ffmpeg win32 sdk就出现在**/home/haibindev/opensdk/ffmpeg**里面了，bin目录下就是我们需要的所有文件了。

![](https://pic002.cnblogs.com/images/2011/254714/2011120110143566.png)



**五 验证编译后的程序**

　　拷贝到windows上，在命令中运行ffmpeg.exe，看

![](https://pic002.cnblogs.com/images/2011/254714/2011120110164869.jpg)

　　验证一下，做个转码操作就行了，执行

```
ffmpeg.exe -i s.avi -vcodec libx264 -acodec libfaac s.mp4
```

　　运行正常，生成了264编码和aac编码的，mp4文件，播放正常，ok了。

![](https://pic002.cnblogs.com/images/2011/254714/2011120110183916.jpg)



**六 最后说一下**

　　请大家编译的时候千万别忘了configure之后，要执行make和make install！另外，一定要看清楚我贴出来的命令，把我的目录，换成你的目录，不要直接拷贝，那是不行的，因为你不叫haibindev:)。如果有的库在make的时候出现了编译错误，简单修改一下源文件就能搞定。

　　ffmpeg有很多配置选项，你可以对它深度定制，可以把avcodec编译得很小，大家可以自己研究。









