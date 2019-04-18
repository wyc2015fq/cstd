# 009_shell中从标准输入信息 - 小灰笔记 - CSDN博客





2018年11月06日 22:22:36[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：33
个人分类：[bash](https://blog.csdn.net/grey_csdn/article/category/7113865)









主要涉及到的一个命令是read，使用示范：

![](https://img-blog.csdnimg.cn/20181106222218111.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

代码解释：

第3行的-n：提示输入信息输出后不换行

第4行：读入的信息读入到line

第7行：不同于第3行，这个输出提示信息后换行

第8行：读取三个变量，读取信息以空格分开。

回车表示输入。



几个注意点：
- 输入比变量多的时候，多出来的信息连同最后一个赋值给最后一个量
- 输入少于变量，剩下的变量赋值为空。



运行1：

![](https://img-blog.csdnimg.cn/20181106222218183.png)

运行2：

![](https://img-blog.csdnimg.cn/20181106222218183.png)

运行3：

![](https://img-blog.csdnimg.cn/20181106222218197.png)



         算是初期学习bash，还不知道这个是否可以用于管道命令等高级处理方式。不过，目前这个功能可以是一个能力练习的好东西。



