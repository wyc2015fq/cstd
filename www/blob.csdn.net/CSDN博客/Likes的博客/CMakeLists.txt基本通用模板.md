# CMakeLists.txt基本通用模板 - Likes的博客 - CSDN博客
2018年12月03日 17:29:10[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：55
```bash
cmake_minimum_required(VERSION 3.9)
project(LevealDBTry)
#设定编译参数
set(CMAKE_CXX_STANDARD       11)
set(CMAKE_BUILD_TYPE "Debug")
#设定源码列表.cpp
set(SOURCE_FILES ./main.cc)
#设定所有源码列表 ：aux_source_directory(<dir> <variable>)
#设定头文件路径
include_directories(../include/)
#include_directories("路径1"  “路径2”...)
#设定链接库的路径（一般使用第三方非系统目录下的库）
link_directories(../build/)
#link_directories("路径1"  “路径2”...)
#添加子目录,作用相当于进入子目录里面，展开子目录的CMakeLists.txt
#同时执行，子目录中的CMakeLists.txt一般是编译成一个库，作为一个模块
#在父目录中可以直接引用子目录生成的库
#add_subdirectory(math)
#生成动/静态库
#add_library(    动/静态链接库名称      SHARED/STATIC(可选，默认STATIC)      源码列表    )
#可以单独生成多个模块
#生成可执行文件
add_executable(myLevealDB   ${SOURCE_FILES} )
#比如：add_executable(  hello_world    ${SOURCE_FILES}  )
target_link_libraries( myLevealDB  libleveldb.a)
```
