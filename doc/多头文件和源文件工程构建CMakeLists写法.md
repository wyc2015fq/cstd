# 多头文件和源文件工程构建CMakeLists写法
2018年10月01日 18:15:47 [正午之阳](https://me.csdn.net/xh_hit) 阅读数：509
1.最简单的CMakeLists.txt如下，只有一个源文件：
```cmake
cmake_minimum_required (VERSION 2.6)   #版本要求
project (Tutorial)                     #工程名称
add_executable(Tutorial tutorial.cpp)  #制定生成目标
```
2.如果有两个以上源文件，并在同级目录下：
```cmake
cmake_minimum_required (VERSION 2.6)   #版本要求
project (Tutorial)                     #工程名称
add_executable(Tutorial 1.cpp 2.cpp 3.cpp)  #制定生成目标
```
如果有很多源文件，那么可用aux_sources_directory统一获得：
```cmake
cmake_minimum_required (VERSION 2.6)   #版本要求
project (Tutorial)                     #工程名称
aux_source_directory（. DIR_SRCS）
add_executable(Tutorial ${DIR_SRCS})  #制定生成目标
```
3.如果有多个cpp文件，且不再同级目录，例如：一级目录cmake_test 包含main.cpp 和 add文件夹，add文件夹含有add.cpp和add.hpp；那么首先需要在add文件夹下创建子目录的CMakeLists.txt,
```cmake
add_library(add add.cpp)  #生成静态库
```
然后在一级目录下创建整体项目的CMakeLists.txt：
```cmake
cmake_minimum_required (VERSION 2.6)   #版本要求
project (Tutorial)                     #工程名称
include_directories("${PROJECT_SOURCE_DIR}add") #添加头文件
add_subdirectory(add)                # 添加 add子目录
aux_source_directory（. DIR_SRCS）
add_executable(Tutorial ${DIR_SRCS})  #制定生成目标
target_link_libraries(Tutorial add)  #添加链接库
```
其中target_link_libraries(Tutorial add)可由link_libraries(add)代替，但是link_libraries(add)一定要放在add_executable()的前面。
4.方法3是多源文件工程的一种目录形式，在构建大型工程时将所有的源文件放在src子目录中，所得头文件放在include子目录中是一种较为常见的工程文件分布形式。
```cmake
cmake_minimum_required (VERSION 2.6)   #版本要求
project (Tutorial)                     #工程名称
#find_package(OpenCV REQUIRED)
#link_libraries(${Opencv_LIBS})
#message(${Opencv_LIBS})
include_directories(include) #添加头文件
FILE(GLOB_RECURSE CURRENT_INCLUDE include/*.hpp)
FILE(GLOB_RECURSE CURRENT_SOURCES src/*.cpp)     #aux_source_directory(src CURRENT_SOURCES)
add_executable(Tutorial ${CURRENT_SOURCES} ${CURRENT_INCLUDE})  #制定生成目标
```