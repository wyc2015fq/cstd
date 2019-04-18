# 查看Eclipse 运行java程序时使用的命令 - z69183787的专栏 - CSDN博客
2014年02月25日 16:41:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1994
昨天在将的的java工程打后运行时总是抛出莫名其妙的错误，可是工程用Eclipse运行却是没有问题呀，是不是我的bat里面运行程序的命令行写错了呀？可是哪里错了呢，我想到了既然Eclipse能够运行工程，那么它的命令行应该没有问题呀，可是怎么查看Eclipse的命令行呢？研究了一下，终于有结果了赶紧分享一下： 
　　1.打开Eclipse的Debug视图 
　　2.运行工程，然后在Debug视窗中找到运行的工程的主线程，点出右键菜单，如图： 
![](http://byfiles.storage.live.com/y1pjzqtpTuM9GSslq6zExC5oYrltbdSNf7SlqjZT-j0fHCIG1NP_rxynuxDnAMZo7PQL7RmkQ6dZU4)
　　3.在弹出的菜单中选中Properties菜单项，弹出如下对话框： 
![](http://byfiles.storage.live.com/y1pjzqtpTuM9GQJuesgSLoPYJuVCpdZuMNmIC4mwmXDv1ZyUNAVXitqqPNfJpc6LyU9wHCdZamHR4U)
　　4.看到Command line对话框了吧，把里面的东东拷下来就是了。 
5.删除javaw命令后面的调试用参数  （ps.增补）
　　最后我终于明白了，我是用java命令运的程序，人家Eclipse是用javaw运行的，哎
原文地址：http://www.cnblogs.com/meetrice/archive/2012/07/13/2589886.html
