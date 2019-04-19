# (c++)web应用开发框架简单对比 - baidu_33570760的博客 - CSDN博客
2017年05月27日 10:46:02[carman_风](https://me.csdn.net/baidu_33570760)阅读数：8431
- - [TreeFrog](#treefrog)- [基本情况](#基本情况)- [介绍](#介绍)
- [特性](#特性)
- [安装](#安装)
- [tufao](#tufao)- [基本情况](#基本情况-1)- [介绍](#介绍-1)
- [它的特点](#它的特点)
- [安装](#安装-1)
- [CppCMS](#cppcms)- [基本情况](#基本情况-2)
- [介绍](#介绍-2)
- [安装要求](#安装要求)
- [Cinatra](#cinatra)- [基本情况](#基本情况-3)
- [介绍](#介绍-3)
- [如何使用](#如何使用)
## TreeFrog
### 基本情况
项目地址： 
[https://github.com/treefrogframework/treefrog-framework](https://github.com/treefrogframework/treefrog-framework)
官网： 
[http://www.treefrogframework.org/](http://www.treefrogframework.org/)
开源协议：BSD 
最近一次更新时间：2016-11-03
#### 介绍
TreeFrog Framework 是一个高速的全堆栈的 Web 应用开发框架，基于 C++ 和 Qt 。Web应用程序可以运行比脚本语言更快。在应用程序的开发，它提供了一个O / R映射的系统和模板系统的MVC架构，旨在通过约定优于配置的原则，以实现高生产率。
![](http://i.imgur.com/jrJHKju.png)
#### 特性：
- 高性能——多用户web依然高性能
- 框架用C++所写 —— web程序同样可以用C++写
- 模版系统 ——表示和逻辑完全分开
- 全栈 – 包括大多数模块
- 跨平台 – Windows, Mac OS X, Linux 等等，无需改写代码
- MVC架构
- O/R映射——隐藏复杂的数据库访问
- 路由系统 – 无需编写路由规则只需放入一个配置文件
- 生成器 ——自动生成框架和makefile
- AJAX支持 —— json，XML或者纯文本
- NQL数据库支持 —— 支持MongoDB
- 开源 —— BSD协议
#### 安装：
- 我们需要提前安装Qt库
- 下载文件编译
- 设置快捷方式
- 参数配置 
参考链接：[http://treefrogframework.github.io/treefrog-framework/user-guide/en/install/](http://treefrogframework.github.io/treefrog-framework/user-guide/en/install/)
## tufao
### 基本情况
项目地址：[https://github.com/vinipsmaker/tufao](https://github.com/vinipsmaker/tufao)
主页：[http://vinipsmaker.github.io/tufao/](http://vinipsmaker.github.io/tufao/)
开源协议：LGPLv2  
最近一次更新时间： 2017-04-17
#### 介绍
Tufão是C ++的Web框架，它利用了Qt的对象通信系统（信号和插槽）。
#### 它的特点：
- 
高性能独立服务器
- 
跨平台支持
- 
良好的文档
- 
支持现代的HTTP特性
- 
持续流
- 
分块实体
- 
100-continue status
- 
WebSocket
- 
支持HTPPS
- 
灵活的路由请求
- 
支持条件请求的静态文件服务器支持条件请求，部分下载和自动检测 mime
- 
基于插件的服务器支持更改运行中的代码，无需重启程序。
- 
灵活和安全的会话支持
- 
QtCreator 插件，允许快速创建新的应用程序
- 
经过了大量的测试
- 
超时支持
- 
C++11
- 
采用Ryan Dahl’s HTTP分析器以提供更好的性能
#### 安装
- 确保你有QT和CMake
- 编译配置
- 环境配置 
参考链接：[https://github.com/vinipsmaker/tufao#build](https://github.com/vinipsmaker/tufao#build)
## CppCMS
### 基本情况
主页：[http://cppcms.com/wikipp/en/page/main](http://cppcms.com/wikipp/en/page/main)
项目地址：[https://sourceforge.net/projects/cppcms/files/](https://sourceforge.net/projects/cppcms/files/)
授权协议:LGPL
开发语言:C/C++
操作系统:跨平台
最近一次更新时间：2014-10-30
### 介绍
CppCMS是一个C++的Web开发框架（不是一个CMS） 。它不同于大多数其他Web开发框架，如巨蟒Django ， Java的Servlets ，或C++ Wt因为它在设计和调整来处理极高的负荷，而且它的目的是发展的网站，而不是“GUI-Like” Web应用程序 
![](http://i.imgur.com/G0NwJlD.png)
### 安装要求
强制性要求
- 现代C ++编译器 -  GCC，MSVC 9，Intel。请参阅受支持的编译器和平台
- CMake 2.6及以上版本，建议使用2.8.x。
- Zlib库
- PCRE库。
- Python> = 2.4（但不是3）
参考链接：[http://cppcms.com/wikipp/en/page/cppcms_1x_build](http://cppcms.com/wikipp/en/page/cppcms_1x_build)
## Cinatra
### 基本情况
项目地址：[https://github.com/topcpporg/cinatra](https://github.com/topcpporg/cinatra)
授权协议:LGPL
开发语言:C/C++
操作系统:Linux
最近一次更新时间：2017-01-20
### 介绍
cinatra是C++开源社区–purecpp发起的一个开源项目，现在正式发布第一个版本cinatra0.9.0，cinatra是一个现代C++写的web framework，它的目的是给用户提供一个易用、灵活和高性能的web框架，让用户能完全专注于核心逻辑而无需关注http细节。它的灵感来源于sinatra，但又有自己的特色。
Cinatra的设计非常简单，只有几个组件，下面是Cinatra的逻辑视图 
![](http://i.imgur.com/8BsrJkG.png)
### 如何使用
- 从github上下载源码。
- 安装boost，因为框架用到asio和coroutine，需要1.57及以上的版本。
- 编译。已经提供vs2013的工程文件和Cmakelist，直接在win和linux平台下编译即可
