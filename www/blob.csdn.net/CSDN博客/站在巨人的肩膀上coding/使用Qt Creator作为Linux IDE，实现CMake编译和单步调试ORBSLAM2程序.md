# 使用Qt Creator作为Linux IDE，实现CMake编译和单步调试ORBSLAM2程序 - 站在巨人的肩膀上coding - CSDN博客





2018年04月24日 00:57:48[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：321








尊重作者，支持原创，如需转载，请附上原地址：

[http://blog.csdn.net/libaineu2004/article/details/78448392](http://blog.csdn.net/libaineu2004/article/details/78448392)


# 第一部分：使用Qt Creator作为Linux IDE，实现CMake编译和单步调试

## 一、前期准备

1、安装Linux系统，例如CentOS 7，带桌面，GNOME安装

2、安装qt-opensource-linux-x64-5.9.1.run

3、安装CMake，参考教程：[CentOS 7安装cmake 2.8.12.2](http://blog.csdn.net/libaineu2004/article/details/77098162)




## 二、新建C/C++工程，选择“与Qt库无关”的工程

![](https://img-blog.csdn.net/20171105113804409?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGliYWluZXUyMDA0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Bulid system请选择CMake

![](https://img-blog.csdn.net/20171105113808693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGliYWluZXUyMDA0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

新建工程完毕，会在路径下生成CMakeLists.txt文件。下次需要打开工程时，qtcreator菜单打开文件/工程，选择CMakeLists.txt文件即可。




## 三、如何实现单步调试

方法1（推荐）：工程默认的构建方式是Default，也就是release的意思。需要调整为Debug才能实现单步断点调试。

![](https://img-blog.csdn.net/20171105113812399?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGliYWluZXUyMDA0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

勾选完成Debug之后，请在菜单->Build点击选项“Run CMake”即可。

![](https://img-blog.csdn.net/20171105113815574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGliYWluZXUyMDA0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

方法2（不推荐）：由于CMake默认产生的是Default/release版本。我们可以手动编辑CMakeLists.txt文件，在文件的最后添加： 

set(CMAKE_BUILD_TYPE Debug)，再次构建即可。

![](https://img-blog.csdn.net/20171105113819134?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGliYWluZXUyMDA0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




## 四、如何添加C++/C源文件和头文件？又如何添加第三方依赖库库和头文件？

唯一的方式就是手动修改CMakeLists.txt文件，详情见

[CMake使用示例与整理总结](http://blog.csdn.net/libaineu2004/article/details/78450341)


[CMake处理多源文件目录的方法](http://blog.csdn.net/a794226986/article/details/18616511) 请关注多文件的添加方法aux_source_directory


[多目录工程的CMakeLists.txt编写（自动添加多目录下的文件）](http://blog.csdn.net/ktigerhero3/article/details/70313350)请关注多文件的添加方法aux_source_directory


举例1：





**[html]**[view
 plain](https://blog.csdn.net/libaineu2004/article/details/78448392#)[copy](https://blog.csdn.net/libaineu2004/article/details/78448392#)



- set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -Wall -O0 -Wno-unused-variable -pthread")  
- 
- link_directories(  
-     ${PROJECT_SOURCE_DIR}/lib  
-     /usr/lib64/mysql/  
- )  
- 
- find_library(MYSQL_LIB libmysqlclient.so /usr/lib64/mysql/)  
- IF (NOT MYSQL_LIB)  
-     MESSAGE(FATAL_ERROR "mysqlclient not found")  
- ENDIF(NOT MYSQL_LIB)  
- 
- set(net_srcs   
- base/timestamp.cpp  
- base/countdownlatch.cpp  
- )  


举例2：

set(SRC_LIST main.c hello.c)

add_executable(hello ${SRC_LIST})

这样写set当然没什么问题，但是如果源文件很多，把所有源文件的名字都加进去将是一件烦人的工作。更省事的方法是使用 aux_source_directory 命令，该命令会查找指定目录下的所有源文件，然后将结果存进指定变量名。其语法如下：aux_source_directory(<dir> <variable>)

aux_source_directory(. DIRSRCS)

add_executable(hello ${DIRSRCS})

学习CMake请参考《CMake Practice》这篇文章，旨在指导用户快速使用CMake，如果需要更详细的内容，请通读《CMake Practice》这篇文章。下载路径：[http://sewm.pku.edu.cn/src/paradise/reference/CMake%20Practice.pdf](http://sewm.pku.edu.cn/src/paradise/reference/CMake%20Practice.pdf)





## 五、如何实现远程调试

请访问姊妹篇《使用Qt Creator作为Linux IDE，代替Vim：实现两台Linux电脑远程部署和调试(一台电脑有桌面系统，一台电脑无桌面系统)》，网址是：[http://blog.csdn.net/libaineu2004/article/details/62423830](http://blog.csdn.net/libaineu2004/article/details/62423830)


注意：CMake远程生成的目标路径，默认是/root/xxx。可以手动修改CMakeLists.txt文件更改输出路径，SET(EXECUTABLE_OUTPUT_PATH "***").如此一来开发机和目标机的路径都会有生成结果。我的实测结果是，貌似路径里面有home目录，目标机就不行？例如SET(EXECUTABLE_OUTPUT_PATH
 "/home/12/")，结果文件开发机在，但是目标机并不在，而是会在根目录生成/12的文件夹。再比如"/home/firecat/test"目标机也不在，而是根目录下生成"test"文件夹。





**[html]**[view
 plain](https://blog.csdn.net/libaineu2004/article/details/78448392#)[copy](https://blog.csdn.net/libaineu2004/article/details/78448392#)



- cmake_minimum_required(VERSION 2.8)  
- 
- project(untitled)  
- add_executable(${PROJECT_NAME} "main.cpp" "test.cpp")  
- ##SET(EXECUTABLE_OUTPUT_PATH "/home/firecat/test/")  
- SET(EXECUTABLE_OUTPUT_PATH "/hellotest/12/34")  


第二部分：使用QT调试CMake工程ORBSLAM2

//TODO









