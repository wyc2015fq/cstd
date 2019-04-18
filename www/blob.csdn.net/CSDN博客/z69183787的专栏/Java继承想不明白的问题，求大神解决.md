# Java继承想不明白的问题，求大神解决 - z69183787的专栏 - CSDN博客
2013年12月03日 22:12:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：759
个人分类：[面试](https://blog.csdn.net/z69183787/article/category/2185959)
[参考地址](http://bbs.csdn.net/topics/390648041?page=1#post-396137811)
B继承A，你创建了B的一个对象b，b实际上包括两部分p1,p2，p1是从A继承过来的，p2是自己的，this.i访问的是自己的i，因为p2中没有i，所以实际上访问的就是p1.i，super.i直接指定访问p1.i，所以this.i和super.i是同一个东西。
子类继承父类的属性，因此子类中拥有父类的属性i，
这个属性i用this指针、super指针都可以访问。
继承，也就继承了父类的方法(私有除外)和变量。在你的代码里父类和子类是共用i这个变量的,如果你想用子类的i->this.i。
