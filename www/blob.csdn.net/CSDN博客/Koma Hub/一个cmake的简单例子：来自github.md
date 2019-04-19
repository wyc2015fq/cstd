# 一个cmake的简单例子：来自github - Koma Hub - CSDN博客
2019年03月19日 22:32:42[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：24
> 
 Copyright © 2013-2014, Rafał Słota, Konrad Zemek
**Table of Contents**
[根CMakeLists.txt](#%E6%A0%B9CMakeLists.txt)
[文件结构](#%E6%96%87%E4%BB%B6%E7%BB%93%E6%9E%84)
[doc/CMakeLists.txt](#doc%2FCMakeLists.txt)
[doc/Doxyfile.in](#doc%2FDoxyfile.in)
[src/CMakeLists.txt](#src%2FCMakeLists.txt)
# 根CMakeLists.txt
```
cmake_minimum_required(VERSION 2.6)
project(TCPIPStack C)
# The version number.
set(TCPIPStack_VERSION_MAJOR 0)
set(TCPIPStack_VERSION_MINOR 1)
set(TCPIPStack_VERSION_PATCH 1)
set(TCPIPStack_VERSION $(TCPIPStack_VERSION_MAJOR).$(TCPIPStack_VERSION_MINOR).$(TCPIPStack_VERSION_PATCH))
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -pthread")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -ggdb")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
add_subdirectory(src)
add_subdirectory(doc)
```
# 文件结构
```
software root directory
 |
 +---doc
 |    |
 |    +---CMakeList.txt
 |    |
 |    +---Doxyfile.in
 |
 +---src
      |
      +---CMakeList.txt
      |
      +---some other source code file
```
# doc/CMakeLists.txt
```
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY
    )
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)
```
# doc/Doxyfile.in
这是一个一libconfig格式的文件。
# src/CMakeLists.txt
```
add_library( tcpip
    eth.c
    hw.c
    icmp.c
    ip.c
    ndp_daemon.c
    net.c
    tcp.c
    udp.c
)
add_executable( test main.c )
target_link_libraries( test tcpip )
```
> 
详情请见：GitHub's TCPIPv6 project
