# 008_shell创建只读量 - 小灰笔记 - CSDN博客





2018年11月05日 22:28:58[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：16
个人分类：[bash](https://blog.csdn.net/grey_csdn/article/category/7113865)









先看一下非常量：

![](https://img-blog.csdnimg.cn/20181105222757489.png)

         虽然上面的代码中，变量的名称中带着const，但是这其实是一个简单的变量的定义方式。而加上const是为了跟接下来的代码有一点对比的效果。代码的执行结果：

![](https://img-blog.csdnimg.cn/2018110522280556.png)

         再看一下只读量，方式1：

![](https://img-blog.csdnimg.cn/20181105222811898.png)

         执行，看得出效果中这个量已经是只读：

![](https://img-blog.csdnimg.cn/20181105222819685.png)

         上面的方式中，declare可以替换成typeset，这其实可以归为同一种方式。接下来看第二种：

![](https://img-blog.csdnimg.cn/20181105222830228.png)

         运行结果：

![](https://img-blog.csdnimg.cn/20181105222841517.png)

         看得出两种方式有着相同的效果。



