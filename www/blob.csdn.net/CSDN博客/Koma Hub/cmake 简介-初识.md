# cmake 简介-初识 - Koma Hub - CSDN博客
2019年03月06日 22:16:44[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：22
个人分类：[cmake/make/automake](https://blog.csdn.net/Rong_Toa/article/category/8730065)
**Table of Contents**
[CMake 使用方法](#CMake%20%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95)
[/src/main.c:](#%2Fsrc%2Fmain.c%3A)
[/src/a.c](#%2Fsrc%2Fa.c)
[/include/a.h](#%2Finclude%2Fa.h)
[补充：](#%E8%A1%A5%E5%85%85%EF%BC%9A)
CMake是一个跨平台的安装(编译)工具,可以用简单的语句来描述所有平台的安装(编译过程)。他能够输出各种各样的makefile或者project文件,能[测试](http://lib.csdn.net/base/softwaretest)编译器所支持的C++特性,类似UNIX下的automake。
## CMake 使用方法
    CMake的所有的语句都写在一个叫:CMakeLists.txt的文件中。当CMakeLists.txt文件确定后,可以用ccmake命令对相关 的变量值进行配置。这个命令必须指向CMakeLists.txt所在的目录。配置完成之后,应用cmake命令生成相应的makefile（在Unix like系统下）或者 project文件（指定用window下的相应编程工具编译时）。
    其基本操作流程为：
> - 
$> ccmake directory
- 
$> cmake directory
- 
$> make
  其中directory为CMakeList.txt所在目录；
> - 第一条语句用于配置编译选项，如VTK_DIR目录 ，一般这一步不需要配置，直接执行第二条语句即可，但当出现错误时，这里就需要认为配置了，这一步才真正派上用场；
- 第二条命令用于根据CMakeLists.txt生成Makefile文件；
- 第三条命令用于执行Makefile文件，编译程序，生成可执行文件；
CMake的执行就是这么简单，其难点在于如何编写CMakeLists.txt文件，下面结合例子简单介绍CMakeLists.txt的编写，看下面这个CMakeLists.txt
```
#project name
PROJECT(test_math)
#head file path
INCLUDE_DIRECTORIES(
    include
)
#source directory
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
#set environment variable
SET(TEST_MATH
    ${DIR_SRCS}
)
#set extern libraries
SET(LIBRARIES
    libm.so
)
#add executable file
ADD_EXECUTABLE(../bin/bin ${TEST_MATH})
#add link library
TARGET_LINK_LIBRARIES(../bin/bin ${LIBRARIES})
```
            或者用下面这个CMakeLists.txt
```
#project name  
PROJECT(test_math)  
  
add_definitions("-Wall -lpthread -g")  
  
#head file path  
INCLUDE_DIRECTORIES(  
include  
)  
  
#source directory  
AUX_SOURCE_DIRECTORY(src DIR_SRCS)  
  
#set environment variable  
SET(TEST_MATH  
${DIR_SRCS}  
)  
  
#set extern libraries  
SET(LIBRARIES  
libm.so  
)  
  
# set output binary path  
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)  
  
SET(FS_BUILD_BINARY_PREFIX "Yfs")  
  
#add executable file  
ADD_EXECUTABLE(${FS_BUILD_BINARY_PREFIX}sqrt ${TEST_MATH})  
  
#add link library  
TARGET_LINK_LIBRARIES(${FS_BUILD_BINARY_PREFIX}sqrt ${LIBRARIES})
```
这是一个测试数学函数的程序的CMakeLists.txt，"#"后面为注释的内容，CMake的命令全部为大写
第2行指定生成的工程名为test_math
第4行指定头文件目录为include
第8行指定源文件目录为src，并将其赋值给环境变量DIR_SRCS
第10行设定环境变量TEST_MATH的值为环境变量DIR_SRCS的值，此处用于显示如何用环境变量对环境变量进行赋值
第14行将数学函数库赋值给环境变量LIBRARIES，当然，可以不用这个环境变量，而在后面直接使用该库名
第18行用于指定生成文件，将环境变量TEST_MATH目录下的所有文件编译生成../bin目录下的可执行文件bin
第20行指定../bin/bin执行时的链接库为环境变量LIBRARIES的值－libm.so
下面给出源文件
### /src/main.c:
```cpp
#include<stdio.h>
#include"../include/a.h"
int main()
{
    double b=25.0;
    double a=0.0;
    a=get_sqrt(b);
 
    printf("a is %lf, b is %lf\n",a,b);
    return 0;
}
```
### /src/a.c
```cpp
#include"../include/a.h"
double get_sqrt(double var1)
{
    return sqrt(var1);
}
```
### /include/a.h
```cpp
#ifndef  A_FILE_HEADER_INC
#define  A_FILE_HEADER_INC
#include<math.h>
 
double get_sqrt(double var1);
 
#endif
```
将CMakeLists.txt放在当前目录下，执行CMakeLists.txt
> - 
$> cmake .
- 
$> make
即可生成可执行文件，在目录/bin下的bin文件，好了运行看其效果是否和所想一样。
## 补充：
未完待续
见：[https://www.cnblogs.com/lidabo/p/7359422.html](https://www.cnblogs.com/lidabo/p/7359422.html)
