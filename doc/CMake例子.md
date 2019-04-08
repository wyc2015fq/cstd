# CMake例子

2016年01月31日 11:04:48 [WhateverYoung](https://me.csdn.net/yangfeng2014) 阅读数：1279



 版权声明：本文为博主原创文章，转载表明出处。	https://blog.csdn.net/yangfeng2014/article/details/50614011

**本帖子适用于初学者，StepByStep的入门CMake，CMake也是一门编程语言，只不过是针对编译和链接这种程序构建的过程的语言，学习基本语法就可以初步入门，后面需要什么复杂的功能，进阶掌握需要自己去查找官方文档，后续会给出查阅的一些方式**

## 软件平台

- Win 7
- VMware Workstation 12 Pro
- Ubuntu 15.10
- CMake 3.2.2



# 编译可执行程序



## case 1

### 目录结构

```
+
| 
+--- main.cpp
+--- CMakeLists.txt
|
/--+ build/
   |
   +--- hi
```

### 文件内容

```cpp
//main.cpp
#include <iostream>

using namespace std;

int main()
{
    cout<<"Hello CMake!"<<endl;
    return 0;
}
```
```cmake
#CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(HELLO)
ADD_EXECUTABLE(hi main.cpp)
```

```shell
mkdir build
cd build
cmake ..
# linux
::make 
# windows
set path="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE";%path%
devenv HELLO.sln /Build "Release|Win32"
```

即可在build目录下编译出hello可执行文件

### 语法解释

> cmake不区分大小写，建议命令使用大写！
>
> **CMAKE_MINIMUM_REQUIRED(VERSION 2.8)** 
> 设置所写的txt文件，至少需要的cmake版本
>
> **PROJECT(HELLO)** 
> 为本次构建起一个名字，这不是最后exe的名字
>
> **ADD_EXECUTABLE(hi main.cpp)** 
> 添加可执行文件，第一个参数是可执行文件的名字，第二个参数是源文件（暂时只考虑少量源文件，这样可以依次写在后面，后面有命令来批量加入源文件）
>
> **cmake ..** 
> 该命令中..，意味着CMakeLists.txt在上一层目录，这样构建保证了所有构建的文件全部在build文件夹中，方便使用版本控制软件如git进行版本控制，只需要忽略build就可以了

## case 2

### 目录结构

同case1

### 文件内容

```cmake
#CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(HELLO)
SET(SRC_LIST main.cpp)
MESSAGE(STATUS "This is BINARY dir：" ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir：" ${PROJECT_SOURCE_DIR})
ADD_EXECUTABLE(hi ${SRC_LIST})
#SET_TARGET_PROPERTIES(hi PROPERTIES LINKER_LANGUAGE CXX)
```

### 语法解释

> **SET(VAR_NAME t1.cpp t2.cpp t3.cpp …)** 
> set命令用于设置一个变量SRC_LIST，用来表示后面的所有给出的文件 
> 使用变量需要用如下形式 ：${SVAR_NAME}，但是如果在IF中使用，则不需要${},直接IF（VAR_NAME）
>
> **PROJECT_BINARY_DIR**和**PROJECT_SOURCE_DIR**变量是在PROJECT(HELLO)语句之后cmake默认定义和初始化 
> 其中PROJECT_BINARY_DIR，是输入cmake命令的目录，本例就是build目录 
> 其中PROJECT_SOURCE_DIR，是输入cmake命令后面紧跟着的目录，本例就是..目录, 
> 注：PROJECT_SOURCE_DIR目录里要有CMakeLists.txt
>
> **MESSAGE([SEND_ERROR | STATUS | FATAL_ERROR] “message to display” …)** 
> message命令用于在cmake..时，向命令行输出信息 
> **SEND_ERROR**，产生错误信息，跳过生成信息 
> **STATUS**，输出前缀为-的信息 
> **FATAL_ERROR**，立即终止所有cmake过程
>
> **SET_TARGET_PROPERTIES(hi PROPERTIES LINKER_LANGUAGE CXX)** 
> SET_TARGET_PROPERTIES设置一些标志，下面的命令设置连接器为C++连接器 
> 注意到，#是cmake的注释符

## case 3

### 目录结构

```
+
| 
+--- CMakeLists.txt
+--- Copyright.txt
+--- ReadME.txt
/--+ src/
   |
   +--- CMakeLists.txt
   /--+ hello/
      |
      +--- CMakeLists.txt  
      /--+ src/
         |
         +--- main.cpp 
|
/--+ build/
   |
   /--+ bin/
      |
      +--- mycmake
/--+ doc/
   |
   +--- Userguide.rst
```

**本次依旧使用case2的例子，但是更像一个工程，doc目录存在文档，src目录存放源文件（当然实际工程src目录下可能还有很多子目录，还有一些依赖的外部库等等，这些会后面详细说明），我们控制最后编译链接出的mycmake可执行程序在build的bin目录下**

### 文件内容

```cmake
#CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(HELLO)
MESSAGE(STATUS "This is BINARY dir" ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir" ${PROJECT_SOURCE_DIR})
ADD_SUBDIRECTORY(src)
//  /src/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
ADD_SUBDIRECTORY(hello)

# /src/hello/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/hello/src HELLO_src)
ADD_EXECUTABLE(mycmake ${HELLO_src})
SET_TARGET_PROPERTIES(mycmake PROPERTIES LINKER_LANGUAGE CXX)
```

### 语法解释

> **ADD_SUBDIRECTORY(src)** 
> 添加子目录，子目录要有CMakeLists.txt
>
> **SET(EXECUTABLE_OUTPUT_PATH “${PROJECT_BINARY_DIR}/bin”)** 
> EXECUTABLE_OUTPUT_PATH是cmake默认变量，修改它可以把可执行文件的路径修改，下面修改为build/bin
>
> **AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/hello/src HELLO_src)** 
> AUX_SOURCE_DIRECTORY，自动识别目标目录下的所有源文件，赋值给第二个参数变量，可以直接用于ADD_EXECUTABLE语句





# 编译静态库和动态库



## case 4

### 目录结构

```
+
| 
+--- CMakeLists.txt
/--+ src/
   |
   +--- CMakeLists.txt
   /--+ hellolib/
      |
      +--- CMakeLists.txt  
      /--+ include/
         |
         +--- hello.h 
      /--+ src/
         |
         +--- hello.cpp      
|
/--+ build/
   |
   /--+ lib/
      |
      +--- libhello.so
```

### 文件内容

```cmake
#/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(HELLOLIB)
ADD_SUBDIRECTORY(src)
#  /src/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
ADD_SUBDIRECTORY(hellolib)

#  /src/hellolib/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/hellolib/src HELLOlib_src)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/hellolib/include")
ADD_LIBRARY(hello SHARED ${HELLOlib_src})
```
```cpp
//  /src/hellolib/include/hello.h
# ifndef HELLO_H
# define HELLO_H
# include <iostream>
void HelloFunc();
# endif

//  /src/hellolib/src/hello.cpp
# include "hello.h"

using namespace std;

void HelloFunc(){
    cout<<"Hello My Linux !"<<endl;
}
```

### 语法解释

> **ADD_LIBRARY(hello SHARED ${HELLOlib_src})** 
> 添加编译库命令： 
> SHARED，动态库，libhello.so 
> STATIC，静态库，libhello.a
>
> **SET(LIBRARY_OUTPUT_PATH “${PROJECT_BINARY_DIR}/lib”)** 
> ELIBRARY_OUTPUT_PATH是cmake默认变量，修改它可以把库文件的路径修改，本例子中修改为build/lib

## case 5

### 目录结构

```
+
| 
+--- CMakeLists.txt
/--+ src/
   |
   +--- CMakeLists.txt
   /--+ hellolib/
      |
      +--- CMakeLists.txt  
      /--+ include/
         |
         +--- hello.h 
      /--+ src/
         |
         +--- hello.cpp      
|
/--+ build/
   |
   /--+ lib/
      |
      +--- libhello.so
      +--- libhello.a
```

### 文件内容

```cmake
#  /src/hellolib/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/hellolib/src HELLOlib_src)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/hellolib/include")
ADD_LIBRARY(hello SHARED ${HELLOlib_src})
ADD_LIBRARY(hello_static STATIC ${HELLOlib_src})
SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")

#SET_TARGET_PROPERTIES(hello PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#SET_TARGET_PROPERTIES(hello_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#SET_TARGET_PROPERTIES(hello PROPERTIES VERSION 1.2 SOVERSION 1)
```

**其余同case4**

### 语法解释

> **INCLUDE_DIRECTORIES(“${PROJECT_SOURCE_DIR}/src/hellolib/include”)** 
> 头文件目录包含指令
>
> **SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME “hello”)** 
> 一般我们希望同时编译出静态库和动态库，并且命名一致，但是cmake的target不允许重名，所以我们把静态库命名为hello_static，然后通过上述语句修改其输出的名字，为hello，这样就可以在lib目录下生成libhello.a和lobhello.so。
>
> **SET_TARGET_PROPERTIES(hello PROPERTIES CLEAN_DIRECT_OUTPUT 1)** 
> 如果执行make后只生成了一种库，说明生成第二个库的时候删除了同名的其他库。上面的语句可以显示指定不删除同名的库。
>
> **SET_TARGET_PROPERTIES(hello PROPERTIES VERSION 1.2 SOVERSION 1)** 
> 一般动态库会包含一个版本号，使用上述命令可以携带版本号：VERSION代表同太苦版本，SOVERSION指代API版本。

## case 6

### 目录结构

```shell
+
| 
+--- CMakeLists.txt
/--+ src/
   |
   +--- CMakeLists.txt
   /--+ hellolib/
      |
      +--- CMakeLists.txt  
      /--+ include/
         |
         +--- hello.h 
      /--+ src/
         |
         +--- hello.cpp
   /--+ mycmake/
      |
      +--- CMakeLists.txt 
      /--+ src/
         |
         +--- mycmake.cpp 
|
/--+ build/
   |
   /--+ lib/
      |
      +--- libhello.so
      +--- libhello.a
   /--+ bin/
      |
      +--- mycmake
```

### 文件内容

```cmake
#  /src/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
ADD_SUBDIRECTORY(hellolib)
ADD_SUBDIRECTORY(mycmake)

#  /src/mycmake/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/mycmake/src mycmake_src)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/hellolib/include")
LINK_DIRECTORIES("${PROJECT_BINARY_DIR}/lib")
ADD_EXECUTABLE(mycmake ${mycmake_src})
TARGET_LINK_LIBRARIES(mycmake libhello.a)
#TARGET_LINK_LIBRARIES(mycmake libhello.so)
```

**其余同case5**

### 语法解释

> **LINK_DIRECTORIES(“${PROJECT_BINARY_DIR}/lib”)** 
> 库文件目录包含指令
>
> **TARGET_LINK_LIBRARIES(mycmake libhello.a)** 
> 链接指令，链接自己编译出的静态/动态库，或者链接网上下载的外部库





