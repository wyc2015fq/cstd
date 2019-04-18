# cmake使用总结(一）---工程主目录CMakeList文件编写 - gauss的专栏 - CSDN博客
2015年05月17日 22:39:36[gauss](https://me.csdn.net/mathlmx)阅读数：3515
在linux 下进行开发很多人选择编写makefile文件进行项目环境搭建，而makefile 文件依赖关系复杂，工作量很大，搞的人头很大。采用自动化的项目构建工具cmake可以将程序员从复杂的makefile 文件中解脱出来。cmake 根据内置的规则和语法来自动生成相关的makefile文件进行编译，同时还支持静态库和动态库的构建，我把工作中用到的东东总结在此，方便忘记时随时查看，具体cmake的介绍和详细语法还是参考官方文档（http://www.cmake.org/），有一篇中文的***cmake实践***写的不错，可以google一下。
使用cmake很简单，只需要执行cmake, make 两个命令即可，用我工作中的一个工程举例说明。
假设当前的项目代码在src 目录。 src下有子目录：server, utility, lib, bin, build
server -----存放项目的主功能类文件
utility----- 存放项目要用到相关库文件，便已成为库文件存放到子目录lib 中
lib -----存放utility 生成的库
bin -----存放association 生成的二进制文件
build -----编译目录，存放编译生成的中间文件
cmake要求工程主目录和所有存放源代码子目录下都要编写CMakeLists.txt 文件，注意大小写（cm 大写，list中l大写且落下s).
src/CMakeLists.txt文件如下：
-------------------------------------------------------------------------------------------------------------
#cmake file for project association #表示注释
#author:>---double__song
#created:>--2011/03/01
CMAKE_MINIMUM_REQUIRED(VERSION 2.8) #cmake 最低版本要求，低于2.6 构建过程会被终止。
PROJECT(server_project) #定义工程名称
MESSAGE(STATUS "Project: SERVER") #打印相关消息消息
MESSAGE(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")
SET(CMAKE_BUILE_TYPE DEBUG) #指定编译类型
SET(CMAKE_C_FLAGS_DEBUG "-g -Wall") #指定编译器
ADD_SUBDIRECTORY(utility) #添加子目录
ADD_SUBDIRECTORY(server)
-------------------------------------------------------------------------------------------------------------
相关解释：
1. CMakeLists.txt 文件中不区分大小写
2. PROJECT(project_name) 定义工程名称
语法：project(projectname [cxx] [c] [java])
可以指定工程采用的语言，选项分别表示：C++, C, java， 如不指定默认支持所有语言
3. MESSAGE（STATUS, "Content") 打印相关消息
输出消息，供调试CMakeLists.txt 文件使用。
4. SET(CMAKE_BUILE_TYPE DEBUG) 设置编译类型debug 或者release。 debug版会生成相关调试信息，可以使用GDB 进行
调试；release不会生成调试信息。当无法进行调试时查看此处是否设置为debug.
5. SET(CMAKE_C_FLAGS_DEBUG "-g -Wall") 设置编译器的类型
CMAKE_C_FLAGS_DEBUG ---- C 编译器
CMAKE_CXX_FLAGS_DEBUG ---- C++ 编译器
6. ADD_SUBDIRECTORY(utility) 添加要编译的子目录
为工程主目录下的存放源代码的子目录使用该命令，各子目录出现的顺序随意。
如上便是工程server_project 主目录src 下的CMakeLists.txt文件，下一篇我们解释子目录utiltiy中的CMakeLists.txt 文件。
