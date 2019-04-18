# 013_bash中的加法 - 小灰笔记 - CSDN博客





2018年11月08日 22:25:42[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：98








调试了一个小程序，按照C语言的习惯来调试，发现结果一直不是预期。打印了一下中间状态发现bash中的四则运算跟我想象中的差异还是挺大的，似乎bash主要还是面向字符串的处理工具。

简单总结一下几个学习过程中遇到的错误：

代码

![](https://img-blog.csdnimg.cn/20181108222523946.png)

         我期待的是输出2，结果输出值：

![](https://img-blog.csdnimg.cn/2018110822252466.png)

         疑惑中，改成了以下代码：

![](https://img-blog.csdnimg.cn/2018110822252458.png)

         执行结果：

![](https://img-blog.csdnimg.cn/2018110822252477.png)

         看起来，这些操作全都是字符串或者字符类的操作而不是数字的操作。

         后来，查了一下找到了一个 let命令，算是初步解决了我的疑惑。修改代码如下：

![](https://img-blog.csdnimg.cn/2018110822252481.png)

         执行结果：

![](https://img-blog.csdnimg.cn/2018110822252487.png)

         同时，找到了一种简写方式，虽然我不觉得这个有多简单。几行示范代码修改如下：

![](https://img-blog.csdnimg.cn/2018110822252462.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         执行结果：

![](https://img-blog.csdnimg.cn/20181108222524166.png)

         先从let入手吧，先能够掌握一种方法然后再进行知识体系的扩充。



