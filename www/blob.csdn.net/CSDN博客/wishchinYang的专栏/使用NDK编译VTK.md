# 使用NDK编译VTK - wishchinYang的专栏 - CSDN博客
2015年07月03日 11:26:47[wishchin](https://me.csdn.net/wishchin)阅读数：2053
          VTK提供了对安卓的CMAKE编译支持，其介绍文件在源代码根目录下的 “/cmake/android.toolchain.cmake”。
对Wndows的编译自持描述为：
     注意：但是此种编译只能提供对MinGW的支持，不能提供安卓支持！
```
#  Usage Windows:
#     You need native port of make to build your project.
#     Android NDK r7 (and newer) already has make.exe on board.
#     For older NDK you have to install it separately.
#     For example, this one: http://gnuwin32.sourceforge.net/packages/make.htm
#
#   $ SET ANDROID_NDK=C:\absolute\path\to\the\android-ndk
    $ SET ANDROID_NDK=C:\QT\android-ndk-r10
#   $ mkdir build && cd build
#   $ cmake.exe -G"MinGW Makefiles"
#       -DCMAKE_TOOLCHAIN_FILE=path\to\the\android.toolchain.cmake
#       -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%\prebuilt\windows\bin\make.exe" ..
#   $ cmake.exe --build .
#
#
```
**一、使用默认配置选项，使用MinGW编译**
1、确认系统中已经安装   MinGW，选择编译器MinGW MakeFiles ,确认   
![](https://img-blog.csdn.net/20150702095854186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2、确认后选择安卓支持
![](https://img-blog.csdn.net/20150702100132169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
一般情况下会Generate 解决方案成功！
3. 进入MinGW文件夹，打开 bat 文件，运行
![](https://img-blog.csdn.net/20150703111753058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
cd 到生成方案文件夹，运行make命令，即可开始漫长的编译过程，makefile文件里面已经写入自动安装的脚本........................
**二、使用NDK--Unix Makefile进行编译**
ndk-build 其实就是对GNU Make的封装，它的目的是调用正确的NDK编译脚本，它等价于 make -f $NDK/build/core/build-local.mk [参数]
此种方式必须安装cygwin64，安装完成后重启；
在Configure时选择Unix Makefiles
![](https://img-blog.csdn.net/20150703112443558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Configure过程：比较漫长.....................
![](https://img-blog.csdn.net/20150703112531568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择VTK-安卓支持，和NDK文件夹
![](https://img-blog.csdn.net/20150703113025091?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意：图中的NDK目录要修改一下，把Windows风格的路径修改为Unix风格的路径
           /cygdrive/c/QT/android-ndk-r10
![](https://img-blog.csdn.net/20150703113341395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Configuring done
Generating done
运行命令行：
切换到目标目录下：
cd C:\Tools\VTK6.2Android2
make
![](https://img-blog.csdn.net/20150703113716711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
静静 等待编译完成.............................
依然不能生成对安卓支持的库.
**三、启用对安卓的移动支持**
在VTK的源代码根目录下，CMakeLists.txt文件里面，修改
option(VTK_ANDROID_BUILD "Build VTK for Android" OFF)
为
option(VTK_ANDROID_BUILD "Build VTK for Android" ON)
再次配置Cmake，进行编译文件...
**四、使用VES-VTK on OpenGL ES**
参考链接：[http://www.vtk.org/Wiki/VES/Download_And_Build_Instructions#Android_instructions](http://www.vtk.org/Wiki/VES/Download_And_Build_Instructions#Android_instructions)
1.下载Git，安装后直接添加对
[git://vtk.org/VES.git]()
的克隆，到本地文件夹；
2.下载完成后，再次使用Cmake配置编译器，配置——生成；
使用Unix MakeFile 进入下载目录进行编译，make
在make的过程中需要下载Eigen和VTK6.0，容易链接不到VTK.org；建议把下载好的VTK文件移到download文件夹里
3.make.....
结果：
         最终还是未能成功，一直在试错中.....
