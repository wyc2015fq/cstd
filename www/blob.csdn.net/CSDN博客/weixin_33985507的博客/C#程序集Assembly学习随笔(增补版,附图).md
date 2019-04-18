# C#程序集Assembly学习随笔(增补版,附图) - weixin_33985507的博客 - CSDN博客
2006年11月08日 13:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
【声明】本文章来源于[我的亲密战友Ax](http://www.cnblogs.com/AXzhz/archive/2006/10/30/544700.html)
我一直以为我是个人才,今天才TMD知道,我是个天才!  下面的问题一下就弄出来了.
上篇我讲了下我对Assembly的初步认知.
现在来说说怎么使用已有的Assembly.
比如说我在做一个解决方案时,发现你做的解决方案的某个项目中的类的方法(功能)我正好可以用到,哈,那我就不用写类了,直接考过来,不是考代码文件,而是考的那个需要的项目的Assembly....我现在就来解决这个问题.
【过程】
①打开解决方案,在【解决方案资源管理器】的项目上单击右键,选择【添加引用】,弹出【添加引用】对话框,单击【浏览】,找到并选定刚刚从你那Copy过来的Assembly,(假设为:ClassLibrary_AX)→【打开】→【确定】
现在,就可以使用你的类了!!!!!
②当然,要引用的地方,不要忘记引用: using ClassLibrary_AX;
OK了!!!!尽情使用Assembly里的类吧.
如下图:
![](https://images.cnblogs.com/cnblogs_com/axzhz/UsingAssembly.JPG)
③对于菜鸟,可能还是不清楚Assembly里面包含的类,请照下图操作
![](https://images.cnblogs.com/cnblogs_com/axzhz/ViewAssembly.JPG)
④至于这些类里面的方法的功能解释,在你使用方法的时候会看到,见下图
不好意思,因为写方法的时候没有写相应的注释,结果不能看出这个方法是干什么用的,当然你English好可以看到是计算奖金的方法,返回值是一个double类型.
可见写代码没注释是不行的.没有可读性的代码就是垃圾.
![](https://images.cnblogs.com/cnblogs_com/axzhz/Method.JPG)
【追加】IDE会把Assembly自动Copy到你当前项目的Debug文件夹下.
