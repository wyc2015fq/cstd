# UML中类图的符号解释 - L_Andy的专栏 - CSDN博客

2014年06月09日 17:33:31[卡哥](https://me.csdn.net/L_Andy)阅读数：1081


在UML的定义中，描述类和对象之间的关系，包括以下几种方式：依赖（Dependency）、关联（Association）、聚合（Aggregation）、组合（Composition）、泛化（Generalization）和实现（Realization）。现分别说明如下：

**1.依赖（Dependency）**

在uml中，“依赖”表示为带箭头的虚线，箭头指向被依赖的元素。是类与类之间的连接，表示为一个类依赖于另一个类的定义，其中一个类的变化将影响另一个类。依赖总是单向的，不应该存在双向依赖，这一点要特别注意。更具体的说，依赖可以理解为：一个类（A）对不在其实例作用域内的另一个类或对象（B）的任何类型的引用。大致包含以下几种情况：

（1）局部变量；

（2）方法的参数；

（3）静态方法的调用；

下面是依赖关系的uml示意图：

![](https://img-blog.csdn.net/20140609173102843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**2.关联（Association）**

在uml中，关联表示为带箭头的实线。关联可以是单向的，也可以是双向的。如果是双向关联，则可以表示为双向箭头，或者没有箭头。一般来说，系统设计应表现为单向关联，这样利于维护。一个关联可以附加“多重性”的修饰符，表示两个类之间的数量关系。关联可以理解为：一个类（A）持有另一个类或对象（B）。具体表现为：

（1）成员变量

下面是关联关系的uml示例图：

![](https://img-blog.csdn.net/20140609173119921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上面的关联表示，一个Employee持有(has)0个或多个TimeCard。

**3.聚合（Aggregation）**

在uml中，聚合关系表示为空心的菱形箭头线。聚合关系是关联关系的一种，表示一种“强”关联关系。对比与关联关系，两个类是处于同一个层次的。而聚合关系，两个类处于不同的层次，强调了一个整体/局部的关系。例如一辆汽车有一个引擎，4个轮胎。

在聚合关系中，体现了一种“弱拥有”的概念。也就是说，对象A拥有对象B，但B并不是A的组成部分。更具体的表现为，如果A由B聚合而成，则A包含B的全局对象，但B对象可以不在A对象创建时创建。回到前面的例子，汽车对象由轮胎对象聚合而成，但是轮胎对象的生命期并不受汽车对象的左右。当汽车对象销毁时，轮胎对象也可以单独存在！

下面是聚合关系的uml示意图：

![](https://img-blog.csdn.net/20140609173131515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

从代码上看，聚合和关联没有任何区别。这里仅仅体现一种概念上的含义。在创建ClassA的时候，不一定需要同时创建ClassB的实例。

**4.组合（Composition）**

在uml中，组合关系表示为实心菱形箭头线。组合也叫合成。合成关系强调了比聚合关系更加强的整体/部分的关联，例如人和四肢。和聚合关系所不同的是，在组合关系中，虽然局部不一定随着整体的销毁而销毁，但整体要么负责保持局部的存活状态，要么负责将其销毁。也就是说，组合关系中，局部的存活期一定是小于，最多是等于整体的存活期的。

下面是组合关系的uml示例图：

**![](https://img-blog.csdn.net/20140609173142609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**5.泛化（Generalization）与实现（Realization）**

泛化也就是通常所谓的继承关系，在uml中表示为一个带空心三角的实线。表示为is-a的关系，是对象间耦合度最大的一种关系，子类继承父类的所有细节，并可以在此基础上添加自己的特性。

下面是泛化关系的uml图：

![](https://img-blog.csdn.net/20140609173155593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

所谓实现就是对接口的定义实现，很简单。表现为带箭头的虚线。下面的实现的uml图：

![](https://img-blog.csdn.net/20140609173204250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

