# TODO：windows下vs2013+cmake源码安装opencv3.2.0 - 站在巨人的肩膀上coding - CSDN博客





2018年07月18日 14:15:08[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：453








最近开始折腾opencv了，先用它来做一些简单的功能实现，然后再深入算法一探究竟。

当然，天才第一步，环境最先布。

准备：

1、下载opencv3.2.0源码，我们可以在[opencv官网](https://opencv.org/releases.html)上下载，或者直接[百度搜索opencv3.2.0](https://www.baidu.com/s?ie=utf-8&f=8&rsv_bp=1&tn=baidu&wd=opencv3.2&oq=opencv%2526lt%253B.2.0&rsv_pq=c7b553d2000232a9&rsv_t=da4aViLvK9jpd%2B26v6nIncjTOZ8fkuBN2beynxpuWl2EO%2BQjf7%2FfeeytkBk&rqlang=cn&rsv_enter=1&inputT=794&rsv_sug3=15&rsv_sug1=13&rsv_sug7=100&rsv_sug2=0&rsv_sug4=1541)，我都给各位准备好了

2、下载cmake工具，[下载地址](https://cmake.org/download/)，下载最新版3.12就可以

第一步：

1、cmake安装

将下载好的压缩包直接解压到你想放的路径下面

![](https://img-blog.csdn.net/20180718132915956?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0xPVkUxMDU1MjU5NDE1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后将bin目录加入到系统环境变量

控制面板-->系统和安全-->高级设置-->设置环境变量-->系统变量

找到PATH后双击，选择添加 D:\cmake-3.12.0-win32-x86\bin（以D盘为例，添加的是你的对应文件夹的路径，用英文分号;和前面的内容隔开。）

这里不需要重启生效，直接添加完成之后就可以用哦。

![](https://img-blog.csdn.net/20180718133322464?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0xPVkUxMDU1MjU5NDE1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180718133335643?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0xPVkUxMDU1MjU5NDE1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180718133807990?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0xPVkUxMDU1MjU5NDE1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2、opencv安装

2.1 源码安装方式：

上面第一步将Cmake的bin文件夹加入到环境变量生效以后，直接在cmd里面输入cmake-gui然后回车就可以直接打开cmake了。（这里我使用的是Cmder，界面会比windows自带的cmd界面友好一些，而且可以加入右键菜单直接打开，类似于Ubuntu的Terminal,很方便，推荐大家使用，具体安装方法参见

[cmder安装](https://blog.csdn.net/piggy1984/article/details/73743872)  提醒：按照上面的方式将D:\cmder_mini添加到环境变量。

[Cmder 加入右键菜单](http://blog.csdn.net/chris_zhangrx/article/details/78534405)：win+r打开运行之后输入cmder,进入cmder后命令行执行Cmder.exe /REGISTER ALL即成功添加到右键菜单）

![](https://img-blog.csdn.net/20180718140033182?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0xPVkUxMDU1MjU5NDE1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如上图步骤：

1）首先选择source code的路径,就是上面的opencv-3.2.0、

2）手动在opencv-3.2.0里面建一个名字为 build 的文件夹，把 build 文件夹的路径加到这步

3）点击 Configue

4）就会弹出一个选择编译器的界面，选择自己对应的VS版本就行了。

5）点击Finish

文件就开始编译了，第一次编译的话会下载一些依赖的东西，**所以需要多等一会**。会有如下生成：

```
CMake Deprecation Warning at CMakeLists.txt:72 (cmake_policy):
  The OLD behavior for policy CMP0020 will be removed from a future version
  of CMake.

  The cmake-policies(7) manual explains that the OLD behaviors of all
  policies are deprecated and that a policy should be set to OLD only under
  specific short-term circumstances.  Projects should be ported to the NEW
  behavior and not rely on setting a policy to OLD.


CMake Deprecation Warning at CMakeLists.txt:76 (cmake_policy):
  The OLD behavior for policy CMP0022 will be removed from a future version
  of CMake.

  The cmake-policies(7) manual explains that the OLD behaviors of all
  policies are deprecated and that a policy should be set to OLD only under
  specific short-term circumstances.  Projects should be ported to the NEW
  behavior and not rely on setting a policy to OLD.


CMake Deprecation Warning at CMakeLists.txt:81 (cmake_policy):
  The OLD behavior for policy CMP0026 will be removed from a future version
  of CMake.

  The cmake-policies(7) manual explains that the OLD behaviors of all
  policies are deprecated and that a policy should be set to OLD only under
  specific short-term circumstances.  Projects should be ported to the NEW
  behavior and not rely on setting a policy to OLD.


The CXX compiler identification is MSVC 18.0.21005.1
The C compiler identification is MSVC 18.0.21005.1
Check for working CXX compiler: D:/VS2013/VC/bin/cl.exe
Check for working CXX compiler: D:/VS2013/VC/bin/cl.exe -- works
Detecting CXX compiler ABI info
Detecting CXX compiler ABI info - done
Detecting CXX compile features
Detecting CXX compile features - done
Check for working C compiler: D:/VS2013/VC/bin/cl.exe
Check for working C compiler: D:/VS2013/VC/bin/cl.exe -- works
Detecting C compiler ABI info
Detecting C compiler ABI info - done
Detecting C compile features
Detecting C compile features - done
Performing Test HAVE_CXX_FSIGNED_CHAR
Performing Test HAVE_CXX_FSIGNED_CHAR - Failed
Performing Test HAVE_C_FSIGNED_CHAR
Performing Test HAVE_C_FSIGNED_CHAR - Failed
FP16: Compiler support is available
Check if the system is big endian
Searching 16 bit integer
Looking for sys/types.h
Looking for sys/types.h - found
Looking for stdint.h
Looking for stdint.h - found
Looking for stddef.h
Looking for stddef.h - found
Check size of unsigned short
Check size of unsigned short - done
Using unsigned short
Check if the system is big endian - little endian
Looking for fseeko
Looking for fseeko - not found
Looking for unistd.h
Looking for unistd.h - not found
Check size of off64_t
Check size of off64_t - failed
Looking for assert.h
Looking for assert.h - found
Looking for fcntl.h
Looking for fcntl.h - found
Looking for io.h
Looking for io.h - found
Looking for jbg_newlen
Looking for jbg_newlen - not found
Looking for mmap
Looking for mmap - not found
Looking for search.h
Looking for search.h - found
Looking for string.h
Looking for string.h - found
Looking for unistd.h
Looking for unistd.h - not found
Downloading opencv_ffmpeg.dll...
Downloading opencv_ffmpeg.dll... Done
Downloading opencv_ffmpeg_64.dll...
Downloading opencv_ffmpeg_64.dll... Done
Downloading ffmpeg_version.cmake...
Downloading ffmpeg_version.cmake... Done
ICV: Downloading ippicv_windows_20151201.zip...
```

然后就会得到下面的界面：

![](https://img-blog.csdn.net/2018071814131042?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0xPVkUxMDU1MjU5NDE1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里根据自己的需求勾选自己需要的选项或者消去自己不需要的选项就可以了，选择完成后点击 Configure 旁边的 **Generate** ，这个过程很快，最后会得到：

![](https://img-blog.csdn.net/20180718141257376?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0xPVkUxMDU1MjU5NDE1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

后续步骤参考以下链接：[https://blog.csdn.net/chris_zhangrx/article/details/79090463](https://blog.csdn.net/chris_zhangrx/article/details/79090463)



