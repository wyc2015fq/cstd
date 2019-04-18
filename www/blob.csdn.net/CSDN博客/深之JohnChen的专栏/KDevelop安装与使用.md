# KDevelop安装与使用 - 深之JohnChen的专栏 - CSDN博客

2019年03月02日 00:16:05[byxdaz](https://me.csdn.net/byxdaz)阅读数：551


KDevelop是一个支持多程序设计语言的集成开发环境。它运行于linux和其它类unix环境。

KDevelop本身不包含编译器，而是调用其它编译器来编译程序。

一、KDevelop安装

打开命令窗口:

1. 安装gcc(编译器)

    sudo apt-get build-dep gcc

    sudo apt-get install build-essential

 2. 安装kdevelop

    sudo apt-get install kdevelop

 3. 安装cmake等

   sudo apt-get install automake autoconf g++ libtool cmake

 创建桌面快捷方式：直接从/usr/share/applications/kde4目录下找到KDevelop 4图标，右击，然后Copy To...桌面

即可。

 也可以从终端打开kDevelop软件：打开一个终端，输入：kdevelop

二、KDevelop使用

1、启动kdevelop,新建New Project

2、默认的选择，并填写工程名。

3、默认选择！版本控制，默认选择。

4、添加CMake编译的时候一些配置参数，默认不填写。

5、build进行编译：编译结果。

KDevelop具体使用步骤参考这篇文章:
[https://blog.csdn.net/qq_27806947/article/details/80119294](https://blog.csdn.net/qq_27806947/article/details/80119294)

三、KDevelop下项目编译加入头文件目录、静态库

添加头目录，在CMakeLists.txt文件中增加INCLUDE_DIRECTORIES("头文件目录")

添加库目录，在CMakeLists.txt文件中增加LINK_DIRECTORIES( "库文件目录")

添加静态库文件，在中CMakeLists.txt文件中target_link_libraries(${PROJECT_NAME} 库文件名) 

比如：调用线程函数，需要使用pthread库。

KDevelop3以下的版本，“在"项目->配置->连接器选项"的标签面中，在附加库一栏中填入你所用到的库”或”在link

option中加-pthread

KDevelop4以及以上版本，工程的右击菜单的”打开配置“中，cmake配置页下点”显示高级“，再选上”显示高级变

量“，会出现很多变量，在CMAKE_EXE_LINKER_FLAGS中加入需要的编译参数-lpthread即可。

![](https://img-blog.csdnimg.cn/20190302001434328.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9jaGVuemhlbmd5aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

同时在CMakeLists.txt文件中target_link_libraries(${PROJECT_NAME} ${CMAKE_EXE_LINKER_FLAGS}) 

或者直接在中CMakeLists.txt文件中target_link_libraries(${PROJECT_NAME} pthread) 

四、CMakeLists.txt编写和使用方法

1、使用方法 

一般把CMakeLists.txt文件放在工程目录下，使用时，先创建一个叫build的文件夹（这个并非必须，只

是生成的Makefile等文件放在build里比较整齐），然后执行下列操作： 

cd build 

cmake .. 

make 

其中cmake .. 在build里生成Makefile，make应当在有Makefile的目录下，根据Makefile生成可执行文件。

2、编写方法

```
# 声明要求的cmake最低版本
cmake_minimum_required( VERSION 2.8 )

# 添加c++11标准支持
set( CMAKE_CXX_FLAGS "-std=c++11" )

# 声明一个cmake工程
project( 工程名 )

# 找到后面需要库和头文件的包
find_package（包的名称及最低版本）
# 例如find_package(OpenCV 2.4.3 REQUIRED)

# 头文件
include_directories("路径")
# 例如
#include_directories(
# ${PROJECT_SOURCE_DIR}
# ${PROJECT_SOURCE_DIR}/include
# ${EIGEN3_INCLUDE_DIR}
)

# 设置路径（下面生成共享库的路径）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
# 即生成的共享库在工程文件夹下的lib文件夹中

# 创建共享库（把工程内的cpp文件都创建成共享库文件，方便通过头文件来调用）
add_library(${PROJECT_NAME} SHARED
src/cpp文件名
……
）
# 这时候只需要cpp，不需要有主函数
# ${PROJECT_NAME}是生成的库名 表示生成的共享库文件就叫做 lib工程名.so
# 也可以专门写cmakelists来编译一个没有主函数的程序来生成共享库，供其它程序使用

# 链接库
# 把刚刚生成的${PROJECT_NAME}库和所需的其它库链接起来
target_link_libraries(${PROJECT_NAME}
/usr/lib/i386-linux-gnu/libboost_system.so
)

# 编译主函数，生成可执行文件

# 先设置路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 可执行文件生成
add_executable(要生成的可执行文件名 从工程目录下写起的主函数文件名)

# 这个可执行文件所需的库（一般就是刚刚生成的工程的库咯）
target_link_libraries(可执行文件名 ${PROJECT_NAME})
```

如果添加OpenCV,OpenNI,PCL等库刚需要添加以下

set(OpenCV_DIR "/home/xx/xx")#指定使用哪种版本的OpenCV,必须在project前

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)#自己的cmake模块所在的路径，或者

include( cmake 模块的绝对路径+名称）

find_package（<name1>）#find_package(<Name>)命令首先会在模块路径中寻找 Find<name>.cmake

 find_package (<name2>)

 ...

 include_directories（<name_INCLUDE_DIRS>)##添加相关库的头文件路径

 link_directories（<name_LIB_DIRS>)##添加相关库的库文件路径

target_link_libraries(${PROJECT_NAME} $(name1_LIBS) ...)  #将目标文件与库文件进行链接

这样一个完整的CMakeLists.txt就完成了。

