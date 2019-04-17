# Python安装第三方包时报错：Microsoft Visual C++ 14.0 is required - ChihkAnchor的博客 - CSDN博客





2018年12月05日 21:01:44[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：276








在使用Python时跑代码的时候，有条件的尽量不要在Windows中运行，这个系统总是会在不经意间制造一点惊喜，真是令人头大

今天使用pip安装 pyahocorasick 的时候，报错Microsoft Visual C++ 14.0 is required，上网搜索说手动下载一个whl并给出了一个网址： https://www.lfd.uci.edu/~gohlke/pythonlibs/

上去看了一下，安装包挺多的，但是那么多还是没有我要的 pyahocorasick，无奈！

然后上官网把源码文件下载下来试图手动安装，还是不行。

官网：[https://pypi.python.org/pypi/pyahocorasick/](https://pypi.python.org/pypi/pyahocorasick/)

那就只能想其他办法了，真的勇士敢于直面bug，直接搜Microsoft Visual C++ 14.0 is required，这个是因为确实C++编译器导致的，然后下载了一个MinGW，然鹅，，，，

那，，，换一种方式：

[https://964279924.ctfile.com/fs/1445568-239446865](https://964279924.ctfile.com/fs/1445568-239446865)，下载并安装 **[Microsoft visual c++ 14.0](https://964279924.ctfile.com/fs/1445568-239446865)**

![](https://img-blog.csdnimg.cn/20181205205911744.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

然后 pip install pyahocorasick ，OK



