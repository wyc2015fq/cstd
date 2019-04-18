# MATLAB面向对象编程import导入信息冲突解决方案 - 小灰笔记 - CSDN博客





2017年05月18日 23:37:43[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：443








      在使用MATLAB面向对象编程的包管理import导入时，如果出现导入的两个包中存在相同的类时就会出现我们期待之外的软件行为。因为，MATLAB的导入遵循的是“先入为主”的生效方式，也就是说后面的导入时无效的。然而，我们在新的一次导入结束后通常希望使用的都是新导入的类。这样，如果不使用合适的处理方式我们的设计目的就无法达成。

      在MATLAB包管理import导入的时候，为了避免类似导入的冲突可以在新的导入并调用之前先把之前导入的信息清空掉。也就是调用clear import操作命令。

      简单的演示如下：

![](https://img-blog.csdn.net/20170518233715855?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




      通过上面的结果可以看出，通过clear import指令操作之后，生效的类是最新导入的信息。



