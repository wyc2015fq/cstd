# 初探Java设计模式4：JDK中的设计模式 - 程序员江湖 - CSDN博客





2017年05月15日 11:03:57[黄小斜](https://me.csdn.net/a724888)阅读数：909
所属专栏：[深入浅出Java核心技术](https://blog.csdn.net/column/details/21930.html)









JDK中设计模式

本文主要是归纳了JDK中所包含的设计模式，包括作用和其设计类图。

首先来个总结，具体的某个模式可以一个一个慢慢写，希望能对研究JDK和设计模式有所帮助。


一、设计模式是什么

（1）反复出现问题的解决方案

（2）增强软件的灵活性

（3）适应软件不断变化


二、学习JDK中设计模式的好处

（1）借鉴优秀代码的设计，有助于提高代码设计能力

（2）JDK的设计中体现了大多数设计模式，是学习设计模式的较好的方式

（3）可以更加深入的了解JDK


三、类间关系

继承、委托、依赖、聚合、组合
![](http://hi.csdn.net/attachment/201101/3/0_1294058465rSf1.gif)


四、介绍方式

（1）作用：归纳某设计模式的基本要点

（2）JDK中体现：某设计模式在JDK中是怎样体现出来的

（3）类图：某设计模式在JDK中所对应的类图


五、经典设计模式在JDK中的体现
**1.Singleton（单例）**

作用：保证类只有一个实例；提供一个全局访问点

JDK中体现：

（1）Runtime

（2）NumberFormat

类图：
![](http://hi.csdn.net/attachment/201101/3/0_1294058629PD9z.gif)

**2.Factory（静态工厂）**

作用：

（1）代替构造函数创建对象

（2）方法名比构造函数清晰

JDK中体现：

（1）Integer.valueOf

（2）Class.forName

类图：
![](http://hi.csdn.net/attachment/201101/3/0_1294058705f0y7.gif)

**3.Factory Method（工厂方法）**

作用：子类决定哪一个类实例化

JDK中体现：Collection.iterator方法

类图：
![](http://hi.csdn.net/attachment/201101/3/0_129405877799Eh.gif)

**4.Abstract Factory（抽象工厂）**

作用：创建某一种类的对象

JDK中体现：

（1）java.sql包

（2）UIManager（swing外观）

类图：
![](http://hi.csdn.net/attachment/201101/3/0_1294058833piFB.gif)

**5.Builder（构造者）**

作用：

（1）将构造逻辑提到单独的类中

（2）分离类的构造逻辑和表现

JDK中体现：DocumentBuilder(org.w3c.dom)

类图：
![](http://hi.csdn.net/attachment/201101/3/0_1294058900z0nk.gif)

**6.Prototype（原型）**

作用：

（1）复制对象

（2）浅复制、深复制

JDK中体现：Object.clone；Cloneable

类图：

![](http://hi.csdn.net/attachment/201101/3/0_1294058951u556.gif)

**7.Adapter（适配器）**

作用：使不兼容的接口相容

JDK中体现：

（1）java.io.InputStreamReader(InputStream)

（2）java.io.OutputStreamWriter(OutputStream)

类图：
![](http://hi.csdn.net/attachment/201101/3/0_1294058999cvJS.gif)

**8.Bridge（桥接）**

作用：将抽象部分与其实现部分分离，使它们都可以独立地变化

JDK中体现：java.util.logging中的Handler和Formatter

类图： 
![](http://hi.csdn.net/attachment/201101/3/0_1294059065CHyJ.gif)



**9.Composite（组合）**

作用：一致地对待组合对象和独立对象

JDK中体现：

（1）org.w3c.dom

（2）javax.swing.JComponent#add(Component)

类图：
![](http://hi.csdn.net/attachment/201101/3/0_12940591438tt5.gif)

**10.Decorator（装饰器）**

作用：为类添加新的功能；防止类继承带来的爆炸式增长

JDK中体现：

（1）java.io包

（2）java.util.Collections#synchronizedList(List)

类图：
![](http://hi.csdn.net/attachment/201101/3/0_1294059189Z6z9.gif)



**11.Façade（外观）**

作用：

（1）封装一组交互类，一致地对外提供接口

（2）封装子系统，简化子系统调用

JDK中体现：java.util.logging包

类图：
![](http://hi.csdn.net/attachment/201101/3/0_1294059242iBI6.gif)



**12.Flyweight（享元）**

作用：共享对象，节省内存

JDK中体现：

（1）Integer.valueOf(int i)；Character.valueOf(char c)

（2）String常量池

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_1294059313R7gf.gif)**



**14.Proxy（代理）**

作用：

（1）透明调用被代理对象，无须知道复杂实现细节

（2）增加被代理类的功能

JDK中体现：动态代理；RMI

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_1294059364KAKJ.gif)**


**15.Iterator（迭代器）**

作用：将集合的迭代和集合本身分离

JDK中体现：Iterator、Enumeration接口

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_1294059440PtTX.gif)16.Observer（观察者）**

作用：通知对象状态改变

JDK中体现：

（1）java.util.Observer,Observable

（2）Swing中的Listener

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_12940594862KCk.gif)17.Mediator（协调者）**

作用：用于协调多个类的操作

JDK中体现：Swing的ButtonGroup

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_1294059535cunk.gif)18.Template method（模板方法）**

作用：定义算法的结构，子类只实现不同的部分

JDK中体现：ThreadPoolExecutor.Worker

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_129405958555W9.gif)19.Strategy（策略）**

作用：提供不同的算法

JDK中的体现：ThreadPoolExecutor中的四种拒绝策略

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_1294059635WZ13.gif)20.Chain of Responsibility（责任链）**

作用：请求会被链上的对象处理，但是客户端不知道请求会被哪些对象处理

JDK中体现：

（1）java.util.logging.Logger会将log委托给parent logger

（2）ClassLoader的委托模型

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_12940596873g03.gif)21.Command（命令）**

作用：

（1）封装操作，使接口一致

（2）将调用者和接收者在空间和时间上解耦合

JDK中体现：Runnable；Callable；ThreadPoolExecutor

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_1294059739iKYQ.gif)22.Null Object（空对象）**

作用：不需每次判空，对待空值，如同对待一个相同接口的对象

JDK中体现：Collections.EMPTY_LIST

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_1294059830VRDq.gif)23.State（状态）**

作用：将主对象和其状态分离，状态对象负责主对象的状态转换，使主对象类功能减轻

JDK中体现：未发现

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_1294059912fL5S.gif)24.Visitor（访问者）**

作用：异构的类间添加聚合操作；搜集聚合数据

JDK中的体现：未发现

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_1294059969wolD.gif)25.Interpreter（解释器）**

作用：用一组类代表某一规则

JDK中体现：java.util.regex.Pattern

类图：四则运算
**![](http://hi.csdn.net/attachment/201101/3/0_1294060030XvOm.gif)26.Memento（备忘录）**

作用：保持对象状态，需要时可恢复

JDK中体现：未发现

类图：
**![](http://hi.csdn.net/attachment/201101/3/0_1294060089NKr4.gif)**


六、参考文献
**1. Design Pattern（GoF）2. Software Architecture Design Patterns in Java3. JDK 5 Documentation4. http://stackoverflow.com/questions/1673841/examples-of-gof-design-patterns5. http://java.csdn.net/a/20101129/282644.html**





**更多内容请关注微信公众号【Java技术江湖】**

**这是一位阿里 Java 工程师的技术小站，作者黄小斜，专注 Java 相关技术：SSM、SpringBoot、MySQL、分布式、中间件、集群、Linux、网络、多线程，偶尔讲点Docker、ELK，同时也分享技术干货和学习经验，致力于Java全栈开发！（关注公众号后回复”资料“即可领取 3T 免费技术学习资源以及我我原创的程序员校招指南、Java学习指南等资源）**

**![](https://img-blog.csdnimg.cn/20190406150016453.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=,size_16,color_FFFFFF,t_70)**



