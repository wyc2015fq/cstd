# source insight支持.S的汇编文件 - xqhrs232的专栏 - CSDN博客
2010年05月13日 10:48:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4482标签：[汇编																[filter																[file																[x86](https://so.csdn.net/so/search/s.do?q=x86&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=filter&t=blog)](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)
个人分类：[SI/Notepad++/EditPlus](https://blog.csdn.net/xqhrs232/article/category/906927)
原文地址::
[http://www.91linux.com/html/article/program/asm/200912/16-18325.html](http://www.91linux.com/html/article/program/asm/200912/16-18325.html)
用source insight看blob以及内核中的代码，发现即使全局搜索，也找不到定义中后缀为.S的函数，而明明在*.S中用汇编定义了该函数的。去网上查了一下，发现原因是*.S文件并没有添加到改工程中来。
为了用source insight能搜索到.S的汇编文件，必须把该文件添加进来。 
方法： 
1、在创建工程前先特别设置好source insight 。设置如下： 
Options->Document Options->Doucment Types->选择x86 Assemble，再在右边File Filter里输入*.S。这样以后再new一个工程，在添加文件时，就可以把文件夹中的*.S添加进去了。
2、如果之前已经创建好工程的，而又想把*.S文件添加进来。如下： 
打开创建好的工程，Options->Document Options->Doucment Types->选择x86 Assemble，再在右边File Filter里输入*.S。然后project-->add and remove project files,再重新把工程的所有文件夹添加一遍，这样就可以把*.S文件添加进来了（注：原来的*.c等文件不会被重复添加），然后重新rebuild就可以了。
本文来自: ([www.91linux.com](http://www.91linux.com)) 详细出处参考：[http://www.91linux.com/html/article/program/asm/200912/16-18325.html](http://www.91linux.com/html/article/program/asm/200912/16-18325.html)
