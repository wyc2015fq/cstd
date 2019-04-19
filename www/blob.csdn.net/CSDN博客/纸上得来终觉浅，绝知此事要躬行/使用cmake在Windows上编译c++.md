# 使用cmake在Windows上编译c++ - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年03月27日 17:53:21[boonya](https://me.csdn.net/boonya)阅读数：37标签：[cmake																[c																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=cmake&t=blog)
个人分类：[C/C++](https://blog.csdn.net/boonya/article/category/7395156)
原文地址：[https://blog.csdn.net/zhangyiant/article/details/51289404](https://blog.csdn.net/zhangyiant/article/details/51289404)
## cmake介绍
cmake是一个构建C++代码的跨平台工具，他能够干什么呢？他能够搜索你的平台环境，然后生成平台上用于Build的文件。在Windows上安装了Visual Studio, 他能够帮你生成.sln, .vcxproj文件。如果在Linux上，能够帮你生成makefile。在MAC OS上，帮你生成xcode的项目文件。利用这些文件你就可以在本地编译，链接文件。生成这些项目，solution文件的输入信息，就是一套自己编写的和平台无关的配置文件。一般使用CMakeLists.txt文件。
## cmake在Windows上的安装
[https://cmake.org/download/](https://cmake.org/download/) 上下载Windows安装包，安装就可以了。
## cmake的简单使用
### 编写C++文件
首先需要一个简单的Hello World程序。
HelloWorld.cpp
```cpp
#include <iostream>
using namespace std;
int main() {
  cout<<"Hello World"<<endl;
  return 0;
}
```
### 需要一个CMakeLists.txt
在cpp文件相同的目录下（项目cpp源文件根目录）建立一个CMakeLists.txt文件。
```
cmake_minimum_required (VERSION 2.6)
project (HelloWorld)
add_executable (HelloWorld HelloWorld.cpp)
```
### 运行cmake
到CMakeLists.txt文件所在目录，运行cmake。
`cmake .`
输出
```
D:\HelloWorld>cmake .
-- Building for: Visual Studio 14 2015
-- The C compiler identification is MSVC 19.0.23506.0
-- The CXX compiler identification is MSVC 19.0.23506.0
-- Check for working C compiler using: Visual Studio 14 2015
-- Check for working C compiler using: Visual Studio 14 2015 -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working CXX compiler using: Visual Studio 14 2015
-- Check for working CXX compiler using: Visual Studio 14 2015 -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: D:/HelloWorld
```
### 生成的.sln, .vcxproj文件
ID    文件名
1    HelloWorld.sln
2    ALL_BUILD.vcxproj
3    HelloWorld.vcxproj
4    ZERO_CHECK.vcproj
你可以用visual studio打开.sln文件来编译，运行，调试程序。
### 构建程序
`msbuild HelloWorld.sln`
然后就可以在Debug目录下面找到HelloWorld.exe程序。
## 总结
cmake给跨平台的C++程序提供了很好的构建解决方案。适合需要跨平台项目的使用。功能不多，简单，但还是比较实用的。建议阅读https://cmake.org/cmake-tutorial/ 。基本功能在tutorial里面都有介绍。
