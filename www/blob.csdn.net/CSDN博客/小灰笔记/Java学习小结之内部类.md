# Java学习小结之内部类 - 小灰笔记 - CSDN博客





2017年02月21日 23:33:35[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：154








内部类就是定义在另一个类里面的类，与之对应，包含内部类的类成为外部类。如此，加单的话描述一下内部类：类中类！




写一段测试代码：

![](https://img-blog.csdn.net/20170221232628157?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




编译以及运行结果;

![](https://img-blog.csdn.net/20170221232747583?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





从上面的结果中可以看出，经过编译的java程序生成了两个类文件，其中多出的一个就是内部类。而编译生成的内部类有着统一命名规则，那就是：外部类名+$+内部类名+扩展名。




从代码可以看出，内部类可以直接访问外部类的数据，包括私有数据。再对代码中内部类的使用做几个关键词的总结：

1， 类中类

2， 建上建

3， 必须使用外部对象来创建一个内部类对象，不能够直接创建内部类对象

4， 编译后的内部类文件有着统一的命名规则




以上是内部类最基本的东西，大致学习了一下内部类其他更为详细的知识点，感觉这部分信息很多实用方式上有点像C语言中的局部变量，可以拿这个来做简单的类比学习。



