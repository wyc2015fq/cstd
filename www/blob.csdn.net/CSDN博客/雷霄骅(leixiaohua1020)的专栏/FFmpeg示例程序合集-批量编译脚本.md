# FFmpeg示例程序合集-批量编译脚本 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年02月23日 15:20:04[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：11784
所属专栏：[FFmpeg](https://blog.csdn.net/column/details/ffmpeg-devel.html)









                此前做了一系列有关FFmpeg的示例程序，组成了《[最简单的FFmpeg示例程序合集](http://blog.csdn.net/leixiaohua1020/article/details/42658139)》，其中包含了如下项目：
simplest ffmpeg player:                  最简单的基于FFmpeg的视频播放器
simplest ffmpeg audio player:       最简单的基于FFmpeg的音频播放器
simplest ffmpeg pic encoder:        最简单的基于FFmpeg的图像编码器
simplest ffmpeg video encoder:    最简单的基于FFmpeg的视频编码器
simplest ffmpeg audio encoder:   最简单的基于FFmpeg的音频编码器
simplest ffmpeg streamer:             最简单的基于FFmpeg的推流器（推送RTMP）
simplest ffmpeg mem handler:     最简单的基于FFmpeg的内存读写例子
simplest ffmpeg device:                 最简单的基于FFmpeg的AVDevice例子
simplest ffmpeg format:                 最简单的基于FFmpeg的封装格式方面的例子
simplest ffmpeg video filter:          最简单的基于FFmpeg的AVfilter例子（水印叠加）
simplest ffmpeg swscale:             最简单的基于FFmpeg的libswscale的示例

开始的时候，我只是在VC2010下调试通过了这些程序。去年就有不少朋友跟我提建议希望能把代码改成跨平台的。后来我调查了一下也发现FFmpeg确实在各个平台都有广泛的应用，因此也产生了把代码改成跨平台的意愿。但是改成跨平台代码的工作量比较大，所以一直也没有做出实际行动。今年春节前夕可算是得到了一阵相对自由的时间，于是果断进行了一阵子“连续战斗”，终于实现了这些工程在VC++，cl.exe，MinGW，Linux GCC以及MacOS GCC上面顺利的编译。在这个过程中，也写了各种编译器的编译脚本，在这里记录一下。
本文记录以下几种编译脚本：

> **VC++：**simplest_ffmpeg_demos_compile_devenv_all.bat
**CL.exe：**simplest_ffmpeg_demos_compile_cl_all.bat
**MinGW：**simplest_ffmpeg_demos_compile_mingw_all.sh
**GCC (Linux/MacOS)：**simplest_ffmpeg_demos_compile_gcc_all.sh

![](https://img-blog.csdn.net/20150224112724627)![](https://img-blog.csdn.net/20150223150313008)![](https://img-blog.csdn.net/20150223150318484)![](https://img-blog.csdn.net/20150223150340711)
下载地址：
【Github】[https://github.com/leixiaohua1020/leixiaohua1020.github.io/tree/master/batch](https://github.com/leixiaohua1020/leixiaohua1020.github.io/tree/master/batch)



## 准备工作

在记录具体的编译脚本之前，首先简单记录一下在不同平台下编译这些FFmpeg工程需要做的准备工作。注意这一步骤针对的是一台完全空白未做任何配置的“裸机”。如果已经编译安装过FFmpeg，就可以直接跳过这一步骤。

PS：一些示例程序需要安装SDL2，方法类似，不再记录。

### VC++

VC++的编译是最简单的，相关的include和lib都已经配置好了，可以直接编译运行。

### CL.exe

基本上等同于使用VC++编译，可以直接编译运行。

### MinGW

两种方法：直接安装和源代码编译

**直接安装**

(1)从FFmpeg Windows Build ([http://ffmpeg.zeranoe.com/](http://ffmpeg.zeranoe.com/)) 网站下载最新的shared 和dev版本的FFmpeg。

(2)在Msys安装目录下创建“local”文件夹，“local”文件夹下创建“include”和“lib”文件夹。

(3)将FFmpeg的dev版本下的include拷贝至{msys}/local/include；lib拷贝至{msys}/local/lib。

(4)将FFmpeg的shared版本下的Dll拷贝至{mingw}/bin。

**源代码编译**

(1)安装Yasm

从官网下载yasmXXX.exe，然后重命名为yasm.exe，拷贝至{mingw}/bin

PS:也可以下载源代码自己编译，稍微麻烦些，不再记录。

(2)编译安装SDL

PS：注意SDL的安装路径如果没有安装在{msys}/local目录下的话，configure的时候设置一下。

(3)编译安装libx264

(3)编译安装libfaac

(4)编译安装FFmpeg



```
./configure --enable-shared --enable-libfaac --enable-libx264 --enable-gpl --enable-nonfree
make
make install
```




### Linux

(0)前期准备

某些Linux没有安装gcc/g++，需要先安装gcc/g++

进入超级管理员“su”

(1)安装相关的类库

*[Debian/Ubuntu]*



`apt-get -y install yasm libfaac-dev libx264-dev libsdl1.2-dev`


PS：这些类库也可以下载源代码手动编译，稍微麻烦些，不再记录。


*[RedHat/Fedora/CentOS]*


需要手动编译每个类库

(2)编译安装FFmpeg



```
./configure --enable-shared --enable-libfaac --enable-libx264 --enable-gpl --enable-nonfree
make
make install
```




### MacOS

两种方法：直接安装和源代码编译


**直接安装**


(1)安装Homebrew


`ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`


(2)安装FFmpeg


`brew install ffmpeg`





**源代码编译**




(1)安装Homebrew


`ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`


(2)安装相关的类库


`brew install yasm faac x264 sdl`


(3)编译安装FFmpeg

进入超级管理员“su”



```
./configure --enable-shared --enable-libfaac --enable-libx264 --enable-gpl --enable-nonfree
make
make install
```






## VC++




![](https://img-blog.csdn.net/20150223150225054)

使用devenv.exe进行编译是最简单的一种命令行编译方式。这种编译方式和使用VC++代开*.sln解决方案文件然后单击“编译”按钮的效果是一样的。由于项目解决方案中已经做过了include，lib以及相关选项的设置，所以不需要做各种参数的配置。下面这条命令编译Debug版本程序：
`devenv.exe simplest_ffmpeg_player2.sln /rebuild Debug`下面这条命令编译生成Release版本程序，同时将编译过程中的日志输出到“sf_player_release_ compile_log.txt”文件中：
`devenv.exe simplest_ffmpeg_player2.sln /rebuild Release /Out sf_player_release_ compile_log.txt`使用devenv进行编译的时候，需要VC++运行环境，有2种方法可以设置：
（1）       批处理调用VC++设置环境的脚本，例如对于VC2010来说，位于“D:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat”
（2）       在“Visual Studio 命令提示符”中运行批处理（或者编译命令）。“Visual Studio 命令提示符”位于伴随着VC++安装，位于“Visual Studio Tools”目录下。

完整的脚本simplest_ffmpeg_demos_compile_devenv_all.bat如下所示。将脚本拷贝至《最简单的FFmpeg示例程序合集》所在目录运行，就可以编译所有项目的Release版本和Debug版本，并且输出相关的编译日志。


```
@echo off
echo ---------------------------------------------
echo simplest ffmpeg demos list ---- Devenv compile all
echo Lei Xiaohua
echo Communication University of China / Digital TV Technology
echo leixiaohua1020@126.com
echo http://blog.csdn.net/leixiaohua1020
echo ---------------------------------------------
 
set logfile=compile_log.txt
::VS2010 Environment
call "D:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_player...
cd simplest_ffmpeg_player
devenv.exe simplest_ffmpeg_player2.sln /rebuild Debug /Out ../sf_player_debug_%logfile%
devenv.exe simplest_ffmpeg_player2.sln /rebuild Release /Out ../sf_player_release_%logfile%
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_audio_player...
cd simplest_ffmpeg_audio_player
devenv.exe simplest_ffmpeg_audio_player2.sln /rebuild Debug /Out ../sf_audio_player_debug_%logfile%
devenv.exe simplest_ffmpeg_audio_player2.sln /rebuild Release /Out ../sf_audio_player_release_%logfile%
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_picture_encoder...
cd simplest_ffmpeg_picture_encoder
devenv.exe simplest_ffmpeg_picture_encoder.sln /rebuild Debug /Out ../sf_picture_encoder_debug_%logfile%
devenv.exe simplest_ffmpeg_picture_encoder.sln /rebuild Release /Out ../sf_picture_encoder_release_%logfile%
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_video_encoder...
cd simplest_ffmpeg_video_encoder
devenv.exe simplest_ffmpeg_video_encoder.sln /rebuild Debug /Out ../sf_video_encoder_debug_%logfile%
devenv.exe simplest_ffmpeg_video_encoder.sln /rebuild Release /Out ../sf_video_encoder_release_%logfile%
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_audio_encoder...
cd simplest_ffmpeg_audio_encoder
devenv.exe simplest_ffmpeg_audio_encoder.sln /rebuild Debug /Out ../sf_audio_encoder_debug_%logfile%
devenv.exe simplest_ffmpeg_audio_encoder.sln /rebuild Release /Out ../sf_audio_encoder_release_%logfile%
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_streamer...
cd simplest_ffmpeg_streamer
devenv.exe simplest_ffmpeg_streamer.sln /rebuild Debug /Out ../sf_streamer_debug_%logfile%
devenv.exe simplest_ffmpeg_streamer.sln /rebuild Release /Out ../sf_streamer_release_%logfile%
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_mem_handler...
cd simplest_ffmpeg_mem_handler
devenv.exe simplest_ffmpeg_mem_handler.sln /rebuild Debug /Out ../sf_mem_handler_debug_%logfile%
devenv.exe simplest_ffmpeg_mem_handler.sln /rebuild Release /Out ../sf_mem_handler_release_%logfile%
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_device...
cd simplest_ffmpeg_device
devenv.exe simplest_ffmpeg_device.sln /rebuild Debug /Out ../sf_device_debug_%logfile%
devenv.exe simplest_ffmpeg_device.sln /rebuild Release /Out ../sf_device_release_%logfile%
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_format...
cd simplest_ffmpeg_format
devenv.exe simplest_ffmpeg_format.sln /rebuild Debug /Out ../sf_format_debug_%logfile%
devenv.exe simplest_ffmpeg_format.sln /rebuild Release /Out ../sf_format_release_%logfile%
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_video_filter...
cd simplest_ffmpeg_video_filter
devenv.exe simplest_ffmpeg_video_filter.sln /rebuild Debug /Out ../sf_video_filter_debug_%logfile%
devenv.exe simplest_ffmpeg_video_filter.sln /rebuild Release /Out ../sf_video_filter_release_%logfile%
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_swscale...
cd simplest_ffmpeg_swscale
devenv.exe simplest_ffmpeg_swscale.sln /rebuild Debug /Out ../sf_swscale_debug_%logfile%
devenv.exe simplest_ffmpeg_swscale.sln /rebuild Release /Out ../sf_swscale_release_%logfile%
cd ..
 
::=====================================================================================
```



## CL.exe




![](https://img-blog.csdn.net/20150223150241948)

cl.exe 是Microsoft C/C++编译器，和GCC属于同一个层面的东西。一个基本的调用cl.exe编译的命令行如下所示：
`cl.exe helloworld.cpp`上述命令执行完后即可在同一目录下生成一个helloworld.exe的可执行程序。
编译包含类库的程序相对来说要复杂一些，下面以Simplest FFmpeg Player为例看一下它的编译脚本。Simplest FFmpeg Player使用CL.exe编译的脚本保存在“compile_cl.bat”文件中，如下所示。

```
::最简单的基于FFmpeg的视频播放器 2----命令行编译
::Simplest FFmpeg Player 2----Compile in Cmd
::
::雷霄骅 Lei Xiaohua
::leixiaohua1020@126.com
::中国传媒大学/数字电视技术
::Communication University of China / Digital TV Technology
::http://blog.csdn.net/leixiaohua1020
::
::VS2010 Environment
call "D:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
::include
@set INCLUDE=include;%INCLUDE%
::lib
@set LIB=lib;%LIB%
::compile and link
cl simplest_ffmpeg_player.cpp /MD /link SDL2.lib SDL2main.lib avcodec.lib ^
avformat.lib avutil.lib avdevice.lib avfilter.lib postproc.lib swresample.lib swscale.lib ^
/SUBSYSTEM:WINDOWS /OPT:NOREF
exit
```

这个脚本按照顺序做了以下几点工作：
（1）设置VC++运行环境。这一步通过call "D:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"实现。
（2）设置include目录。这一步设置FFmpeg以及SDL2的头文件所在的目录，通过修改include环境变量实现（在include环境变量前面加上项目文件夹中的“include”目录）。
（3）设置lib目录。这一步设置FFmpeg以及SDL2的库文件所在的目录，通过修改lib环境变量实现（在lib环境变量前面加上项目文件夹中的“lib”目录）。
（4） 编译和链接。这一步用于将simplest_ffmpeg_player.cpp编译生成simplest_ffmpeg_player.exe。在这里需要注意几点：

> a) 链接类库使用/link
b) 使用SDL类库的时候，务必设置/MD选项（使用动态链接的库）
c)  使用SDL类库的时候，务必设置/SUBSYSTEM:WINDOWS
d)  使用FFmpeg类库的时候，务必设置/OPT:NOREF
上述脚本运行完城后，生成simplest_ffmpeg_player.exe。

完整的脚本simplest_ffmpeg_demos_compile_cl_all.bat如下所示。将脚本拷贝至《最简单的FFmpeg示例程序合集》所在目录运行，就可以编译所有的示例程序。注意这个脚本只是分别调用了各个程序目录下的compile_cl.bat文件。


```
@echo off
echo ---------------------------------------------
echo simplest ffmpeg demos list ---- CL compile all
echo Lei Xiaohua
echo Communication University of China / Digital TV Technology
echo leixiaohua1020@126.com
echo http://blog.csdn.net/leixiaohua1020
echo ---------------------------------------------
::=====================================================================================
 
echo.
echo Compile simplest_ffmpeg_player...
cd simplest_ffmpeg_player
cd simplest_ffmpeg_player
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_player_su
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_decoder_pure
start /wait compile_cl.bat
cd ..
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_audio_player...
cd simplest_ffmpeg_audio_player
cd simplest_ffmpeg_audio_player
start /wait compile_cl.bat
cd ..
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_picture_encoder...
cd simplest_ffmpeg_picture_encoder
cd simplest_ffmpeg_picture_encoder
start /wait compile_cl.bat
cd ..
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_video_encoder...
cd simplest_ffmpeg_video_encoder
cd simplest_ffmpeg_video_encoder
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_video_encoder_pure
start /wait compile_cl.bat
cd ..
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_audio_encoder...
cd simplest_ffmpeg_audio_encoder
cd simplest_ffmpeg_audio_encoder
start /wait compile_cl.bat
cd ..
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_streamer...
cd simplest_ffmpeg_streamer
cd simplest_ffmpeg_streamer
start /wait compile_cl.bat
cd ..
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_mem_handler...
cd simplest_ffmpeg_mem_handler
cd simplest_ffmpeg_mem_player
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_mem_transcoder
start /wait compile_cl.bat
cd ..
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_device...
cd simplest_ffmpeg_device
cd simplest_ffmpeg_grabdesktop
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_readcamera
start /wait compile_cl.bat
cd ..
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_format...
cd simplest_ffmpeg_format
cd simplest_ffmpeg_demuxer
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_demuxer_simple
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_muxer
start /wait compile_cl.bat
cd ..
cd simplest_ffmpeg_remuxer
start /wait compile_cl.bat
cd ..
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_video_filter...
cd simplest_ffmpeg_video_filter
cd simplest_ffmpeg_video_filter
start /wait compile_cl.bat
cd ..
cd ..
 
::=====================================================================================
echo.
echo Compile simplest_ffmpeg_swscale...
cd simplest_ffmpeg_swscale
cd simplest_ffmpeg_swscale
start /wait compile_cl.bat
cd ..
cd simplest_pic_gen
start /wait compile_cl.bat
cd ..
cd ..
 
::=====================================================================================
```




## MinGW




![](https://img-blog.csdn.net/20150223150315158)

MinGW是Windows下的GCC/G++编译器。使用MinGW编译需要运行其安装目录下的msys.bat设置其环境变量。一个基本的调用MinGW gcc编译的命令行如下所示：
`gcc helloworld.cpp -o helloworld.exe`上述命令执行完后即可在同一目录下生成一个helloworld.exe的可执行程序。
编译包含类库的程序相对来说要复杂一些，下面以Simplest FFmpeg Player为例看一下它的编译脚本。Simplest FFmpeg Player使用MinGW GCC编译的脚本保存在“compile_mingw.sh”文件中，如下所示。

```
#! /bin/sh
#最简单的基于FFmpeg的视频播放器 2----MinGW命令行编译
#Simplest FFmpeg Player 2----Compile in MinGW
#
#雷霄骅 Lei Xiaohua
#leixiaohua1020@126.com
#中国传媒大学/数字电视技术
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
g++ simplest_ffmpeg_player.cpp -g -o simplest_ffmpeg_player.exe \
-I /usr/local/include -L /usr/local/lib \
-lmingw32 -lSDL2main -lSDL2 -lavformat -lavcodec -lavutil -lswscale
```

这个脚本使用MinGW中的g++完成编译。在这里要注意一点，并不是gcc只能编译c代码，g++只能编译c++代码，而是gcc和g++都可以编译C和C++代码。源代码文件后缀为.c的，gcc把它当作是C程序，而g++当作是c++程序；后缀为.cpp的，两者都会认为是c++程序。上述编译命令有以下几点需要注意：
(a)  include目录设置为/usr/local/include，要确保SDL2和FFmpeg的头文件都安装在这个目录里（SDL2有可能没有安装在这里）
(b)  lib目录设置为/usr/local/lib，要确保SDL2和FFmpeg的库文件都安装在这个目录里（SDL2有可能没有安装在这里）
(c)  使用SDL类库的时候，务必链接-lmingw32
上述脚本运行完城后，生成simplest_ffmpeg_player.exe。

完整的脚本simplest_ffmpeg_demos_compile_mingw_all.sh如下所示。将脚本拷贝至《最简单的FFmpeg示例程序合集》所在目录运行，就可以编译所有的示例程序。注意这个脚本只是分别调用了各个程序目录下的compile_mingw.sh文件。


```
echo "============================================="
echo "simplest ffmpeg demos list ---- MinGW compile all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="
#=====================================================================================
 
echo ""
echo "Compile simplest_ffmpeg_player..."
cd simplest_ffmpeg_player
cd simplest_ffmpeg_player
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_player_su
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_decoder_pure
sh compile_mingw.sh
cd ..
cd ..
 
#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_audio_player..."
cd simplest_ffmpeg_audio_player
cd simplest_ffmpeg_audio_player
sh compile_mingw.sh
cd ..
cd ..
 
#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_picture_encoder..."
cd simplest_ffmpeg_picture_encoder
cd simplest_ffmpeg_picture_encoder
sh compile_mingw.sh
cd ..
cd ..
 
#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_video_encoder..."
cd simplest_ffmpeg_video_encoder
cd simplest_ffmpeg_video_encoder
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_video_encoder_pure
sh compile_mingw.sh
cd ..
cd ..
 
#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_audio_encoder..."
cd simplest_ffmpeg_audio_encoder
cd simplest_ffmpeg_audio_encoder
sh compile_mingw.sh
cd ..
cd ..
 
#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_streamer..."
cd simplest_ffmpeg_streamer
cd simplest_ffmpeg_streamer
sh compile_mingw.sh
cd ..
cd ..
 
#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_mem_handler..."
cd simplest_ffmpeg_mem_handler
cd simplest_ffmpeg_mem_player
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_mem_transcoder
sh compile_mingw.sh
cd ..
cd ..
 
#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_device..."
cd simplest_ffmpeg_device
cd simplest_ffmpeg_grabdesktop
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_readcamera
sh compile_mingw.sh
cd ..
cd ..
 
#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_format..."
cd simplest_ffmpeg_format
cd simplest_ffmpeg_demuxer
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_demuxer_simple
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_muxer
sh compile_mingw.sh
cd ..
cd simplest_ffmpeg_remuxer
sh compile_mingw.sh
cd ..
cd ..
 
#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_video_filter..."
cd simplest_ffmpeg_video_filter
cd simplest_ffmpeg_video_filter
sh compile_mingw.sh
cd ..
cd ..
 
#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_swscale..."
cd simplest_ffmpeg_swscale
cd simplest_ffmpeg_swscale
sh compile_mingw.sh
cd ..
cd simplest_pic_gen
sh compile_mingw.sh
cd ..
cd ..
 
#=====================================================================================
```



## GCC (Linux/MacOS)




![](https://img-blog.csdn.net/20150223150313008)![](https://img-blog.csdn.net/20150223150318484)![](https://img-blog.csdn.net/20150223150340711)

GCC是Linux/MacOS下的C/C++编译器。在Linux/MacOS下可以使用GCC编译C/C++程序，一个简单的GCC编译命令：
`gcc helloworld.cpp -o helloworld.out`上述命令执行完后即可在同一目录下生成一个名称为helloworld.out的可执行程序。
编译包含类库的程序相对来说要复杂一些，下面以Simplest FFmpeg Player为例看一下它的编译脚本。Simplest FFmpeg Player使用GCC编译的脚本保存在“compile_gcc.sh”文件中，如下所示。

```
#! /bin/sh
#最简单的基于FFmpeg的视频播放器 2----命令行编译
#Simplest FFmpeg Player 2----Compile in Shell
#
#雷霄骅 Lei Xiaohua
#leixiaohua1020@126.com
#中国传媒大学/数字电视技术
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
gcc simplest_ffmpeg_player.cpp -g -o simplest_ffmpeg_player.out \
-I /usr/local/include -L /usr/local/lib -lSDL2main -lSDL2 -lavformat -lavcodec -lavutil -lswscale
```

这个脚本使用gcc完成编译。上文中已经提到过一次，并不是gcc只能编译c代码，g++只能编译c++代码，而是gcc和g++都可以编译C和C++代码。源代码文件后缀为.c的，gcc把它当作是C程序，而g++当作是c++程序；后缀为.cpp的，两者都会认为是c++程序。上述编译命令有以下几点需要注意：
(a)  include目录设置为/usr/local/include，要确保SDL2和FFmpeg的头文件都安装在这个目录里
(b)  lib目录设置为/usr/local/lib，要确保SDL2和FFmpeg的库文件都安装在这个目录里
(c)  有些版本中的Linux可能没有安装gcc/g++编译器，需要先装好这两个编译器
上述脚本运行完城后，生成simplest_ffmpeg_player.out。


### 注意事项：Linux和MacOS的不同
Linux和MacOS在SDL1.2的使用上有很大的不同。在MacOS下使用SDL1.2必须加上“-framework Cocoa”参数，否则编译会出现错误。因此在MacOS下编译使用SDL1.2的程序的时候，编译命令如下所示（以simplest_ffmpeg_grabdesktop为例，该程序采用了SDL1.2）。

```
gcc simplest_ffmpeg_grabdesktop.cpp -g -o simplest_ffmpeg_grabdesktop.out \
-framework Cocoa -I /usr/local/include -L /usr/local/lib -lSDLmain -lSDL -lavformat -lavcodec -lavutil -lavdevice -lswscale
```
对于这些采用SDL1.2的程序，保存了一个MacOS下专有的脚本“compile_gcc_mac.sh”。
此外，Linux和MacOS在显示上也有较大的不同。此前发现Windows和MacOS下可以正常显示的程序在Ubuntu下却会出现“绿屏”现象。不过随着代码的调整这一情况已经被消除了。

完整的脚本simplest_ffmpeg_demos_compile_gcc_all.sh如下所示。将脚本拷贝至《最简单的FFmpeg示例程序合集》所在目录运行，就可以编译所有的示例程序。注意这个脚本只是分别调用了各个程序目录下的compile_gcc.sh文件。

```
echo "============================================="
echo "simplest ffmpeg demos list ---- GCC compile all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="
#=====================================================================================
#OS kernel
kernel=$(uname -s)

#change the access permissions (--recursive)
chmod -R 777 ./

echo ""
echo "Compile simplest_ffmpeg_player..."
cd simplest_ffmpeg_player
cd simplest_ffmpeg_player
sh compile_gcc.sh
cd ..
cd simplest_ffmpeg_player_su
sh compile_gcc.sh
cd ..
cd simplest_ffmpeg_decoder_pure
sh compile_gcc.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_audio_player..."
cd simplest_ffmpeg_audio_player
cd simplest_ffmpeg_audio_player
sh compile_gcc.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_picture_encoder..."
cd simplest_ffmpeg_picture_encoder
cd simplest_ffmpeg_picture_encoder
sh compile_gcc.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_video_encoder..."
cd simplest_ffmpeg_video_encoder
cd simplest_ffmpeg_video_encoder
sh compile_gcc.sh
cd ..
cd simplest_ffmpeg_video_encoder_pure
sh compile_gcc.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_audio_encoder..."
cd simplest_ffmpeg_audio_encoder
cd simplest_ffmpeg_audio_encoder
sh compile_gcc.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_streamer..."
cd simplest_ffmpeg_streamer
cd simplest_ffmpeg_streamer
sh compile_gcc.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_mem_handler..."
cd simplest_ffmpeg_mem_handler
cd simplest_ffmpeg_mem_player
if [ "$kernel" == "Darwin" ];then
sh compile_gcc_mac.sh
else
sh compile_gcc.sh
fi
cd ..
cd simplest_ffmpeg_mem_transcoder
sh compile_gcc.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_device..."
cd simplest_ffmpeg_device
cd simplest_ffmpeg_grabdesktop
if [ "$kernel" == "Darwin" ];then
sh compile_gcc_mac.sh
else
sh compile_gcc.sh
fi
cd ..
cd simplest_ffmpeg_readcamera
if [ "$kernel" == "Darwin" ];then
sh compile_gcc_mac.sh
else
sh compile_gcc.sh
fi
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_format..."
cd simplest_ffmpeg_format
cd simplest_ffmpeg_demuxer
sh compile_gcc.sh
cd ..
cd simplest_ffmpeg_demuxer_simple
sh compile_gcc.sh
cd ..
cd simplest_ffmpeg_muxer
sh compile_gcc.sh
cd ..
cd simplest_ffmpeg_remuxer
sh compile_gcc.sh
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_video_filter..."
cd simplest_ffmpeg_video_filter
cd simplest_ffmpeg_video_filter
if [ "$kernel" == "Darwin" ];then
sh compile_gcc_mac.sh
else
sh compile_gcc.sh
fi
cd ..
cd ..

#=====================================================================================
echo ""
echo "Compile simplest_ffmpeg_swscale..."
cd simplest_ffmpeg_swscale
cd simplest_ffmpeg_swscale
sh compile_gcc.sh
cd ..
cd simplest_pic_gen
sh compile_gcc.sh
cd ..
cd ..

#=====================================================================================
```









