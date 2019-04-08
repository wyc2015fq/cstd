# CMake综述



## Where download？

> www.cmake.org

## Why CMake ？

1. 开放源代码
2. 跨平台编译构建
   - Linux/Unix，生成makefile
   - Apple，生成Xcode工程文件
   - Windows，生成MSVS工程文件

（注：网上也有资料，可以直接在windows生成nmake文件，直接nmake编译链接出可执行程序，笔者尝试并未成功，主要错误是缺少很多dll或者pdb文件） 
\3. 支持安装（make install）、测试安装的程序是否能正确执行（make test，或者ctest）、生成当前平台的安装包（make package）、生成源码包（make package_source）、产生Dashboard显示数据并上传等高级功能，只要在CMakeLists.txt中简单配置，就可以完成很多复杂的功能，包括写测试用例。

## When CMake ？

1. 实际的项目需求
2. 大工程，文件多
3. C，C++，java

## Reference

> <https://cmake.org/> 介绍与下载 
> <https://cmake.org/install/> 安装方法 
> <https://cmake.org/mailing-lists/> 邮件组 
> <https://cmake.org/cmake/help/cmake2.6docs.html> 参考文档 
> <https://cmake.org/Wiki/CMake/Language_Syntax> 语法释义 
> <https://cmake.org/Wiki/CMake:Articles> 
> <https://cmake.org/Wiki/CMake_FAQ> 问题集锦 
> <https://techbase.kde.org/Development/CMake> 
> <https://techbase.kde.org/Policies/CMake_Coding_Style> 代码风格 
> CMake_Tutorial.pdf 
> CMake实践.pdf