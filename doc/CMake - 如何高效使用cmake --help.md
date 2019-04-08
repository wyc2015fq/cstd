# CMake - 如何高效使用cmake --help

2016年02月05日 11:43:52 [WhateverYoung](https://me.csdn.net/yangfeng2014) 阅读数：1059



 版权声明：本文为博主原创文章，转载表明出处。	https://blog.csdn.net/yangfeng2014/article/details/50637486

本帖子适用于继续深入学习者，进阶掌握需要自己的检索和查找能力，本文给出一些方式方法。

安装好cmake之后，其实就自带了最为丰富也是最为权威的说明文档，也就是”cmake –help”，以下简要介绍。

## command

> **cmake –help-commands** 
> 这个命令给出cmake内置的命令的详细帮助 
> **cmake –help-command-list** 
> 打印出所有command列表
>
> 下面给出如何查找一个命令解析的大概过程（Linux）： 
> （Win下可以重定向到文本中，使用编辑器进行查找） 
> **cmake –help-command-list | grep find** 
> 可以在list中查找带find的命令 
> **cmake –help-command find_library** 
> 获得find_library的详细解释

## module

用于查找常用的模块，譬如boost，bzip2, python等。

通过简单的include命令包含预定义的模块，就可以得到一些模块执行后定义好的变量，非常方便。

譬如常用的boost库，可以通过如下方式： 
\# Find boost 1.40 
INCLUDE(FindBoost) 
find_package(Boost 1.40.0 COMPONENTS thread unit_test_framework) 
if(NOT Boost_FOUND) 
message(STATUS “BOOST not found, test will not succeed!”) 
endif() 
一般开头部分的解释都相当有用，可满足80%需求：

> cmake –help-module FindBoost |head -40

```
FindBoost
---------

Find Boost include dirs and libraries

Use this module by invoking find_package with the form::

 find_package(Boost
   [version] [EXACT]      # Minimum or EXACT version e.g. 1.36.0
   [REQUIRED]             # Fail with error if Boost is not found
   [COMPONENTS <libs>...] # Boost libraries by their canonical name
   )                      # e.g. "date_time" for "libboost_date_time"

This module finds headers and requested component libraries OR a CMake
package configuration file provided by a "Boost CMake" build.  For the
latter case skip to the "Boost CMake" section below.  For the former
case results are reported in variables::

 Boost_FOUND            - True if headers and requested libraries were found
 Boost_INCLUDE_DIRS     - Boost include directories
 Boost_LIBRARY_DIRS     - Link directories for Boost libraries
 Boost_LIBRARIES        - Boost component libraries to be linked
 Boost_<C>_FOUND        - True if component <C> was found (<C> is upper-case)
 Boost_<C>_LIBRARY      - Libraries to link for component <C> (may include
                          target_link_libraries debug/optimized keywords)
 Boost_VERSION          - BOOST_VERSION value from boost/version.hpp
 Boost_LIB_VERSION      - Version string appended to library filenames
 Boost_MAJOR_VERSION    - Boost major version number (X in X.y.z)
 Boost_MINOR_VERSION    - Boost minor version number (Y in x.Y.z)
 Boost_SUBMINOR_VERSION - Boost subminor version number (Z in x.y.Z)
 Boost_LIB_DIAGNOSTIC_DEFINITIONS (Windows)
                        - Pass to add_definitions() to have diagnostic
                          information about Boost's automatic linking
                          displayed during compilation

This module reads hints about search locations from variables::

 BOOST_ROOT             - Preferred installation prefix
  (or BOOSTROOT)
 BOOST_INCLUDEDIR       - Preferred include directory e.g. <prefix>/include
1234567891011121314151617181920212223242526272829303132333435363738394041
```

## property

Property一般很少需要直接改动，除非你想修改一些默认的行为，譬如修改生成的动态库文件的soname等。

譬如需要在同一个目录下既生成动态库，也生成静态库，那么默认的情况下，cmake根据你提供的target名字自动生成类似的libtarget.so, libtarget.a，但是同一个project只能同时有一个，因为target必须唯一。

这时候，就可以通过修改taget对应的文件名，从而达到既生成动态库也产生静态库的目的。

```
cmake --help-property-list | grep NAME
:
<CONFIG>_OUTPUT_NAME
ARCHIVE_OUTPUT_NAME
ARCHIVE_OUTPUT_NAME_<CONFIG>
COMPILE_PDB_NAME
COMPILE_PDB_NAME_<CONFIG>
EXPORT_NAME
GENERATOR_FILE_NAME
IMPORTED_NO_SONAME
IMPORTED_NO_SONAME_<CONFIG>
IMPORTED_SONAME
IMPORTED_SONAME_<CONFIG>
INSTALL_NAME_DIR
LIBRARY_OUTPUT_NAME
LIBRARY_OUTPUT_NAME_<CONFIG>
NAME
NO_SONAME
OUTPUT_NAME
OUTPUT_NAME_<CONFIG>
PDB_NAME
PDB_NAME_<CONFIG>
RUNTIME_OUTPUT_NAME
RUNTIME_OUTPUT_NAME_<CONFIG>
VS_GLOBAL_ROOTNAMESPACE
VS_SCC_PROJECTNAME1234567891011121314151617181920212223242526
cmake --help-property OUTPUT_NAME
:
OUTPUT_NAME
-----------

Output name for target files.

This sets the base name for output files created for an executable or
library target.  If not set, the logical target name is used by
default.
1234567891011
```

## variable

可以查找cmake自己定义了哪些变量你可以直接使用，譬如OSName，是否是Windows，Unix等。

```
cmake --help-variable-list | grep CMAKE | grep HOST
:
CMAKE_HOST_APPLE
CMAKE_HOST_SYSTEM
CMAKE_HOST_SYSTEM_NAME
CMAKE_HOST_SYSTEM_PROCESSOR
CMAKE_HOST_SYSTEM_VERSION
CMAKE_HOST_UNIX
CMAKE_HOST_WIN32
12345678910
cmake --help-variable CMAKE_HOST_SYSTEM
:
CMAKE_HOST_SYSTEM
-----------------

Composit Name of OS CMake is being run on.

This variable is the composite of ``CMAKE_HOST_SYSTEM_NAME`` and
``CMAKE_HOST_SYSTEM_VERSION``, e.g.
``${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_HOST_SYSTEM_VERSION}``.  If
``CMAKE_HOST_SYSTEM_VERSION`` is not set, then this variable is
the same as ``CMAKE_HOST_SYSTEM_NAME``.
12345678910111213
```

## 进阶文档

> <https://cmake.org/> 介绍与下载 
> <https://cmake.org/install/> 安装方法 
> <https://cmake.org/mailing-lists/> 邮件组 
> <https://cmake.org/cmake/help/cmake2.6docs.html> 参考文档 
> <https://cmake.org/Wiki/CMake/Language_Syntax> 语法释义 
> <https://cmake.org/Wiki/CMake:Articles> 
> <https://cmake.org/Wiki/CMake_FAQ> 问题集锦 
> <https://cmake.org/cmake/help/v2.8.8/cmake.html#section_Commands> CMake 2.8 的命令可以在此查询 
> <https://techbase.kde.org/Development/CMake> 
> <https://techbase.kde.org/Policies/CMake_Coding_Style> 代码风格