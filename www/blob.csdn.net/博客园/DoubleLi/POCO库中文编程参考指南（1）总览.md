# POCO库中文编程参考指南（1）总览 - DoubleLi - 博客园






# POCO库中文编程参考指南（1）总览
- 作者：柳大·Poechant
- 博客：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)
- 邮箱：zhongchao.ustc#gmail.com (# -> @)
- 日期：April 14th, 2012

## 1 What is POCO?

POCO 是什么？这是我最常被人问到的。POCO 是 POrtable COmponents 的缩写，官方主页：[POCOProject.org](http://pocoproject.org/)。是一个非常出色的 C++ 库，相当于 Java 的 Class Library、.NET 的 Framework 和 Apple 的 Cocoa。它与 Boost 是非常不同的，简单说 Boost 就是相当于 Advanced STL。

## 2 本系列文章所使用的 POCO 版本

```
POCO C++ Library 1.4.3p1 (2012-01-23)
```

## 3 库逻辑结构

下图摘自 POCO 官网。OS API 是使用操作系统层的，C++/C Standard Libararies 是标准库，其他的模块都是 POCO 所提供的，包括：Foundation、XML、Crypto、XML、Util、Net、Zip、NetSSL、Data（支持 SQLite、ODBC、MySQL）、Tools & Utilities、Application。

![Resize icon](http://img.my.csdn.net/uploads/201204/14/1334412487_6587.png)

## 4 支持平台

Desktop/Server: - Windows - Linux - Mac OS X - Solaris - HP-UX - AIX

Embedded: - Windows Embedded CE - Embedded Linux (uClibc or glibc) - iOS - QNX - VxWorks - Android

## 5 Namespaces
- Poco
- Poco::Crypto
- Poco::Data

> - Poco::Data::MySQL
- Poco::Data::ODBC
- Poco::Data::SQLite


- Poco::Net
- Poco::Util

> - Poco::Util::Units
- Poco::Util::Units::Constants
- Poco::Util::Units::Internal
- Poco::Util::Units::Values


- Poco::XML
- Poco::Zip

-

from：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)









