# VS2013配置编译Caffe-Win10_X64 - wishchinYang的专栏 - CSDN博客
2015年04月18日 22:14:42[wishchin](https://me.csdn.net/wishchin)阅读数：5561
 原文链接：[http://blog.csdn.net/joshua_1988/article/details/45036993](http://blog.csdn.net/joshua_1988/article/details/45036993)
        有少量修改................
        2014年4月的时候自己在公司就将Caffe移植到Windows系统了，今年自己换了台电脑，想在家里也随便跑跑，本来也装了Ubuntu可以很方便的配置好，无奈在家的风格是“娱乐的时候抽空学习”，所以移植到Windows还是很有必要的。但是，公司禁止将公司内部资料带出，很多地方又都忘记了，周末磨了一天终于移植完，本篇为记录将Caffe移植至Windows7 x64系统下的一些关键步骤。第一步先看看这老些依赖库怎么搞。
在真正开始编译各依赖库之前，需要准备一些必备工具：
        首先当然是VS2013，下载地址：[https://www.visualstudio.com/](https://www.visualstudio.com/)
        其次是 CMake工具，下载地址：[http://www.cmake.org/download/](http://www.cmake.org/download/)
**        （CMake要使用最新版本，否则无法识别 Dev12......）**
**1．Boost**
下载地址：[http://www.boost.org/](http://www.boost.org/)
编译方法：
> 
1) 运行Visual Studio Tools中的VS2013 x64 Cross Tools Command Prompt终端工具
2) 从终端进入boost库所在目录，如D:\LIBS\boost_1_57_0
3) 运行bootstrap.bat生产64位的bjam.exe
4) 输入命令进行编译，更正一下，msvc-12.0才是vs2013哈，感谢halooooJeffrey [http://weibo.com/2211847364](http://weibo.com/2211847364)
静态库：`bjam --build-type=complete ~~toolset=msvc-9.0~~ toolset=msvc-12.0 threading=multi link=static address-model=64`
共享库：`bjam --build-type=complete ~~toolset=msvc-9.0~~ toolset=msvc-12.0 threading=multi link=shared address-model=64`
```
（**使用Bjam编译还是比较蛋疼的，直接在GIt上下载EXE安装版本就好了......**）
```
```
```
**2．OpenCV**
下载地址：[http://opencv.org/downloads.html](http://opencv.org/downloads.html)  本文中使用的是2.4.10版本
编译方法：
> 
下载后的安装包中有已编译好的库，可直接引用，如D:\LIBS\opencv\build\x64\vc12
（**如果不是使用Linux，还是直接安装就好，不要自己编译，很麻烦.....**）
**3．OpenBlas**
下载地址：[http://sourceforge.net/projects/openblas/files/](http://sourceforge.net/projects/openblas/files/)
编译方法：
> 
OpenBlas库在windows上编译起来比较复杂，这里给出的下载地址是一个已编译好的压缩包[OpenBLAS-v0.2.14-Win32.zip (12.1 MB)](http://sourceforge.net/projects/openblas/files/latest/download?source=files)，直接提供了./bin ./include ./lib路径...(赞 原作者...)
**4．CUDA**
下载地址：[https://developer.nvidia.com/cuda-downloads](https://developer.nvidia.com/cuda-downloads)
编译方法：
> 
与OpenCV类似，安装好后直接有已编译好的库。如C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.0\include和C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.0\lib\x64
以上是依赖库比较常用，最好将该它们的include和lib加入到Windows的环境变量中，方便后续的库的编译
**5．GFlags**
下载地址：[https://github.com/gflags/gflags](https://github.com/gflags/gflags)
编译方法：
> 
1) 启动CMake(cmake-gui)工具
2) 设置source code路径和build路径
![](https://img-blog.csdn.net/20150414191717633)
3) 单击Configure按钮，并选择并选择Visual Studio 12 2013 Win64编译器编译器
![](https://img-blog.csdn.net/20150414191748646)
4) 更新完成后，勾选中BUILD_SHARED_LIBS和BUILD_STATIC_LIBS
![](https://img-blog.csdn.net/20150414191735171)
5) 单击Generate按钮，生成VS工程
![](https://img-blog.csdn.net/20150414191752783)
6) 打开刚刚生成的VS工程，build其中的ALL_BUILD工程，注意选择x64模式，并分别生成Debug和Release下的库
![](https://img-blog.csdn.net/20150414191853542)
7) 编译成功后，在工程路径下会生成bin、include、lib三个文件夹
![](https://img-blog.csdn.net/20150414191814655)
**6．GLog**
下载地址：[https://github.com/google/glog](https://github.com/google/glog)
编译方法：
> 
该工程中包含VS工程[google-glog.sln](https://github.com/google/glog/blob/master/google-glog.sln)，打开直接编译即可，同样注意Solution Platform选择x64模式，并分别生成Debug和Release下的库
**7．LevelDB**
下载地址：[https://github.com/bureau14/leveldb](https://github.com/bureau14/leveldb)
> 
这里没有选择官方的[https://github.com/google/leveldb](https://github.com/google/leveldb)是由于官方版本移除了CMake的依赖项，自己写起来比较复杂
编译方法：
> 
与编译GFlags方法类似，唯一需要注意的地方是将CMakeLists.txt中第82行的-DSNAPPY注释掉，否则需要依赖Snappy库，其实并不绝对需要，为了简单起见将此库去掉。另外**Leveldb   依赖于 boost****  库**，如果没有将boost库添加至环境变量，可能需要手动进行设置。
**8．LMDB**
下载地址：[https://gitorious.org/mdb/mdb/archive/462dc097451834477b597447af69c5acc93182b7.tar.gz](https://gitorious.org/mdb/mdb/archive/462dc097451834477b597447af69c5acc93182b7.tar.gz)
编译方法：
> 
1) 解压压缩包到某路径，例如D:\CODE\CXX\mdb-mdb
2) 在VS2013中新建工程，FILE --> New --> Project From Existing Code..
![](https://img-blog.csdn.net/20150414192056673)
3) 选取源码所在路径，并给工程起名
![](https://img-blog.csdn.net/20150414192110573)
4) 单击next按钮后选择Project type为Console application project
![](https://img-blog.csdn.net/20150414192123287)
5) 将Solution Platform修改为x64模式
6) 注意将工程的输出改为静态库，右键单击工程 --> property --> Configuration Properties --> General --> Project Default --> Configureation Type --> Static library (.lib)
7) 其中一个.c文件中包含了unistd.h，为了解决这个问题需要准备三个文件[unistd.h](https://drive.google.com/file/d/0B_G5BUend20PVmFQQkJyT1V2b2M/view?usp=sharing)、[getopt.h](https://drive.google.com/file/d/0B_G5BUend20PekJ0aHBQMGRJRFU/view?usp=sharing)、[getopt.c](https://drive.google.com/file/d/0B_G5BUend20PWHpva1pEU1JuelU/view?usp=sharing)。unistd.h可以考[http://stackoverflow.com/questions/341817/is-there-a-replacement-for-unistd-h-for-windows-visual-c](http://stackoverflow.com/questions/341817/is-there-a-replacement-for-unistd-h-for-windows-visual-c)解决。另外两个可以从[http://ieng6.ucsd.edu/~cs12x/vc08install/getopt9.zip](http://ieng6.ucsd.edu/~cs12x/vc08install/getopt9.zip)下载
8) 最后编译即可（若是 添加unistd.h 之后 仍然显示无法找到unistd.h，重启一下电脑就可以找到了！！！  还是不行？？？？
在linux下编译的程序必须包含  **unistd.h文件**，但是VC里面是没有的，所以必须写一个到 VC的包含目录...
**所以必须把这个头文件加入到VS2012的头文件库里面，其它的编译器也是如此，对于VS2012来说只要在你的默认库文件夹下（我的电脑是C:\Program Files(X86)\Microsoft Visual Studio 12.0\VC\include）添加一个unistd.h文件即可。然后再把那两个Getopt文件也添加进去...**    成功！）
**9．ProtoBuf**
下载地址：[https://github.com/google/protobuf](https://github.com/google/protobuf)
编译方法：
> 
压缩包里有一个叫vsprojects的文件夹，其中有现成的VS工程，可以用来直接编译，也需要注意将Solution Platform修改为x64模式
[( 对于： error C1041: 无法打开程序数据库vc120.pdb](http://www.hankcs.com/acg/game/to-solve-the-cocos2d-x-3-2-error-c1041-cannot-open-the-program-database-vc120-pdb.html) 解决方法如链接所示：在么一个工程 出现[Visual
 Studio 2013 fatal error C1041 /FS](http://stackoverflow.com/questions/20643370/visual-studio-2013-fatal-error-c1041-fs) 解决方案是为单个项目添加 /FS (Force Synchronous PDB Writes) 编译选项，
若是多个，参考：[http://www.hankcs.com/acg/game/to-solve-the-cocos2d-x-3-2-error-c1041-cannot-open-the-program-database-vc120-pdb.html](http://www.hankcs.com/acg/game/to-solve-the-cocos2d-x-3-2-error-c1041-cannot-open-the-program-database-vc120-pdb.html)
**在C:\Tools\protobuf-master\src\google\protobuf\compiler\mock_code_generator.cc  出现  #include <gtest/gtest.h> 找不到的现象！为什么？**）
（参考：http://stackoverflow.com/questions/10945386/cannot-open-include-file-gtest-h-no-such-file-or-directory
   必须 编译安装gtest，完成测试功能...）
**10．HDF5**
下载地址：[http://www.hdfgroup.org/ftp/HDF5/current/src/CMake/hdf518-CMakeWindows.zip](http://www.hdfgroup.org/ftp/HDF5/current/src/CMake/hdf518-CMakeWindows.zip)
编译方法：
> 
解压后，在VS2013 x64 Cross Tools Command Prompt终端工具中运行build-VS2013-64.bat即可。
（CMake Error at HDF518CMake.cmake:6 (cmake_minimum_required):
  CMake 3.1.0 or higher is required.  You are running version 3.0.2 ，看来Cmake又得更新了...
 编译结果出现failed！不知道运行起来怎么样？？？43% Test Failed，1129 of 1975 failed！
）
**如果直接安装可以使用的话，就不要自己编译HDF5了。**
**整理头文件和库文件**
将5-10的头文件和编译后的库统一整理到一个3rdparty文件夹下，其中包含两个文件夹include和lib
![](https://img-blog.csdn.net/20150414192122495)
        include文件夹下包含gflags、glog、google、hdf5、leveldb、lmdb六个文件。gflags的头文件来自于生成的VS工程目录中的include文件夹；glog的头文件来自于VS工程目录中的src\windows文件夹；google中是protobuf的头文件，来自于压缩包中的src\google文件夹；hdf5来自于压缩包中的CMake\hdf5-1.8.14\src文件夹，保留.h文件即可；leveldb的头文件来自于压缩包的include文件夹；lmdb的头文件来自于压缩包中的libraries\liblmdb文件夹，保留.h文件即可
       lib文件夹中的.lib文件直接从编译好的工程目录下拷贝即可，注意debug模式下的.lib文件的文件名修改为xxxd.lib形式
       至此，caffe需要的各项依赖库已经准备完毕，后续会上一些图，看起来直观一些。
11.开始编译Caffe..................
      使用Cmake编译Caffe............
      配置Cmake时使用glog包含，包含**src/Windows**文件夹，若使用src里面修改后缀的/glog/logging.h，则出现@字符串错误，一时难解。
      配置Cmake的包含目录后，在使用VS编译时仍会发现找不到 “google/protobuf/stubs/common.h”文件，挺蛋疼..................
      使用VS编译时，发现caffe.pd.h始终会被修改为 <google/protobuf/stubs/common.h>，即使设定为只读，只要一编译，就会自动修改为原文件，真是醉了！醉了......
      所以，编译失败了...........
出现@字符串错误：把包含的 windows/glog 替换 glog里面的文件；
  编译成功.............................
