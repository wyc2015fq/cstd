# zlib、libzip、 libzippp 库编译(windows + cmake + vs2013) - DoubleLi - 博客园






"libzipp" 这库是基于 "libzip" 之上封装的，而 "libzip" 又是基于 "zlib"库封装的，所以要编译 "libzipp" 库就要先编译其他两个库。
下载准备：
"[zlib-1.2.8.tar.gz](http://download.csdn.net/detail/u014260855/9409944)"
"[libzip-1.0.1.tar.gz](http://download.csdn.net/detail/u014260855/9409958)"
"[libzippp-master.zip](http://download.csdn.net/detail/u014260855/9409969)"




编译环境：cmake + vs2013 ,   安装好cmake和 "VS2013 x86 本机工具命令提示" 工具（安装vs2013的时候已经有）
命令行环境使用的是[VS2013 x86 本机工具命令提示]，“开始”->"Visual Studio 2013"->"Visual Studio Tools"->"VS2013 x86 本机工具命令提示"


以下是本次编译过程，记录详细流程以备忘：


一、编译 zlib 库
1、将解压出来的 "zlib-1.2.8" 文件夹拷贝到 "E:\BuildWorkspace\" ;
2、命令行进入此路径，命令：[mkdir build] （创建 build 文件夹） ;
3、进入刚创建的"build"文件夹，命令：[cmake .. -G"Visual Studio 9 2008" -DCMAKE_INSTALL_PREFIX="install"]  (构建工程)
4、在"build"目录下就有了工程文件，打开工程，编译"ALL_BUILD"项目生成库文件，编译"INSTALL"项目，会执行工程目录下的"cmake_install.cmake"脚本，最终在"build"目录下生成"install"文件夹，头文件、库文件等都在文件夹内了。


二、编译 libzip 库
1、将解压出来的"libzip-1.0.1"文件夹拷贝到"E:\BuildWorkspace\" ;
2、命令行进入此路径，命令：[mkdir build] （创建 build 文件夹） ;
3、进入刚创建的"build"文件夹，命令：[cmake .. -G"Visual Studio 9 2008" -DCMAKE_PREFIX_PATH="../zlib-1.2.8/build/install"]  (构建工程)
4、在"build"目录下就有了工程文件，打开工程，编译"ALL_BUILD"项目，到这里就报错了(最少我在这里是报错了) ;
5、将"libzippp-master\lib\libzip-1.0.1-windows-patch.zip"的压缩文件解压出来，根据同名文件夹替换掉相应的文件，在执行第4步，OK ;
6、此时如果直接编译"INSTALL"项目，会生成到C盘根目录下"C:/Program Files (x86)/libzip"文件夹中，为了让它生成到"build\install"中，这里需要修改工程目录下的"cmake_install.cmake"脚本文件的第5行：
"set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/libzip")" 改成
"set(CMAKE_INSTALL_PREFIX "E:\BuildWorkspace\libzip-1.0.1\build/install")"，
7、编译"INSTALL"->OK ;


三、编译 libzippp 库
1、将解压出来的"libzippp-master"文件夹拷贝到"E:\BuildWorkspace\" ;(PS:上面编译 libzip 库中的第五步就是在从此文件夹中找的"libzippp-master\lib\libzip-1.0.1-windows-patch.zip")
2、命令行进入此路径，命令：[mkdir build] （创建 build 文件夹） ;
3、为了方便 libzippp 项目的依赖关系，懒得修改，直接按照作者意思，把上面的 "libzip-1.0.1" 和 "zlib-1.2.8" 文件夹整个拷贝到 "E:\BuildWorkspace\libzippp-master\lib\" 目录中（当前，前提是已经完成了上面的编译工作）;
4、进入刚创建的"build"文件夹，命令：[cmake .. -G"Visual Studio 12" -DCMAKE_PREFIX_PATH="lib/zlib-1.2.8/build/install"] （构建工程）
5、在"build"目录下就有了工程文件，打开工程，编译"ALL_BUILD"项目，OK！


PS：实测，用vs2008使用这库会有问题（可能是因为是用vs2013编译的缘故），后来干脆直接使用 libzippp 的类文件，不编译成库使用，OK！









