# cmake用法回顾与总结
## （1）用cmake创建可执行文件

### 1）单源文件目录

清单 1 源文件 main.cpp 

```cpp
 #include<iostream>
 int main()  {
     std::cout<<"Hello word!"<<std::endl;
     return 0;
 }
```
CMakeLists.txt文件与main.cpp放在同一目录下
清单 2 CMakeLists.txt  
```xml
PROJECT(main)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 
AUX_SOURCE_DIRECTORY(. DIR_SRCS) 
ADD_EXECUTABLE(main ${DIR_SRCS})
```
CMakeLists.txt 的语法比较简单,由命令、注释和空格组成,其中命令是不区分大小写的,符号"#"后面的内容被认为是注释。命令由命令名称、小括号和参数组成,参数之间使用空格进行间隔。
例如对于清单2的 CMakeLists.txt 文件:
第一行是一条命令,名称是 PROJECT ,参数是 main ,该命令表示项目的名称是 main 。
第二行的命令限定了 CMake 的版本。
第三行使用命令 AUX_SOURCE_DIRECTORY 将当前目录中的源文件名称赋值给变量 DIR_SRCS 。 CMake 手册中对命令 AUX_SOURCE_DIRECTORY 的描述如下:
aux_source_directory(<dir> <variable>) 
该命令会把参数 <dir> 中所有的源文件名称赋值给参数 <variable> 。
第四行使用命令 ADD_EXECUTABLE 指示变量 DIR_SRCS 中的源文件需要编译成一个名称为 main 的可执行文件。

### 2）多源文件目录

![img](https://img-blog.csdn.net/20131014145154203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVyb2ppYV8x/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
清单 3 目录 step2 中的 CMakeLists.txt 

```xml
PROJECT(main)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ADD_SUBDIRECTORY( src )
AUX_SOURCE_DIRECTORY(. DIR_SRCS) 
ADD_EXECUTABLE(main ${DIR_SRCS})
TARGET_LINK_LIBRARIES( main Test )
```
相对于清单 2，该文件添加了下面的内容: 第三行，使用命令 ADD_SUBDIRECTORY 指明本项目包含一个子目录 src 。第六行，使用命令 TARGET_LINK_LIBRARIES 指明可执行文件 main 需要连接一个名为Test的链接库 。
第二步，子目录中的 CMakeLists.txt 
在子目录 src 中创建 CMakeLists.txt。文件内容如下:
清单 4 目录 src 中的 CMakeLists.txt
```xml
AUX_SOURCE_DIRECTORY(. DIR_TEST1_SRCS)
ADD_LIBRARY ( Test ${DIR_TEST1_SRCS})  
```
在该文件中使用命令 ADD_LIBRARY 将 src 中的源文件编译为库
ADD_SUBDIRECTORY( src ) 时进入目录src 对其中的CMakeLists.txt 进行解析。
3）在工程中查找并使用其他库初步
清单5 Findlibdb_cxx.cmake文件
```xml
MESSAGE(STATUS"Using bundled Findlibdb.cmake...")
FIND_PATH(
    LIBDB_CXX_INCLUDE_DIR
    NAMES
    PATHS
    )
FIND_LIBRARY(
    LIBDB_CXX_LIBRARIES NAMES
    PATHS
    )
```
清单6 能调用Finddb_cxx.cmake的CMakeLists.txt文件
```perl
PROJECT(main)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_SOURCE_DIR .)
SET(CMAKE_MODULE_PATH ${CMAKE_ROOT}/Modules ${CMAKE_SOURCE_DIR}/cmake/modules)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
ADD_EXECUTABLE(main ${DIR_SRCS})
FIND_PACKAGE(libdb_cxx REQUIRED)
MARK_AS_ADVANCED(
LIBDB_CXX_INCLUDE_DIR
LIBDB_CXX_LIBRARIES
)
IF(LIBDB_CXX_INCLUDE_DIR AND LIBDB_CXX_LIBBRARIES)
MESSAGE(STATUS"Found libdb libraries")
INClUDE_DIRECTORIES(${LIBDB_CXX_INCLUDE_DIR})
MESSAGE(${LIBDB_CXX_LIBRARIES})
TARGET_LINK_LIBRARIES(main ${LIBDB_CXX_LIBRARIES})
ENDIF(LIBDB_CXX_INCLUDE_DIR AND LIBDB_CXX_LIBRARIES)
```
其中第4行便是设置调用清单5的指令；第8行时，cmake会到CMAKE_MODULE_PATH中查找并执行清单5的指令

## （2）用cmake创建动态库和静态库

新建个t3目录，在t3目录中再建个lib目录,在t3目录中新建个CMakeLists.txt文件，然后在lib中新建CMakeLists.txt，hello.h，hello.cpp三个文件

### 1)清单1 hello.h

```cs
//hello.h
#ifndef HELLO_H
#define HELLO_H
#include <stdio.h>
void HelloFunc();
#endif
```
### 2)清单2 hello.cpp

```cs
//hello.c
#include "hello.h"
void HelloFunc()
{
	printf("Hello World\n");
}
```
### 3)清单3 lib目录下CMakeLists.txt

```perl
#CMakeLIsts.txt
SET(LIBHELLO_SRC hello.c)
ADD_LIBRARY(hello SHARED ${LIBHELLO_SRC})
ADD_LIBRARY(hello_static STATIC ${LIBHELLO_SRC})
SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")
SET_TARGET_PROPERTIES(hello PROPERTIES VERSION 1.2 SOVERSION 1)
```
### 4)清单4 t3目录下CMakeLists.txt

```vbnet
PROJECT(HELLOLIB)
ADD_SUBDIRECTORY(lib)
```
## （3）调用动态库和静态库

### 1）安装动态库，在lib目录下的CMakeLists.txt 清单1

```bash
#CMakeLists.txt
SET(LIBHELLO_SRC hello.c)
ADD_LIBRARY(hello SHARED ${LIBHELLO_SRC})
ADD_LIBRARY(hello_static STATIC ${LIBHELLO_SRC})
SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")
#SET_TARGET_PROPERTIES(hello PROPERTIES VERSION 1.2 SOVERSION 1)
SET(CMAKE_INSTALL_PREFIX /home/hewenjia/test/cmaketest/cmake_practice)
INSTALL(TARGETS hello hello_static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
```
### 2) 调用外部共享库和头文件

新建目录t4用于存放可执行文件工程，再建个src，在src中建CMakeLists.txt，类似前面的步骤，清单1如下：

```ruby
INCLUDE_DIRECTORIES(/home/hewenjia/test/cmaketest/cmake_practice/t3/lib)
LINK_DIRECTORIES(/home/hewenjia/test/cmaketest/cmake_practice/lib)
ADD_EXECUTABLE(main main.c)
TARGET_LINK_LIBRARIES(main libhello.so)
```
PS：一定需要在创建main后，在链接库，这个过程可能逻辑上相反，但实现过程的逻辑正确！
清单2 调用函数的main.c文件
```cs
//main.c
#include <hello.h>
int main()
{
	HelloFunc();
	return 0;
}
```
最后运行结果：
![img](https://img-blog.csdn.net/20131015112529078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVyb2ppYV8x/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
参考文献：
[1] http://wenku.baidu.com/view/2fdbbcf3f61fb7360b4c6526.html
[2] http://wenku.baidu.com/view/c172762fb4daa58da0114a54.html