# 010_bash脚本的参数传递 - 小灰笔记 - CSDN博客





2018年11月06日 22:40:40[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：50








参数传递是一个很好的功能，在其他脚本语言中我使用很频繁。主要的作用可以让脚本得以复用。简单看一下bash中的参数传递：

         如同Perl中的$ARGV[x]，bash中使用更简单的$n。其中$1-$9代表前9个参数，而$0是文件名称。说起来，这个应该更像Python或者C语言。

         简单测试：

![](https://img-blog.csdnimg.cn/20181106224021454.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         执行结果：

![](https://img-blog.csdnimg.cn/20181106224021494.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         改一下参数：

![](https://img-blog.csdnimg.cn/20181106224021501.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         有意思的现象出现了：超过9，不准确！

         对代码的其他解释：

                   $#，代表参数个数

                   $0，文件名称

                   $*，全部参数

                   $@，全部参数

         其实，最后两个全部参数是有差异的。但是目前我掌握的简单技能还不能够通过代码对这个进行一个准确的描述，作为后面我自己学习的一个问题点记录吧！

        再补充一下：如果参数少于代码中的接收参数的变量时，剩下的接收变量全都赋值为空。



