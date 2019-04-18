# Cmake的介绍和使用 Cmake实践 - gauss的专栏 - CSDN博客
2015年05月17日 13:08:52[gauss](https://me.csdn.net/mathlmx)阅读数：321
个人分类：[开发工具/调试技巧](https://blog.csdn.net/mathlmx/article/category/1317877)
Cmake优点：
1.开发源代码，实用类BSD许可发布。
2.跨平台，并可以生成native编译配置文件，在linux/unix平台，生成makefile，在mac平台可以生成xcode，在windows平台可以生成msvc工程的配置文件。
3.能够管理大型项目
4.简化编译构建过程和编译过程，只需要cmake+make就可以
5.高效率
6.可扩展，可以为cmake编写特定功能的模块，扩充cmake功能
如何安装cmake
1.Cmake的安装可以使用autotools进行安装，点击[cmake-2.8.6.tar.gz](http://www.cmake.org/files/v2.8/cmake-2.8.6.tar.gz)链接，可以对软件进行下载。
2../configure
3.make
4.sudo make install
Cmake的原理
Helloworld cmake
//main.cpp
#include<cstdio>
int main()
{
    printf("hello world from main\n");
    return 0; 
}
创建CMakeLists.txt（注意大小写一个字母都不能错）
向该文件中加入以下几行（稍后会做解释）
PROJECT (HELLO)
SET(SRC_LIST main.cpp)
MESSAGE(STATUS "This is BINARY dir " ${HELLO_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir "${HELLO_SOURCE_DIR})
ADD_EXECUTABLE(hello ${SRC_LIST})
运行以下命令：
cmake . (别忘记加上这个点，表示当前目录)
![](http://www.cppblog.com/images/cppblog_com/roger/cmake/1.jpg)
注意执行完这句话之后会生成几个文件如下：
![](http://www.cppblog.com/images/cppblog_com/roger/cmake/2.jpg)
CMakeFiles, CMakeCache.txt, cmake_install.cmake等文件，并且生成了Makefile
然后执行make 就可以生成可执行文件hello
![](http://www.cppblog.com/images/cppblog_com/roger/cmake/3.jpg)
这是当前目录下就会生成可执行文件如下图：
![](http://www.cppblog.com/images/cppblog_com/roger/cmake/4.png)
对例子的解释：
CMakeLists.txt的内容如下：
PROJECT (HELLO)
SET(SRC_LIST main.cpp)
MESSAGE(STATUS "This is BINARY dir " ${HELLO_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir "${HELLO_SOURCE_DIR})
ADD_EXECUTABLE(hello ${SRC_LIST})
Project的指令的语法是：
PROJECT(projectname [CXX] [C] [JAVA])
这个执行是用来定义工程的名称的和定义工程支持的语言。这个指令也隐式的定义了两个cmake变量：<projectname>_BINARY_DIR以及<projectname>_BINARY_DIR,这里就是HELLO_BINARY_DIR和HELLO_SOURCE_DIR，两个变量指的都是当前工程的路径。
SET指令的语法：
SET（VAR[VALUE] [CACHE TYPE DOCSTRING [FORCE]]）
Set指令是用来显式的定义变量的，我们之前用到的是SET(SRC_LIST main.cpp)如果有多个源文件，也可以定义成SET(SRC_LIST main.cpp t1.cpp t2.cpp)。
MESSAGE指令的语法是：
MESSAGE([SEND_ERROR | STATUS | FATAL_ERROR] "message to display" ...)
这个指令用于向终端输出用户信息，包含三种类型：
SEND_ERROR，产生错误，生成过程被跳过。
SATUS,输出前缀为-的信息。
FATAL_ERROR，立即终止所有cmake过程。
我们在这里使用的是STATUS信息输出，显示了由PROJECT指令顶一顶两个饮食变量HELLO_BINARY_DIR和HELLO_SOURCE_DIR。
ADD_EXECUTABLE(hello ${SRC_LIST})
定义了这个工程会生成一个文件名为hello的可执行文件，相关的源文件是SRC_LIST中定义的源文件列表，本例中你可以直接写成ADD_EXECUTABLE(hello main.c)。
将本例改写成一个最简化的CMakeLists.txt：
PROJECT(HELLO)
ADD_EXECUTABLE(hello main.c)
下面我们介绍一个比较实用的例子，即包含生成静态库又包含引入外部头文件和链接库的cmake demo。
先按照工程规范建立工程目录，并编写代码，以下面的工程目录为例进行解释这个例子,工程的目录结构为：
![](http://www.cppblog.com/images/cppblog_com/roger/cmake/5.jpg)
编译工程要实现的目标：
1. 添加子目录doc，用以放置这个工程的文档hello.txt
2. 生成hello的静态库，并在main可执行程序链接hello静态库
3. 在这个工程中添加COPYRIGHT,README
4. 在工程目录中添加一个run.sh的脚本，用以调用生成的二进制可执行文件
5. 将生成的二进制文件生成到bin子目录中
6. 编写安装程序
1. 编写CMakeLists.txt
由于一个工程目录中包含多个项目，其中在此项目中包含util项目和main项目，其中util项目是用以生成main程序需要的静态库，main是用以生成可执行文件。
在工程项目中的父目录向有一个CMakeLists.txt是用以声明定义工程需要的Cmake设置还定义了子目录src，用以递归的调用src中的MakeLists.txt。其中工程目录的CMakeLists.txt内容定义如下：
PROJECT(HELLO)
ADD_SUBDIRECTORY(src)
在src里面的CMakeLists.txt是用以定义src目录包含的两个工程的依赖关系分别进行编译。
util目录里面的CMakeLists.txt是用以定义生成util静态库的规则，其中内容如下：
SET(LIBRARY_OUTPUT_PATH ${HELLO_SOURCE_DIR}/lib)
SET(CMAKE_C_COMPILER g++)
SET(SRC_LIST hello.c)
INCLUDE_DIRECTORIES(${HELLO_SOURCE_DIR}/include)
ADD_LIBRARY(util STATIC ${SRC_LIST})
其中SET(LIBRARY_OUTPUT_PATH ${HELLO_SOURCE_DIR}/lib)定义了库生成的路径，LIBRARY_OUTPUT_PATH是一个内部变量，存放库生成路径。
SET(SRC_LIST hello.c)是用来定义库文件需要的源文件。
INCLUDE_DIRECTORIES(${HELLO_SOURCE_DIR}/include)是用来定义非标准库头文件要搜索的路径。其中INCLUDE_DIRECTORIES命令的格式为：
INCLUDE_DIRECTORIES([AFTER|BEFORE] [SYSTEM] dir1 dir2 ...)
ADD_LIBRARY(util STATIC ${SRC_LIST})是用来定义生成的库的名字，以及生成库的类型和生成库需要的源文件，其中ADD_LIBRARY命令格式为：
     ADD_LIBRARY(libname    [SHARED|STATIC|MODULE]
          [EXCLUDE_FROM_ALL]
                source1 source2 ... sourceN)
SET(CMAKE_C_COMPILER g++)是用来定义c的编译器为g++，防止出现C和C++代码在不指定C编译器的情况下默认使用gcc，导致系统编译混乱。
在main目录中的CMakeLists.txt是用来定义可执行程序编译和链接时所需要的一些命令或环境。内容如下：
SET(EXECUTABLE_OUTPUT_PATH ${HELLO_SOURCE_DIR}/bin)
SET(SRC_LIST main.cpp)
INCLUDE_DIRECTORIES(${HELLO_SOURCE_DIR}/include)
LINK_DIRECTORIES(${HELLO_SOURCE_DIR}/lib)
ADD_EXECUTABLE(hello ${SRC_LIST})
TARGET_LINK_LIBRARIES(hello util)
INCLUDE_DIRECTORIES命令是定义工程的include文件夹，其中存放使用到的库的头文件，LINK_DIRECTORIES是定义工程的库文件，其中存放着库文件，ADD_EXECUTABLE是定义生成的可执行文件，TARGET_LINK_LIBRARIES用以定义链接时需要的库文件。
2．在工程目录下创建build目录，并采用out-of-source方式编译项目。执行命令make ..，执行结果如下：
![](http://www.cppblog.com/images/cppblog_com/roger/cmake/6.jpg)
执行make，这时在build目录下生成了中间编译文件：
![](http://www.cppblog.com/images/cppblog_com/roger/cmake/7.jpg)
执行make命令，结果如下：
![](http://www.cppblog.com/images/cppblog_com/roger/cmake/8.jpg)
可以看到工程创建和编译成功了。
2. 安装
在工程目录下添加COPYRIGHT、README、和run.sh，重新编辑工程目录下的CMakeLists.txt。在CMakeLists.txt中添加如下命令：
INSTALL(FILES COPYRIGHT README DESTINATION share/doc/cmake_demo)
INSTALL(PROGRAMS run.sh DESTINATION bin)
INSTALL(PROGRAMS bin/hello DESTINATION bin)
INSTALL(DIRECTORY doc/ DESTINATION share/doc/cmake_demo)
这些命令表示在执行make install命令时，安装程序会拷贝相应的文件、目录或程序到指定的前缀开始的目录中，cmake执行命令如下：
cmake -DCMAKE_INSTALL_PREFIX=~/data/cmake_demo ..这时将工程目录安装到~/data/cmake_demo目录下。执行结果如下：
![](http://www.cppblog.com/images/cppblog_com/roger/cmake/9.jpg)
其中cmake编译c、c++工程完毕
**常用命令**
- **project **设置工程的名字。语法：project(<projectname> [languageName1 languageName2 ... ] )。注意这条指令还隐式定义了另外两个变量<projectName>_BINARY_DIR 和<projectName>_SOURCE_DIR，并分别赋值。
- **add_executable **添加一个可执行文件到工程中，并指定所需的源文件。语法：add_executable(<name> [WIN32] [MACOSX_BUNDLE]                 [EXCLUDE_FROM_ALL] source1 source2 ... sourceN)
- **add_definitions **添加-D 选项到编译器，原则上可添加任意选项，但他一开始是设计为添加预处理宏定义的。语法：add_definitions(-DFOO -DBAR ...) 
- **add_library** 添加一个库到工程中，并指定所需的源文件。语法：add_library(<name> [STATIC | SHARED | MODULE]
              [EXCLUDE_FROM_ALL] source1 source2 ... sourceN)注意<name>的命名形式：mingw下为lib<name>.a; msvc下为<name>.lib**此命令还有另一种使用形式**：添加外部库到工程中。语法：add_library(<name> <SHARED|STATIC|MODULE|UNKNOWN> IMPORTED)IMPORTED 表明此库在工程之外，是target_link_libraries的方便形式。外部库的详细信息通过set_target_properties设置以IMPORTED_开头的属性来完成，其中最重要的就是 IMPORTED_LOCATION 属性，它指定外部库的位置。例如：add_library(boost_system STATIC IMPORTED)set_target_properties(boost_system PROPERTIES IMPORTED_LOCATION /path/to/libboost_system.a)target_link_libraries(huang boost_system)等价于：target_link_libraries(huang /path/to/libboost_system.a)
- **add_subdirectory** 添加子目录。语法：add_subdirectory(source_dir [binary_dir] 
                   [EXCLUDE_FROM_ALL])
- **set **设置CMAKE变量值。语法：set(<variable> <value>
      [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])
- **set_property** 设置属性。语法：set_property(<GLOBAL |
                DIRECTORY [dir]                   |
                TARGET    [target1 [target2 ...]] |
                SOURCE    [src1 [src2 ...]]       |
                TEST      [test1 [test2 ...]]     |
                CACHE     [entry1 [entry2 ...]]>
               [APPEND]
               PROPERTY <name> [value1 [value2 ...]])
- **set_target_properties** 设置target的属性。语法：set_target_properties(target1 target2 ...
                        PROPERTIES prop1 value1
                        prop2 value2 ...)
- **set_source_files_properties** 设置源文件的属性。语法：set_source_files_properties([file1 [file2 [...]]]
                              PROPERTIES prop1 value1
                              [prop2 value2 [...]])
- **set_directory_properties** 设置目录的属性。语法：set_directory_properties(PROPERTIES prop1 value1 prop2 value2)
- **message **显式一条消息。语法：message([STATUS|WARNING|AUTHOR_WARNING|FATAL_ERROR|SEND_ERROR]
          "message to display" ...)
- **include_directories** 添加头文件搜索路径，相当于gcc命令行参数 -I。语法：include_directories([AFTER|BEFORE] [SYSTEM] dir1 dir2 ...)。
- **link_directories** 指定一个目录，链接时会自动搜索此目录下的库文件，相当于gcc命令行参数 -L。语法：link_directories(directory1 directory2 ...)需要注意的是此命令只能应用于在它之后生成的target，即先执行link_directories再执行add_executable(target,...)才会对target有效果。
- **target_link_libraries** 将target与给定的动态库/静态库进行链接，相当于gcc命令行参数 -l。语法：target_link_libraries(<target> [item1 [item2 [...]]]
[[debug|optimized|general] <item>] ...)。注意item可使用绝对路径。参考：**add_library**
