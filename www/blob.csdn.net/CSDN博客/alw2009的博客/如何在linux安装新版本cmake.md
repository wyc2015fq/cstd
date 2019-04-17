# 如何在linux安装新版本cmake - alw2009的博客 - CSDN博客





2018年12月29日 22:37:58[traveler_zero](https://me.csdn.net/alw2009)阅读数：41标签：[编程																[CMake](https://so.csdn.net/so/search/s.do?q=CMake&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[CMake																[工具																[编程](https://blog.csdn.net/alw2009/article/category/8573084)




cmake是一款非常著名的跨平台make工具， 今天看一下如何在Linux上安装新版本的cmake。 包含以下步骤：

### 一、下载安装包 

下载地址：[https://cmake.org/download/](https://cmake.org/download/)

```
wget https://github.com/Kitware/CMake/releases/download/v3.13.2/cmake-3.13.2.tar.gz
```

### 二、编译安装

```
tar xf cmake-3.13.2.tar.gz
cd cmake-3.13.2
./bootstrap --prefix=$HOME/local
make && make install
```

### 三、检查安装

```
export PATH=$HOME/local/bin:$PATH
cmake --version

    cmake version 3.13.2

    CMake suite maintained and supported by Kitware (kitware.com/cmake).
```](https://blog.csdn.net/alw2009/article/category/8564835)](https://blog.csdn.net/alw2009/article/category/8573061)





