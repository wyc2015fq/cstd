# anaconda在Linux下的安装 - qq_37385726的博客 - CSDN博客





2018年08月15日 14:38:09[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：54
个人分类：[七里八里](https://blog.csdn.net/qq_37385726/article/category/6706622)









1.从官网下载Linux版本的anaconda，[https://www.anaconda.com/download/](https://www.anaconda.com/download/)

2.安装anaconda，执行下列命令

 bash Anaconda2-5.0.0.1-Linux-x86_64.sh 

![](https://img-blog.csdn.net/20171026100648890)



3.在安装过程中会显示配置路径

Prefix=/home/jsy/anaconda2/

4.安装完之后，运行python，仍是ubuntu自带的python信息，需自己设置下环境变量

5.在终端输入$sudo gedit /etc/profile，打开profile文件

6.在文件末尾添加一行：export PATH=/home/jsy/anaconda2/bin:$PATH，其中，将“/home/jsy/anaconda2/bin”替换为你实际的安装路径，保存。

![](https://img-blog.csdn.net/20171026100654406)



7.重启Linux

8.打开终端，输入python，如果出现如下界面，表明设置成功。

![](https://img-blog.csdn.net/20171026100659839)



你还可以用conda info 来查询安装信息

输入conda list 可以查询你现在安装了哪些库，常用的python, numpy, scipy名列其中。

如果你还有什么包没有安装上，可以运行conda install ***  来进行安装(***代表包名称），如果某个包版本不是最新的，运行conda update *** 就可以了。



参考网址：

在Linux中设置Anaconda环境变量：http://www.linuxidc.com/Linux/2016-08/134259.htm

使用conda查询信息：http://www.cnblogs.com/denny402/p/5849416.html



