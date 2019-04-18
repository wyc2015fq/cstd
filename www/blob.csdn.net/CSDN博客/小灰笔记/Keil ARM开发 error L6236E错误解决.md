# Keil ARM开发 error L6236E错误解决 - 小灰笔记 - CSDN博客





2017年09月03日 00:39:51[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2594








顺利创建了第一个Keil工程却发现不能完成链接，出现了一个下面这样的报错：

.\Objects\demo_simple.sct(7): error: L6236E: No section matches selector - no section to be FIRST/LAST.




通过反复尝试发现，出现这个问题的原因是缺少了启动代码。只需要在pack管理中增加启动代码即可。具体解决方法如下：

1，通过如下操作进入到pack管理 

![](https://img-blog.csdn.net/20170903003849401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2，增加启动代码的勾选 

![](https://img-blog.csdn.net/20170903003900297?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





3，点击确认后会发现工程中增加了启动代码文件，同时，软件工程的编译以及链接也全都没有问题通过。 

![](https://img-blog.csdn.net/20170903003912461?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




