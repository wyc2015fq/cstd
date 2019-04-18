# 轻量级web服务器mongoose - 深之JohnChen的专栏 - CSDN博客

2016年08月27日 16:46:04[byxdaz](https://me.csdn.net/byxdaz)阅读数：7127


﻿Mongoose是一个web服务器，通过提供一个web接口给它，它可以嵌入到现有的应用程序中去。Mongooseweb服务器的执行是自满足的，它不依赖于任何其他服务。如果你将它复制到任何目录并执行，它将将启动web服务并将当前目录作为主目录、端口号是8080。当然这些配置选项都可以通过配置文件mongoose.conf设置。

Mongoose它完全开源和自由使用。它还具有如下特性：

跨平台——Windows、MacOS、大多数UNIX

CGI, SSL, SSI,Digest (MD5) authorization, resumed download, aliases

IP-based ACL,Windows service, GET, POST, HEAD, PUT, DELETE methods

Smallfootprint: executable size is 40 kB on Linux 2.6 i386 system, 60 kB on Windowssystem

**能够用简单和干净的API嵌入到**应用程序中

Languagesuport for:

C/C++ (native)

Python - sinceversion 2.6**(done)**

C# - sinceversion 2.7**(done)**

Ruby - sinceversion 2.9**(todo)**

Lua - sinceversion 2.9**(todo)**

mongoose的代码着实轻量，整个代码也只有一个mongoose.c和mongoose.h两个文件，从引入第三方的考虑上也着实不多。实现的功能还是非常多的，从使用的层面上来说功能还是比较全面。它的代码只用了两个源文件罢了。诸多的功能也大以宏的开始与结束来区分。

资料：

mongoose的源码地址：https://github.com/cesanta/mongoose

mongoose的用户手册：[https://www.cesanta.com/developer/mongoose](https://www.cesanta.com/developer/mongoose)

mongoose安装：[http://www.cnblogs.com/skynet/archive/2010/07/24/1784322.html](http://www.cnblogs.com/skynet/archive/2010/07/24/1784322.html)

Mongoose数据结构篇：[http://www.cnblogs.com/skynet/archive/2010/07/25/1784710.html](http://www.cnblogs.com/skynet/archive/2010/07/25/1784710.html)

mongoose的工作模型：[http://www.cnblogs.com/skynet/archive/2010/07/24/1784476.html](http://www.cnblogs.com/skynet/archive/2010/07/24/1784476.html)

核心处理模块：[http://www.cnblogs.com/skynet/archive/2010/07/25/1784710.html](http://www.cnblogs.com/skynet/archive/2010/07/25/1784710.html)


