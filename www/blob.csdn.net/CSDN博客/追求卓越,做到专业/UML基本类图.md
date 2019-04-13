
# UML基本类图 - 追求卓越,做到专业 - CSDN博客


2012年07月28日 09:21:19[Waldenz](https://me.csdn.net/enter89)阅读数：506


类图表示不同的实体如何彼此相关,显示了系统的静态结构
**类:**
类的UML是表示一个长方形,垂直地分为三个区,顶部区域显示类名,中间区域显示类的属性,底部显示类的操作.
下面是Fight类
![图 1: Flight类的类图](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/bell_fig1.jpg)
下面是一个显示属性默认值的一个类的UML图
![图 2：显示默认为0美元的balance属性值的银行账户类图。](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/bell_fig2.jpg)
**接口**
和继承的区别是连线用的虚线而不是实线,一条带有闭合的单向箭头的虚线意味着实现.而带有闭合的单向箭头的虚线表示继承.
![图 10：Professor类和Student类实现Person接口的类图实例](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/bell_fig10.gif)
例子详解:Professor类和Student类实现Person接口的类图实例.
**继承:**
继承通过指向超类的一条闭合的，单箭头的实线表示 , 实例:
本类中类名BankAccount和Withdrawal方法都是斜体的,表示BankAccount是抽象类,withdrawal是其抽象方法.
![图 4: 继承通过指向超类的一条闭合的，单箭头的实线表示。](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/bell_fig4.jpg)
一个使用树形记号的继承实例
![图 5: 一个使用树形记号的继承实例](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/bell_fig5.jpg)
**关联**
**双向(标准)关联**
关联是两个类间的联接。关联总是被假定是双向的；这意味着，两个类彼此知道它们间的联系，除非你限定一些其它类型的关联。
![图 6：在一个Flight类和Plane类之间的双向关联的实例](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/bell_fig6.jpg)
例子详解:一个双向关联用两个类间的实线表示。在线的任一端，你放置一个角色名和多重值。Flight与一个特定的Plane相关联，而且Flight类知道这个关联。因为角色名以Plane类表示，所以Plane承担关联中的“assignedPlane”角色。紧接于Plane类后面的多重值描述0...1表示，当一个Flight实体存在时，可以有一个或没有Plane与之关联（也就是，Plane可能还没有被分配）。图
 6 也显示Plane知道它与Flight类的关联。在这个关联中，Flight承担“assignedFlights”角色；上图告诉我们，Plane实体可以不与flight关联（例如，它是一架全新的飞机）或与没有上线的flight（例如，一架已经服役5年的飞机）关联。
**单向关联**
在一个单向关联中,两个类是相关的,但是只有一个类知道这种联系的存在,下图显示单项关联的透支财务报告的一个实例.
![图 7: 单向关联一个实例：OverdrawnAccountsReport 类 BankAccount 类，而 BankAccount 类则对关联一无所知。](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/bell_fig7.jpg)
例子详解:OverdrawAccountsResport和BankAccount的一个单向关联,而BankAccount类则对关联一无所知.
**关联类**
在关系建模中，存在一些情况下，你需要包括其他类，因为它包含了关于关联的有价值的信息，对于这种情况，你会使用关联类来绑定你的基本关联。
关联类的表示：主类和关联类之间用一条相交的电线连接。
![图 11：增加关联类 MileageCredit](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/bell_fig11.jpg)
例子详解：上图中，在Flight类和FrequentFlyer类之间的关联，产生了一个称为MieageCredit的关联类。这意味着当Flight类的一个实例关联到FrequentFlyer类的一个实例时，将会产生MieageCredit类的一个实例。
**聚合**
**基本聚合**
在基本聚合关系中，部分类的生命周期独立于整体类的生命周期。
基本聚合关系表示图：从父类画一条到部分类的实线，并在父类的关联末端画一个未填充的菱形◇。
![图 12: 一个聚合关联的例子](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/bell_fig12.jpg)
例子详解：Car为主类,Wheel为部分类,一个Car可以拥有4个Wheel,但Wheel的存在时独立于Car的.
**组合聚合**
组合聚合区别于基本聚合的是,组合聚合的子类实例的生命周期是依赖于父类实例的生命周期.
组合聚合关系表示图：从父类画一条到子类的实线，并在父类的关联末端画一个填充的菱形。
![图 13: 一个组合关系的例子](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/bell_fig13.jpg)
例子详解：一个Company类的实例至少总有一个Department类实例。因为关系式组合关系，当Company实例被移除/销毁时，Department实例也将自动被销毁。
组合聚合的另一个重要功能是部分类只能与父类的实例相关。
**反射关联**
反射关联用于与自身关联。当一个类的实例关联到它本身时，并不意味着类的实例与它本身相关，而是类的一个实例与类的另一个实例相关。
![图 14：一个反射关联关系的实例](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/bell_fig14.gif)
例子详解：说明一个Employee实例可能是另一个Employee实例的经理。然而因为“manages的关系有0...*”的多重性描述，所以一个Employee可能不受任何其他Employee的管理。
详细信息可参考（[http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/](http://www.ibm.com/developerworks/cn/rational/rationaledge/content/feb05/bell/)）


