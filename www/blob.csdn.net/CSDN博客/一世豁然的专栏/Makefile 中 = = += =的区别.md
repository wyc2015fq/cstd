# Makefile 中:= ?= += =的区别 - 一世豁然的专栏 - CSDN博客





2015年06月25日 20:00:40[一世豁然](https://me.csdn.net/Explorer_day)阅读数：468标签：[makefile](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)









本博客转载于：[http://www.cnblogs.com/wanqieddy/archive/2011/09/21/2184257.html](http://www.cnblogs.com/wanqieddy/archive/2011/09/21/2184257.html)







在Makefile中我们经常看到 = := ?= +=这几个赋值运算符，那么他们有什么区别呢？我们来做个简单的实验


新建一个Makefile，内容为：

ifdef DEFINE_VRE

    VRE = “Hello World!”

else

endif


ifeq ($(OPT),define)

    VRE ?= “Hello World! First!”

endif


ifeq ($(OPT),add)

    VRE += “Kelly!”

endif


ifeq ($(OPT),recover)

    VRE := “Hello World! Again!”

endif


all:

    @echo $(VRE)


敲入以下make命令：

make DEFINE_VRE=true OPT=define 输出：Hello World!

make DEFINE_VRE=true OPT=add 输出：Hello World! Kelly!

make DEFINE_VRE=true OPT=recover  输出：Hello World! Again!

make DEFINE_VRE= OPT=define 输出：Hello World! First!

make DEFINE_VRE= OPT=add 输出：Kelly!

make DEFINE_VRE= OPT=recover 输出：Hello World! Again!


从上面的结果中我们可以清楚的看到他们的区别了

= 是最基本的赋值

:= 是覆盖之前的值

?= 是如果没有被赋值过就赋予等号后面的值

+= 是添加等号后面的值




之前一直纠结makefile中“=”和“:=”的区别到底有什么区别，因为给变量赋值时，两个符号都在使用。网上搜了一下，有人给出了解答，但是本人愚钝，看不懂什么意思。几寻无果之下，也就放下了。今天看一篇博客，无意中发现作者对于这个问题做了很好的解答。解决问题之余不免感叹，有时候给个例子不就清楚了吗？为什么非要说得那么学术呢。^_^


      1、“=”


      make会将整个makefile展开后，再决定变量的值。也就是说，变量的值将会是整个makefile中最后被指定的值。看例子：

x = foo

            y = $(x) bar

            x = xyz

在上例中，y的值将会是 xyz bar ，而不是 foo
 bar 。

      2、“:=”

      “:=”表示变量的值决定于它在makefile中的位置，而不是整个makefile展开后的最终值。

x := foo

            y := $(x) bar

            x := xyz


      在上例中，y的值将会是 foo bar ，而不是 xyz bar 了。




