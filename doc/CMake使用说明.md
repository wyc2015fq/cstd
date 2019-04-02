# CMake使用说明

2018年12月03日 19:13:37 [knowledgebao](https://me.csdn.net/knowledgebao) 阅读数：57



编译相关索引：<https://blog.csdn.net/knowledgebao/article/details/84973055>

------

> 1. **定义设置变量：**set(MACROName para),将MACROName定义为para，如果变量已经存在，覆盖。如果不存在，创建并且赋值。可用于头文件、库等的统一定义。
> 2. **设置编译选项：SET(CMAKE_BUILD_TYPE "Debug")SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")**
> 3. **添加头文件：**INCLUDE_DIRECTORIES("a.h" "b.h")，包含所有的头文件，也可以将所有的头文件包含在一个变量里，这里直接包含变量即可。
> 4. **给当前目录源文件关联变量：**aux_source_directory(. test_src)，当前目录源文件关联到test_stc，也就是说test_stc是一个包含了当前目录下所有c/cpp文件的变量。set的扩展功能。
> 5. **编译动态库：**add_library(libTest SHARED ${test_src}),编译动态库libTest,第二个参数SHARED表示动态库。后边跟源文件或源文件对应的变量。
> 6. **编译静态库：**add_library(libTest ${test_src})，同上
> 7. **编译可执行程序：**ADD_EXECUTABLE(ExeName ${exe_src})，编译ExeName可执行程序，后边跟源文件或源文件对应的变量。
> 8. **可执行程序连接库：**TARGET_LINK_LIBRARIES(ExeName a.so b.lib)，给ExeName连接库，包括动态库以及静态库，通过ADD_SUBDIRECTORY添加的无需加路径，否则需要包含依赖路径。默认情况下，编译后程序依赖的库路径和这里的路径一致，除非你设置变量CMAKE_SKIP_BUILD_RPATH或CMAKE_BUILD_WITH_INSTALL_RPATH为TRUE。
> 9. **添加子目录：**ADD_SUBDIRECTORY(subDirectory subDirectoryOut)，添加依赖目录，如果依赖目录不是当前目录的子目录，需要指定路径，比如../parentPath
> 10. **设置工程名字：**project (projectName)，实际上是对变量PROJECT_NAME赋值等价于：set(PROJECT_NAME projectName)
> 11. **指定RPATH目录：**cmake提供4个变量供用户选择，默认RPATH设置如下：
>
> ```
> set(CMAKE_SKIP_BUILD_RPATH FALSE)                 # 编译时加上RPATH
> set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)         # 编译时RPATH不使用安装的RPATH
> set(CMAKE_INSTALL_RPATH "")                       # 安装RPATH为空
> set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)      # 安装的执行文件不加上RPATH
> ```
>
> 如果我们编译后，不执行make install，编译后的可执行程序就想指定RPATH运行时目录，则可以先设置CMAKE_BUILD_WITH_INSTALL_RPATH为TRUE，意思是编译时RPATH使用CMAKE_INSTALL_RPATH变量，再设置CMAKE_INSTALL_RPATH变量，这样编译后就指定了RPATH路径。set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) set(CMAKE_INSTALL_RPATH /home/x64/linux)，关于运行时目录rpath，可参考：<https://blog.csdn.net/knowledgebao/article/details/84565410>

举例：

Main函数所在的：CMakeLists.txt

```bash
cmake_minimum_required(VERSION 2.8)
 
add_definitions("-Wall -std=c++11") # <= 新增的编译选项
 
#set projectName in ${PROJECT_NAME}
project (GStreamerGroup)
 
#not current SubDiretory
if(NOT DEPENDS_DIR)
    set( DEPENDS_DIR ${CMAKE_SOURCE_DIR}/../Depends)
endif()
 
#include directory
if(NOT DEPENDS_INCLUDE)
    set( DEPENDS_INCLUDE ./
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/protocol
	${CMAKE_SOURCE_DIR}/Schedule
	${DEPENDS_DIR}
	${DEPENDS_DIR}/json
	${DEPENDS_DIR}/libuv/include
	${DEPENDS_DIR}/log
	${DEPENDS_DIR}/NCX/include
	${DEPENDS_DIR}/Util
)
endif()
 
#link lib(.a or .so or subDirectory lib)
if(NOT DEPENDS_LIBRARIES)
    set( DEPENDS_LIBRARIES
	libSchedule libprotocol libjson liblog libuvCPP 
	${DEPENDS_DIR}/../x64/linux/libuv.so
	${DEPENDS_DIR}/../x64/linux/libNCXEX.so
	pthread
)
endif()
 
#set(CMAKE_SKIP_BUILD_RPATH TRUE)  
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH ${DEPENDS_DIR}/../x64/linux)
 
#add subDirectory
ADD_SUBDIRECTORY(protocol protocol.out)
ADD_SUBDIRECTORY(Schedule Schedule.out)
#if not subDirectory,must set explicit path
if( IS_DIRECTORY ${DEPENDS_DIR} )
    ADD_SUBDIRECTORY(${DEPENDS_DIR}/json json.out)
	ADD_SUBDIRECTORY(${DEPENDS_DIR}/log log.out)
	ADD_SUBDIRECTORY(${DEPENDS_DIR}/libuv/include libuvCPP.out)
else()
    message(FATAL_ERROR   "INVALID FOLDER 'DEPENDS_DIR'=${DEPENDS_DIR}" )
endif()
 
#define source file MACRO
aux_source_directory(. GStreamerGroupCPP)
INCLUDE_DIRECTORIES(${DEPENDS_INCLUDE})
ADD_EXECUTABLE(${PROJECT_NAME} ${GStreamerGroupCPP})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${DEPENDS_LIBRARIES})
```

子目录所在的CMakeLists.txt

```bash
aux_source_directory(. protocol)
INCLUDE_DIRECTORIES(${DEPENDS_INCLUDE})
ADD_LIBRARY(libprotocol ${protocol})
```

 

参考文章：

1，[cmake教程](https://www.jianshu.com/p/bf1d235ed07a)

2,  [CMake命令target_link_libraries链接库的顺序](https://blog.csdn.net/zhujianwei31415/article/details/50727952)

3，[cmake 添加头文件目录，链接动态、静态库](https://www.cnblogs.com/binbinjx/p/5626916.html)

4，[CMAKE解决动态链接库RPATH错误问题](http://kescoode.com/cmake-rpath-problem/)

 