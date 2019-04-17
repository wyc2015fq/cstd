# MATLAB面向对象编程import使用 - 小灰笔记 - CSDN博客





2017年05月18日 23:19:43[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：879








      类似于Python以及Java，针对模块库或者包中的内容可以通过import指令进行模块或者功能的导入。这样，可以在一定程度上简化代码，提高可读性。

      例如之前在博客中实现的包管理例程，进行一个常用类的导入，可以通过以下方式：

![](https://img-blog.csdn.net/20170518231833200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




      这样，如果直接使用MyClassA即可实现调用Pack1中的类。此时如果要使用Pack2中的类，依然需要指明包。

      如果有一个包中所有的类或者方法使用的都比较频繁，可以使用通配符导入所有的信息。例如，使用上面的例程继续演示如下：

>> importPack1.*

>> B =MyClassB(2,2)



B = 



  MyClassB with properties:



      age: 2

hight: 2                     

此时，Pack1中的类全都可以直接省去包的名称直接使用。即使是没有第一次演示导入MyClassA，此刻MyClassA也一样可以直接使用。

说起来，这种类似的功能并不是很陌生。尤其是在我使用的比较频繁的Python中，这种功能使用还是很寻常的。



