# 设计模式学习笔记——Decorator装饰模式 - 左直拳的马桶_日用桶 - CSDN博客
2017年02月04日 19:20:41[左直拳](https://me.csdn.net/leftfist)阅读数：476
装饰模式的作用或动机就是，尽量避免继承，而使用关联。原因是层层继承下来，内容会越来越多，有失控的危险。就扩展性而言，用关联比用继承好。所谓的关联，A使用了B，就叫A关联了B。
![这里写图片描述](https://img-blog.csdn.net/20170204190456197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Component 抽象构件
ConcreteComponent 具体构件
Decorator 抽象装饰类
从图可知，Decorator聚合（强关联）了Component，Component与Decorator有相同的方法operation()，Decorator本身是调用Component来实现自己这个operation（依赖注入）。
在具体使用过程中，我们是面向Decorator 这个接口来编程；如果要扩展，则是扩展ConcreteComponent的operation()。接口没变，则我们的应用程序也不用做什么修改。
