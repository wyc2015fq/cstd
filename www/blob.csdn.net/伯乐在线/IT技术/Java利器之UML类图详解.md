# Java利器之UML类图详解 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [Code4Android](http://www.jobbole.com/members/Code4Android) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
# 前言
> 
UML（Unified Modeling Language）中文统一建模语言，是一种开放的方法，用于说明、可视化、构建和编写一个正在开发的、面向对象的、软件密集系统的制品的开放方法。UML展现了一系列最佳工程实践，这些最佳实践在对大规模，复杂系统进行建模方面，特别是在软件架构层次已经被验证有效。 【维基百科】
最近看一些开源项目的时候，总是看到UML中的类图，一开始自己的做法就是跳过去类图的部分，不过后来又遇到了几次，就决定学习下，不能再选择逃避了，这也是一个即将优秀的程序探索者应该必备的素质。今天把自己学习到的内容记录一下（怕健忘…），相信对类图不清楚的朋友，通过读取这篇文章后也能有一个比较清晰的认识，让你看的懂，画的出。
# 类图作用
类图是软件工程的统一建模语言一种静态结构图，该图描述了系统的类集合，类的属性和类之间的关系。帮助人们简化对系统的理解，它是系统分析和设计阶段的重要产物，也是系统编码和测试的重要模型依据。学习好类图的绘制，是一位合格的软件工程师应有的技能。
# 类的UML图示
在UML类图中，类使用包含类名，属性，方法名及其参数并且用分割线分隔的长方形表示。例如最简单的一个类Person 类有两个属性分别是name和age,并提供对应的get和set方法。java类代码如下


```
/***********************************************************************
 * Module:  Person.java
 * Author:  Code4Android
 * Purpose: Defines the Class Person
 ***********************************************************************/
import java.util.*;
/** @pdOid 4615fa10-38a3-446e-a7d5-59dcb3e786b5 */
public class Person {
   /** @pdOid f314a698-c3c1-4ec7-a8ea-f8b2df107a29 */
   private String name = zhangsan;
   /** @pdOid 271c633b-87e5-4b41-9f70-2ce320635014 */
   private int age = 23;
   /** @pdOid ccf5b7c1-d005-4a5f-b823-9988f2dd6f91 */
   public String getName() {
      // TODO: implement
      return name;
   }
   /** @pdOid 83e01fba-b004-498e-b7ab-778de8be6dfa */
   public int getAge() {
      // TODO: implement
      return age;
   }
   /** @param name
    * @pdOid 9525895a-11bf-44a3-afed-b4a014540a98 */
   public void setName(String name) {
      // TODO: implement
      this.name=name;
   }
   /** @param age
    * @pdOid 16fd66cc-2af1-4fef-ae98-2a37f495a487 */
   public void setAge(int age) {
      // TODO: implement
      this.age=age;
   }
}
```
那么用类图表示如下，它很简单的表示出了类的所有信息。
![这里写图片描述](http://img.blog.csdn.net/20161205102937081)
通过上面的Person的类图，你应该可以看出类图有三部分组成，类名，属性和操作方法
## 类名
顾名思义就是类的名字，对应于Java类中的类名，如果该类是抽象类的话，在类名右下角会有一个(Abstract)表示，假如Person是一个抽象类的话，则UML类图如下
![这里写图片描述](http://img.blog.csdn.net/20161205103047286)
## 属性名
UML中的属性名就是Java中的成员变量，当然一个类可以没有属性也可以有任意多个属性。Java类中成员变量的修饰符，类型和默认值都可以在UML类图中体现出来。通用表示方法如下


```
可见性  名称:类型 [ = 默认值 ]
```
在Java类中可见性分为三类，分别是private,public 和protected,在类图中分别用符号-、+和#表示。通用表示方法中名称就成员变量的名字，类型就是成员变量的类型如String类型，int类型等，当然也可以是自定义类型。而后面的默认值是可选参数，如果我们没有给成员变量设置初始值，UML类图中就不显示，如上面Person类图，变量name，age没有设置初始值，如果此时我们给name和vaule分别设置默认值zhangsan，23。则UML类图如下
![这里写图片描述](http://img.blog.csdn.net/20161205102818719)
## 操作方法


```
可见性  名称(参数列表) [ : 返回类型]
```
类的操作方法通用表示方式如上，方法可见性和属性可见性是一致的，名称就是方法名，参数列表是可选像，可以是对个参数也是没有参数，若多个参数用英文逗号隔开，返回类型是一个可选项，表示方法的返回值类型，依赖于具体的编程语言，可以是基本数据类型，也可以是用户自定义类型，还可以是空类型(void)，如果是构造方法，则无返回类型。
# 类之间的关系
在UML类图中类与类之间存在多种关系，如泛化(Generalization)关系，实现（Realization)关系，依赖(Dependence)关系，关联（Association）关系，聚合(Aggregation)关系， 组合(Composition)关系。
## 泛化(Generalization)关系
泛化关系也就是Java中的继承，类和类，接口和接口都可以是继承关系，父类又称作基类或超类，子类又称作派生类，类继承父类后可以实现父类的所以功能，并能拥有父类没有的功能。在UML中，泛化关系可以用带空心三角形的直线来表示；例如我们创建两个Java类Teachers和Students类如下代码


```
/***********************************************************************
 * Module:  Teachers.java
 * Author:  Code4Android
 * Purpose: Defines the Class Teachers
 ***********************************************************************/
import java.util.*;
/** @pdOid b54e2d34-d17e-4f2d-993d-563b8e007db4 */
public class Teachers extends Person {
   /** @pdOid 55b38630-1e30-449b-9cb7-f8ef9de59412 */
   private int tNumber;
   /** @pdOid f603d47d-a51f-4b0c-b10b-881842374f8a */
   public String teach() {
      // TODO: implement
      return "I am teaching";
   }
}
/***********************************************************************
 * Module:  Students.java
 * Author:  Code4Android
 * Purpose: Defines the Class Students
 ***********************************************************************/
import java.util.*;
/** @pdOid 6b8ac239-000e-46d3-8233-962a612c12bd */
public class Students extends Person {
   /** @pdOid 0c7627fd-fa78-4f60-a859-2b90274323e1 */
   private int sNumber;
   /** @pdOid 1f929347-b84a-4a54-9ca9-144de66c742b */
   public String study() {
      // TODO: implement
      return "I am learning";
   }
}
```
则UML类图如下
![这里写图片描述](http://img.blog.csdn.net/20161205103413880)
## 实现（Realization)关系
实现关系在java中就是一个类和接口之间的关系，接口中一般是没有成员变量，所有操作都是抽象的（abstract修饰），只有声明没有具体的实现，具体实现需在实现该接口的类中。在UML中用与类的表示法类似的方式表示接口，区别可在UML中类图中看出。如我们创建一个交通工具类接口IVehicle,并有一个形式速度方法声明travelSpeed，Java代码如下


```
/***********************************************************************
 * Module:  IVehicle.java
 * Author:  Code4Android
 * Purpose: Defines the Interface IVehicle
 ***********************************************************************/
import java.util.*;
/** @pdOid 7cbe9f91-a7d9-44b5-a743-280cbc464e61 */
public interface IVehicle {
   /** @pdOid 5e87ebbc-5647-4dc6-8c3c-d23857662584 */
   int travelSpeed();
}
```
对于交通工具，有很多种，不同交通工具都有一个速度，我们举两个例子，如HighSpeedRail（高铁，时速288KM）和Bicycle（自行车类时速20KM）,则两个实现类代码为


```
/***********************************************************************
 * Module:  HighSpeedRail.java
 * Author:  Code4Android
 * Purpose: Defines the Class HighSpeedRail
 ***********************************************************************/
import java.util.*;
/** @pdOid 0ae31b48-07b0-4bcd-83ff-a8a7c7d94518 */
public class HighSpeedRail implements IVehicle {
   /** @pdOid a7693fd4-d6d4-4e93-a946-380f69011b13 */
   public int travelSpeed() {
      // TODO: implement
      return 288;
   }
}
/***********************************************************************
 * Module:  Bicycle.java
 * Author:  Code4Android
 * Purpose: Defines the Class Bicycle
 ***********************************************************************/
import java.util.*;
/** @pdOid c212a8c3-88db-48d8-a2a0-d381d2ee7f91 */
public class Bicycle implements IVehicle {
   /** @pdOid 9ac52769-7489-47b0-9079-4f63f84f1fde */
   public int travelSpeed() {
      // TODO: implement
      return 20;
   }
}
```
在UML中实现接口用虚线和带空心的三角形表示。则上面对应的UML类图如下
![这里写图片描述](http://img.blog.csdn.net/20161204124436841)
## 依赖(Dependence)关系
依赖关系是类与类之间最弱的关系，依赖可以简单的理解一个类使用了另一个类，这种使用关系具有临时性特征，但是一个类又会由于另一个类的改变而受到影响，例如在上面举的例子中，假如我们的Students类拥有一个Bicycle，那么要获取移动速度则需要使用Bicycle中获取速度的方法travelSpeed()方法。此时Students代码如下


```
/***********************************************************************
 * Module:  Students.java
 * Author:  Code4Android
 * Purpose: Defines the Class Students
 ***********************************************************************/
import java.util.*;
/** @pdOid 6b8ac239-000e-46d3-8233-962a612c12bd */
public class Students extends Person {
   /** @pdOid 0c7627fd-fa78-4f60-a859-2b90274323e1 */
   private int sNumber;
   /** @pdOid 1f929347-b84a-4a54-9ca9-144de66c742b */
   public String study() {
      return "I am learning";
   }
   /** @param bicycle
    * @pdOid 82bd76ff-f70c-4e25-bea5-8de19db4699c */
   public int moveSpeed(Bicycle bicycle) {
      return bicycle.travelSpeed();
   }
}
```
在UML中一栏用带箭头的虚线表示依赖，对应的UML类图如下
![这里写图片描述](http://img.blog.csdn.net/20161205103645029)
## 关联（Association）关系
关联关系表示一个类和另一类有联系，例如在上面的举例中每个Teachers都有个家庭住址与之对应，而此时Teacher和Address就形成了一对一的关联关系。如下
![这里写图片描述](http://img.blog.csdn.net/20161205105807907)
关联关系是一种包含关系，在UML中用一个带箭头的实线表示，箭头指向被包含类。在上图中你可能会发现在线上有1 ..1,这个是说明包含关系的。在UML类中有如下几种。


```
1..1 表示另一个类的一个对象只与该类的一个对象有关系
0..* 表示另一个类的一个对象与该类的零个或多个对象有关系
1..* 表示另一个类的一个对象与该类的一个或多个对象有关系
0..1 表示另一个类的一个对象没有或只与该类的一个对象有关系
* 任意多个对象关联
```
## 聚合(Aggregation)关系
聚合关系是表示整体与部分的关系，但是部分可以脱离整体而存在。例如一个Teachers对象有一辆汽车Car，此时Car就是Teachers的一部分，但是Car可以脱离Teachers而存在。在UML类中聚合关系用带空心菱形的直线表示。
![这里写图片描述](http://img.blog.csdn.net/20161205115412548)
## 组合(Composition)关系
组合关系也是一种部分和整体的关系，但是部分存活周期受到整体的影响，若整体不存在则部分也将不存在。此时部分需在整体的构造方法中创建。在UML类中，组合关系用带实心菱形的直线表示。
![这里写图片描述](http://img.blog.csdn.net/20161205115204805)
在文章的最后，附上本篇文章所画的完整的UML类图。
![这里写图片描述](http://img.blog.csdn.net/20161205122009647)
到此，本篇文章真的结束了，若文章有不足或者错误的地方，欢迎指正，以防止给其他读者错误引导。最后感谢[LoveLion](http://blog.csdn.net/LoveLion/article/category/738449)的系列文章。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/09/9c1a7f584ce8aa96b3588db29f9d71de.png)![](http://jbcdn2.b0.upaiyun.com/2016/09/2152fe7617310a41cac8225df1c48ad0.jpg)
