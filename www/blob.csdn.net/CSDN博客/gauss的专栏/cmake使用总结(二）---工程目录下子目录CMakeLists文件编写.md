# cmake使用总结(二）---工程目录下子目录CMakeLists文件编写 - gauss的专栏 - CSDN博客
2015年05月17日 22:49:02[gauss](https://me.csdn.net/mathlmx)阅读数：527
**子目录utility下的CMakeLists.txt 文件如下：**
--------------------------------------------------------------------------------------------------------------------
#Cmake file for library utility.a
#Author: double__song
#Created: 2011/3/3
SET(SOURCE_FILES #设置变量，表示所有的源文件
ConfigParser.cpp
StrUtility.cpp
)
INCLUDE_DIRECTORIES( #相关头文件的目录
/usr/local/include
${PROJET_SOURCE_DIR}/utility
)
LINK_DIRECTORIES( #相关库文件的目录
/usr/local/lib
)
ADD_LIBRARY(association ${SOURCE_FILES})#生成静态链接库libassociation.a
TARGET_LINK_LIBRARY(associationcore） #依赖的库文件
SET_TARGET_PROPERTIES(utility PROPERTIES #表示生成的执行文件所在路径
RUNTIME_OUTPUT_DIRECTORY>"${PROJECT_SOURCE_DIR}/lib")
--------------------------------------------------------------------------------------------------------------------
相关解释：
1.SET(SOURCE_FILES .....)
表示要编译的源文件，所有的源文件都要罗列到此处。set设置变量，变量名SOURCE_FILES自定义。
2.INCLUDE_DIRECTORY(...)
include头文件时搜索的所有目录
变量PROJECT_SOURCE_DIR表示工程所在的路径，系统默认的变量
3.LINK_DIRECTORIES(...)
库文件存放的目录，在程序连接库文件的时候要再这些目录下寻找对应的库文件
4.ADD_LIBRARY(...)
表示生成静态链接库libassociaiton.a，由${PROJECT_SOURCE_DIR}代表的文件生成。
语法：ADD_LIBRARY(libname[SHARED|STATIC]
SHARED表示生成动态库， STATIC表示生成静态库。
5.TARGET_LINK_LIBRARY(association core)
表示库association依赖core库文件
6.SET_TARGET_PROPERTIES
设置编译的库文件存放的目录，还可用于其他属性的设置。如不指定，
生成的执行文件在当前编译目录下的各子目录下的build目录下，好拗口！简单一点：
**EXECUTABLE_OUTPUT_PATH **已过时，使用target 属性 RUNTIME_OUTPUT_DIRECTORY 。
**LIBRARY_OUTPUT_PATH **已过时，使用target 属性 ARCHIVE_OUTPUT_DIRECTORY, LIBRARY_OUTPUT_DIRECTORY, 和 RUNTIME_OUTPUT_DIRECTORY。**CMAKE_BUILD_TYPE**构建类型，可能的值有：空, Debug, Release, RelWithDebInfo 和 MinSizeRel。
**CMAKE_BINARY_DIR **This is the full path to the top level of the current CMake build tree.
**CMAKE_CURRENT_BINARY_DIR **This the full path to the build directory that is currently being processed by cmake.
**PROJECT_BINARY_DIR **This is the binary directory of the most recent PROJECT command.
**[Project name]_BINARY_DIR** Top level binary directory for the named project.
**CMAKE_SOURCE_DIR **This is the full path to the top level of the current CMake source tree.
**CMAKE_CURRENT_SOURCE_DIR **This the full path to the source directory that is currently being processed by cmake.
**PROJECT_SOURCE_DIR **This is the source directory of the most recent PROJECT command.
**[Project name]_SOURCE_DIR** Top level source directory for the named project.
如指定在：./src/lib 下
不指定在：./src/build/utility/build 目录下
生成的中间文件在./src/build/utilty/build目录下，不受该命令额影响
**子目录server下的CMakeLists.txt 文件：**
--------------------------------------------------------------------------------------------
SET(SOURCE_FILES
Gassociation.cpp
ConfigurationHandler.cpp
)
INCLUDE_DIRECTORIES（
/usr/local/include
${PROJECT_SOURCE_DIR}/utility
${PROJECT_SOURCE_DIR}/association
)
LINK_LIBRARIES(
/usr/local/lib
${PROJECT_SOURCE_DIR}/lib
)
ADD_EXECUTABLE(server ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(server
utility
）
SET_TARGET_PROPERTIES(serverPROPERTIES #表示生成的执行文件所在路径
RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
-------------------------------------------------------------------------------------------------------
相关解释：
1.ADD_EXECUTABLE() #指定要生成的执行文件的名称server
其他用法同utilty/CMakeLists.txt
2.SET_TARGET_PROPERTIES
设置生成的执行文件存放的路径，
注意：
执行文件server依赖的子目录utility 子目录生成的静态库libutility.a,在指定的时候要写成：
TARGET_LINK_LIBRARIES(server utility）
而不能写成：
TARGET_LINK_LIBRARIES(serverlibutility.a）
否则编译总会提示找不到libutility库文件。
但使用第三方的库却要指定成具体的库名，如：libACE-6.0.0.so
这一点很诡异，暂时还没找到原因。
完成对应的CMakeLists.txt文件编写后，便可以进行编译了。
编译：
进入./src/build
执行cmake..
make
cmake的使用很简单，更高级的应用好比版本信息，打包，安装等相关基本的应用后面会一一介绍，
复杂的语法使用要参考官方文档。
