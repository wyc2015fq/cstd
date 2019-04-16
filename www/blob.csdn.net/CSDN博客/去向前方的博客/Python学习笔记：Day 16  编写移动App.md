# Python学习笔记：Day 16   编写移动App - 去向前方的博客 - CSDN博客





2018年07月11日 16:06:41[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：204








# 前言

最近在学习深度学习，已经跑出了几个模型，但Pyhton的基础不够扎实，因此，开始补习Python了，大家都推荐廖雪峰的课程，因此，开始了学习，但光学有没有用，还要和大家讨论一下，因此，写下这些帖子，廖雪峰的课程连接在这里：[廖雪峰](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000)

Python的相关介绍，以及它的历史故事和运行机制，可以参见这篇：[python介绍](https://blog.csdn.net/lvsehaiyang1993/article/details/80644237)

Python的安装可以参见这篇：[Python安装](https://blog.csdn.net/lvsehaiyang1993/article/details/80644321)

Python的运行模式以及输入输出可以参见这篇：[Python IO](https://blog.csdn.net/lvsehaiyang1993/article/details/80644827)

Python的基础概念介绍，可以参见这篇：[Python 基础](https://blog.csdn.net/lvsehaiyang1993/article/details/80647010)

Python字符串和编码的介绍，可以参见这篇：[Python字符串与编码](https://blog.csdn.net/lvsehaiyang1993/article/details/80648947)

Python基本数据结构：list和tuple介绍，可以参见这篇：[Python list和tuple](https://blog.csdn.net/lvsehaiyang1993/article/details/80649384)

Python控制语句介绍：ifelse，可以参见这篇：[Python 条件判断](https://blog.csdn.net/lvsehaiyang1993/article/details/80649552)

Python控制语句介绍：循环实现，可以参见这篇：[Python循环语句](https://blog.csdn.net/lvsehaiyang1993/article/details/80651808)

Python数据结构：dict和set介绍[Python数据结构dict和set](https://blog.csdn.net/lvsehaiyang1993/article/details/80652307)

Python函数相关：[Python函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80654173)

Python高阶特性：[Python高级特性](https://blog.csdn.net/lvsehaiyang1993/article/details/80661973)

Python高阶函数：[Python高阶函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80665892)

Python匿名函数：[Python匿名函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80667629)

Python装饰器：[Python装饰器](https://blog.csdn.net/lvsehaiyang1993/article/details/80667177)

Python偏函数：[Python偏函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80673522)

Python模块：[Python模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80673944)

Python面向对象编程（1）：[Python面向对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80674239)

Python面向对象编程（2）：[Python面向对象（2）](https://blog.csdn.net/lvsehaiyang1993/article/details/80675404)

Python面向对象编程（3）：[Python面向对象（3）](https://blog.csdn.net/lvsehaiyang1993/article/details/80675873)

Python面向对象编程（4）：[Pyhton面向对象（4）](https://blog.csdn.net/lvsehaiyang1993/article/details/80676298)

Python面向对象高级编程（上）：[Python面向对象高级编程（上）](https://blog.csdn.net/lvsehaiyang1993/article/details/80688026)

Python面向对象高级编程（中上）：[Python面向对象高级编程（中上）](https://blog.csdn.net/lvsehaiyang1993/article/details/80689908)

Python面向对象高级编程（中下）：[Python面向对象高级编程（中下）](https://blog.csdn.net/lvsehaiyang1993/article/details/80691702)

Python面向对象高级编程（完）：[Python面向对象高级编程（完）](https://blog.csdn.net/lvsehaiyang1993/article/details/80692002)

Python错误调试（起）：[Python调试：起](https://blog.csdn.net/lvsehaiyang1993/article/details/80694444)

Python错误调试（承）：[Python调试：承](https://blog.csdn.net/lvsehaiyang1993/article/details/80694819)

Python错误调试（转）：[Python调试：转](https://blog.csdn.net/lvsehaiyang1993/article/details/80695280)

Python错误调试（合）：[python调试：合](https://blog.csdn.net/lvsehaiyang1993/article/details/80697743)

Python文件IO编程：[Python文件IO](https://blog.csdn.net/lvsehaiyang1993/article/details/80700751)

Python文件IO编程2：[Python文件IO2](https://blog.csdn.net/lvsehaiyang1993/article/details/80707102)

Python文件IO编程3：[PYthon文件IO3](https://blog.csdn.net/lvsehaiyang1993/article/details/80707554)

Python进程和线程（起）：[Python进程和线程起](https://blog.csdn.net/lvsehaiyang1993/article/details/80709180)

Python进程和线程（承）：[Python进程和线程承](https://blog.csdn.net/lvsehaiyang1993/article/details/80709260)

Python进程和线程（转）：[Python进程和线程转](https://blog.csdn.net/lvsehaiyang1993/article/details/80709353)

Python进程和线程（合）：[Python进程和线程合](https://blog.csdn.net/lvsehaiyang1993/article/details/80709405)

Python正则表达式：[Python正则表达式](https://blog.csdn.net/lvsehaiyang1993/article/details/80711331)

Python学习笔记:常用内建模块1：[Python学习笔记：常用内建模块1](https://blog.csdn.net/lvsehaiyang1993/article/details/80712912)

Python学习笔记:常用内建模块2：[Python学习笔记：常用内建模块2](https://blog.csdn.net/lvsehaiyang1993/article/details/80713218)

Python学习笔记:常用内建模块3：[Python学习笔记：常用内建模块3](https://blog.csdn.net/lvsehaiyang1993/article/details/80713747)

Python学习笔记:常用内建模块4：[Python学习笔记:   常用内建模块4](https://blog.csdn.net/lvsehaiyang1993/article/details/80716665)

Python学习笔记:常用内建模块5：[Python学习笔记:   常用内建模块5](https://blog.csdn.net/lvsehaiyang1993/article/details/80716912)

Python学习笔记:常用内建模块6:[Python学习笔记：常用内建模块6](https://blog.csdn.net/lvsehaiyang1993/article/details/80717102)

Python学习笔记:第三方模块1：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726046)

Python学习笔记:第三方模块2：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726301)

Python学习笔记:第三方模块3：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726378)

Pytho学习笔记：网络编程：[Python网络编程](https://blog.csdn.net/lvsehaiyang1993/article/details/80729939)

Python学习笔记：电子邮件：[Python电子邮件1](https://blog.csdn.net/lvsehaiyang1993/article/details/80731086)

Python学习笔记：SMTP服务器：[PythonSMTP服务器](https://blog.csdn.net/lvsehaiyang1993/article/details/80731573)

Python学习笔记：POP3服务器：[PythonPOP3服务器](https://blog.csdn.net/lvsehaiyang1993/article/details/80731697)

Python学习笔记：Python数据库 [Python数据库1](https://blog.csdn.net/lvsehaiyang1993/article/details/80742388)

Python学习笔记：Python数据库2[Python数据库2](https://blog.csdn.net/lvsehaiyang1993/article/details/80747424)

Python学习笔记：web开发1[Python学习笔记：web开发1](https://blog.csdn.net/lvsehaiyang1993/article/details/80765955)

Python学习笔记：web开发2[Python学习笔记: web开发2](https://blog.csdn.net/lvsehaiyang1993/article/details/80778507)

Python学习笔记:  web开发3[Python学习笔记: web开发3](https://blog.csdn.net/lvsehaiyang1993/article/details/80779521)

Python学习笔记：异步IO（1）[Python学习笔记：异步IO（1）](https://blog.csdn.net/lvsehaiyang1993/article/details/80789370)

Python学习笔记：异步IO（2）[Python学习笔记：异步IO（2）](https://blog.csdn.net/lvsehaiyang1993/article/details/80789574)

Python学习笔记：异步IO（3）[Python学习笔记：异步IO（3）](https://blog.csdn.net/lvsehaiyang1993/article/details/80795526)

Python学习笔记：Day 1-2开发[Python学习笔记：Day1-2开发](https://blog.csdn.net/lvsehaiyang1993/article/details/80807565)

Python学习笔记：Day 3 ORM[Python学习笔记：Day3 ORM](https://blog.csdn.net/lvsehaiyang1993/article/details/80842307)

Python学习笔记：Day 4 Model[Python学习笔记：Day4Model](https://blog.csdn.net/lvsehaiyang1993/article/details/80845475)

Python学习笔记：Day 5 web框架[PYTHON学习笔记：DAy5](https://blog.csdn.net/lvsehaiyang1993/article/details/80866237)

Python学习笔记：Day 6 配置文件[Python学习笔记：Day 6 配置文件](https://blog.csdn.net/lvsehaiyang1993/article/details/80875144)

Python学习笔记：Day 7 编写MVC[Python学习笔记：Day 7编写MVC](https://blog.csdn.net/lvsehaiyang1993/article/details/80880125)

Python学习笔记：Day 8 构建前端[Python学习笔记:Day 8 构建前端](https://blog.csdn.net/lvsehaiyang1993/article/details/80899296)

Python学习笔记：Day 9 编写API[Python 9编写API](https://blog.csdn.net/lvsehaiyang1993/article/details/80914717)

Python学习笔记：Day 10 用户注册和登陆[Python 10用户注册和登陆](https://blog.csdn.net/lvsehaiyang1993/article/details/80921837)

Python学习笔记：Day11 编写日志创建页[day11 编写目录](https://blog.csdn.net/lvsehaiyang1993/article/details/80935496)

Python学习笔记：Day12 编写日志列表页[day12 编写日志](https://blog.csdn.net/lvsehaiyang1993/article/details/80949143)

Python学习笔记：Day13 提升开发效率[开发效率](https://blog.csdn.net/lvsehaiyang1993/article/details/80956748)

Python学习笔记：Day14 完成Web App[完成web app](https://blog.csdn.net/lvsehaiyang1993/article/details/80972844)

Python学习笔记：Day15 部署Web App [部署 web app](https://blog.csdn.net/lvsehaiyang1993/article/details/80981079)
# 目录

[TOC] 

网站部署上线后，还缺点啥呢？

在移动互联网浪潮席卷而来的今天，一个网站没有上线移动App，出门根本不好意思跟人打招呼。

所以，awesome-python3-webapp必须得有一个移动App版本！

开发iPhone版本 

我们首先来看看如何开发iPhone App。前置条件：一台Mac电脑，安装XCode和最新的iOS SDK。

在使用MVVM编写前端页面时，我们就能感受到，用REST API封装网站后台的功能，不但能清晰地分离前端页面和后台逻辑，现在这个好处更加明显，移动App也可以通过REST API从后端拿到数据。

我们来设计一个简化版的iPhone App，包含两个屏幕：列出最新日志和阅读日志的详细内容：

![这里写图片描述](https://img-blog.csdn.net/20180711160336552?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

只需要调用API：/api/blogs。

在XCode中完成App编写：

![这里写图片描述](https://img-blog.csdn.net/2018071116034692?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

由于我们的教程是Python，关于如何开发iOS，请移步[Develop Apps for iOS](https://developer.apple.com/ios/)。

点击下载[iOS App源码](https://github.com/michaelliao/awesome-python3-webapp/tree/day-16/ios)。

如何编写Android App？这个当成作业了。

# 尾声

这个课程以及练习就到此为止了，现在开始进行深度学习的学习。




