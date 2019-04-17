# Qt libraries not found  Ubuntu找不到Qt库的解决方案 - DumpDoctorWang的博客 - CSDN博客





2018年07月23日 11:34:20[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：733








# 1、先从官网下载Qt并安装

下载地址：[Index of /official_releases/qt](http://download.qt.io/official_releases/qt/)

2、使用configure配置的项目

configure配置的项目一般依赖于pkg-config去找库的头文件目录和库目录。pkg-config在PKG_CONFIG_PATH所指定的路径中去寻找.pc文件，在这些文件里面找对应的库的配置。因此，要让使用configure配置的项目找到Qt库，就在PKG_CONFIG_PATH添加一个Qt库的.pc文件所在的目录。

```bash
gedit ~/.bashrc
```

在./bashrc文件的末尾添加如下几行

```bash
export QTDIR=/home/creator/Applications/Qt592/5.9.2/gcc_64
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$QTDIR/lib/pkgconfig
export PATH=$QTDIR/bin:$PATH 
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
```

**QTDIR是你Qt的安装路径**，别搞错了。

然后，使用configure配置的项目就能找到Qt的库了。

# 3、使用cmake配置的项目

CMakeLists.txt

```
cmake_minimum_required(VERSION 2.8)
project(Qt5test)

# 添加寻找.cmake文件的路径，有时候不需要写下面这几行
set(QT_CMAKE_DIR /home/creator/Applications/Qt592/5.9.2/gcc_64/lib/cmake)
# 下面两行是添加了寻找Qt5Config.cmake的路径
list(APPEND CMAKE_MODULE_PATH ${QT_CMAKE_DIR}/Qt5)
# 下面两行是添加了寻找Qt5WidgetsConfig.cmake的路径
list(APPEND CMAKE_MODULE_PATH ${QT_CMAKE_DIR}/Qt5Widgets)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

# Qt5Core_INCLUDE_DIRS这个定义去QT_CMAKE_DIR/Qt5Core/Qt5CoreConfig.cmake里面找
# Qt5CoreConfig.cmake里面包含了很多定义，包含Qt5Core_LIBRARIES
include_directories(${Qt5Core_INCLUDE_DIRS}) 
# 类似的Qt5Widgets_INCLUDE_DIRS这个定义去QT_CMAKE_DIR/Qt5Widgets/Qt5WidgetsConfig.cmake里面找
# Qt5WidgetsConfig.cmake里面包含了很多定义，包含Qt5Widgets_LIBRARIES
include_directories(${Qt5Widgets_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp )

target_link_libraries(${PROJECT_NAME} ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES})
```

Qt的官方cmake文档： [http://doc.qt.io/qt-5/cmake-manual.html](http://doc.qt.io/qt-5/cmake-manual.html)



