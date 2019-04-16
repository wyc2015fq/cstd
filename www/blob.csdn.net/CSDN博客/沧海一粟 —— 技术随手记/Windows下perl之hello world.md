# Windows下perl之hello world - 沧海一粟 —— 技术随手记 - CSDN博客





2011年01月05日 14:30:00[慢游](https://me.csdn.net/eengel)阅读数：1855标签：[perl																[windows																[command																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[Perl](https://blog.csdn.net/eengel/article/category/773640)





快过年了，忽然想管管硬盘上庞大的文件系统。该存档的存档，该删的删。

听闻perl在文件处理上的强大实力，于是决定学习一下。



装了[Active Perl](http://www.activestate.com/activeperl)
后，总得写个hello world试试手。

本人是windows环境配置盲。按照网上的帖子，先中规中矩建一个文件test.pl，输入

#!usr/bin/perl

print "Hello World!";

然后在command命令行里运行

>perl test.pl

果然出现了亲切的Hello World!



不过一会儿，perl秉承的“懒惰”习性就出来了：每次调试一点代码都要在文件和command之间切换，还要保存文件，实在麻烦。

于是尝试在command中直接运行perl:

>perl

如愿以偿进入可编辑环境，窃喜！再次尝试hello world：

>print "Hello World!";

可惜回车后什么也没发生。。困扰着我的是如何标记我的代码已经结束了可以运行了？

费了九牛二虎之力并急中生智，尝试了无数我认为的结束代码，终于被我发现ctrl+c 或ctrl+z才是天命所归！



微不足道一件小事，之于个人也是一点小小的进步啊！记录之。](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=perl&t=blog)




