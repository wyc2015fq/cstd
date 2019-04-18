# Spring框架下类的初始化顺序 - z69183787的专栏 - CSDN博客
2014年05月09日 09:29:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12390
**序言**
　　之前的已经分析过在不使用框架的情况下，类中各个部分的初始化或执行顺序，后来我在开发中使用了Spring，发现初始化顺序与之前的稍有不同，特别是其初始化以xml配置文档作为驱动，xml中先定义生么类就试图优先实例化这个类，搞得我有点纠结。现在来细细测试研究一下。
　　这次采用的测试代码与之前的类似：有三个主线类B、C和D，其中D继承C，C继承B，这三个类中均包含static块、普通初始化块和无参的构造方法；有两个辅助类E和F，B中包含E类和F类的成员变量，F类成员变量是static类型，E类的成员变量是普通类型；程序运行入口在A.java中。为了符合Spring的开发思路，增加了两个接口IE和IF，E和F分别实现这两个接口。
**正文**
**IE.java**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1 package chloe.spring;
2 
3 public interface IE 
4 {
5     public void funcOfE();
6 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**IF.java**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1 package chloe.spring;
2 
3 public interface IF 
4 {
5     public void funcOfF();
6 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**E.java**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.spring;
 2 
 3 public class E implements IE
 4 {
 5     E()
 6     {
 7         System.out.println("执行E的构造函数");
 8     }
 9     public void funcOfE()
10     {
11         System.out.println("执行E的函数");
12     }
13 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**F.java**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.spring;
 2 
 3 public class F implements IF
 4 {
 5     F()
 6     {
 7         System.out.println("执行F的构造函数");
 8     }
 9     public void funcOfF()
10     {
11         System.out.println("执行F的函数");
12     }
13 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**B.java**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.spring;
 2 
 3 public class B 
 4 {
 5     protected IE e;//实现的类和实例由Spring容器注入
 6     protected static IF f;//f的实现的类和实例由Spring容器注入
 7     protected String sb;//初始值由Spring容器依据配置文件给出
 8     static
 9     {
10         System.out.println("执行B类的static块(B包含E类的成员变量,包含静态F类成员变量)");
11         //f.funcOfF();属性f的注入在构造函数之后，更在此static初始化之后，所以这里不能调用f的函数，调用的话会报初始化异常
12     }
13     {
14         System.out.println("执行B实例的普通初始化块");
15     }
16     B()
17     {
18         System.out.println("执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)");
19         //e.funcOfE();属性e的注入在此构造函数之后，所以这里不能调用f的函数，调用的话会报初始化异常
20     }
21     
22     public String getSb()
23     {
24         return sb;
25     }
26     public void setSb(String sb1)
27     {
28         this.sb=sb1;
29         System.out.println("已给B的成员变量sb赋初值："+sb1);
30     }
31     
32     public void setE(IE e1)
33     {    
34         this.e = e1;    
35         System.out.println("已将E类实例注入B的引用成员变量e");
36     }
37     
38     public void setF(IF f1)//此方法不能加static修饰符，否则Spring注入时报NotWritablePropertyException
39     {    
40         f = f1;    
41         System.out.println("已将F类实例注入B的static引用成员变量f");
42     }
43 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**C.java**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.spring;
 2 
 3 public class C extends B
 4 {
 5     static
 6     {
 7         System.out.println("执行C的static块(C继承B)");
 8     }
 9     {
10         System.out.println("执行C的普通初始化块");
11     }
12     C()
13     {
14         System.out.println("执行C的构造函数(C继承B)");
15     }
16 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**D.java**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.spring;
 2 
 3 public class D extends C
 4 {
 5     
 6     protected static String sd;//由Spring容器依据配置文件赋初始值
 7     
 8     static
 9     {
10         System.out.println("执行D的static块(D继承C)");
11         
12     }
13     {
14         System.out.println("执行D实例的普通初始化块");
15     }
16     protected String sd1;//由Spring容器依据配置文件赋初始值
17     D()
18     {
19         System.out.println("执行D的构造函数(D继承C);父类B的实例成员变量sb的值为："+sb+";本类D的static成员变量sd的值为："+sd+";本类D的实例成员变量sd1的值是："+sd1);
20     }
21     
22     public void methodOfD()
23     {
24         System.out.println("运行D中的方法methodOfD");
25     }
26     
27     public void setSd(String sdtmp)
28     {
29         sd=sdtmp;
30         System.out.println("已初始化D的static成员变量sd");
31     }
32     
33     public void setSd1(String sd1tmp)
34     {
35         sd1=sd1tmp;
36         System.out.println("已初始化D的实例成员变量sd1");
37         
38     }
39 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**A.java**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.spring;
 2 
 3 import org.springframework.context.ApplicationContext;
 4 import org.springframework.context.support.ClassPathXmlApplicationContext;
 5 
 6 public class A 
 7 {
 8     public static void main(String[] args)
 9     {
10         System.out.println("====运行A中的main函数，准备载入xml配置文件====");
11         ApplicationContext appContext=new ClassPathXmlApplicationContext("applicationContext1.xml");
12         System.out.println("====xml配置文件载入完毕，准备获得bean D====");
13         D d=(D)appContext.getBean("beand");
14         System.out.println("====已经获取bean D，准备运行D中的方法methodOfD====");
15         d.methodOfD();
16         
17     }
18 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　配置文件applicationContext1.xml内容：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 <?xml version="1.0" encoding="UTF-8"?><!--Spring框架需要使用的bean定义文件-->
 2 <beans
 3     xmlns="http://www.springframework.org/schema/beans"
 4     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 5     xmlns:p="http://www.springframework.org/schema/p"
 6     xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd">
 7 
 8     <bean id="beane" class="chloe.spring.E"></bean>
 9 
10     <bean id="beanf" class="chloe.spring.F"></bean>
11 
12     <bean id ="beanb" class="chloe.spring.B">
13         <property name="e" ref="beane"></property>  <!--设置bean的成员变量的实现类-->
14         <property name="f" ref="beanf"></property>
15         <property name="sb" value="初始sb"></property>
16     </bean>
17 
18     <bean id ="beanc" class="chloe.spring.C"></bean>
19 
20     <bean id="beand" class="chloe.spring.D">    
21         <property name="sd1" value="初始sd1"></property>
22         <property name="sd" value="初始sd"></property>    
23     </bean>
24 
25 </beans>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　运行A.java后输出如下结果：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 ====运行A中的main函数，准备载入xml配置文件====
 2 2012-10-28 19:06:40 org.springframework.context.support.AbstractApplicationContext prepareRefresh
 3 信息: Refreshing org.springframework.context.support.ClassPathXmlApplicationContext@a01335: startup date [Sun Oct 28 19:06:40 CST 2012]; root of context hierarchy
 4 2012-10-28 19:06:40 org.springframework.beans.factory.xml.XmlBeanDefinitionReader loadBeanDefinitions
 5 信息: Loading XML bean definitions from class path resource [applicationContext1.xml]
 6 2012-10-28 19:06:40 org.springframework.beans.factory.support.DefaultListableBeanFactory preInstantiateSingletons
 7 信息: Pre-instantiating singletons in org.springframework.beans.factory.support.DefaultListableBeanFactory@1fc2fb: defining beans [beane,beanf,beanb,beanc,beand]; root of factory hierarchy
 8 执行E的构造函数
 9 执行F的构造函数
10 执行B类的static块(B包含E类的成员变量,包含静态F类成员变量)
11 执行B实例的普通初始化块
12 执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
13 已将E类实例注入B的引用成员变量e
14 已将F类实例注入B的static引用成员变量f
15 已给B的成员变量sb赋初值：初始sb
16 执行C的static块(C继承B)
17 执行B实例的普通初始化块
18 执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
19 执行C的普通初始化块
20 执行C的构造函数(C继承B)
21 执行D的static块(D继承C)
22 执行B实例的普通初始化块
23 执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
24 执行C的普通初始化块
25 执行C的构造函数(C继承B)
26 执行D实例的普通初始化块
27 执行D的构造函数(D继承C);父类B的实例成员变量sb的值为：null;本类D的static成员变量sd的值为：null;本类D的实例成员变量sd1的值是：null
28 已初始化D的实例成员变量sd1
29 已初始化D的static成员变量sd
30 ====xml配置文件载入完毕，准备获得bean D====
31 ====已经获取bean D，准备运行D中的方法methodOfD====
32 运行D中的方法methodOfD
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　由输出结果可知，在main函数中执行ApplicationContext appContext=new ClassPathXmlApplicationContext("applicationContext1.xml")时，Spring便开始了对类和实例的初始化，初始化顺序与xml配置文件中bean的定义顺序一致，且一个类的构造函数运行时机总是早于其成员变量的初始化时机，不管成员变量是否是static类型，这一点与不用Spring框架的中的顺序不同。此时如果把xml文件中B的定义的放在E的前面，则在执行完B的构造函数后，注入其属性之前实例化E和F，这时的输出结果为：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 ====运行A中的main函数，准备载入xml配置文件====
 2 2012-10-28 20:06:56 org.springframework.context.support.AbstractApplicationContext prepareRefresh
 3 信息: Refreshing org.springframework.context.support.ClassPathXmlApplicationContext@a01335: startup date [Sun Oct 28 20:06:56 CST 2012]; root of context hierarchy
 4 2012-10-28 20:06:56 org.springframework.beans.factory.xml.XmlBeanDefinitionReader loadBeanDefinitions
 5 信息: Loading XML bean definitions from class path resource [applicationContext1.xml]
 6 2012-10-28 20:06:56 org.springframework.beans.factory.support.DefaultListableBeanFactory preInstantiateSingletons
 7 信息: Pre-instantiating singletons in org.springframework.beans.factory.support.DefaultListableBeanFactory@1fc2fb: defining beans [beanb,beane,beanf,beanc,beand]; root of factory hierarchy
 8 执行B类的static块(B包含E类的成员变量,包含静态F类成员变量)
 9 执行B实例的普通初始化块
10 执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
11 执行E的构造函数
12 执行F的构造函数
13 已将E类实例注入B的引用成员变量e
14 已将F类实例注入B的static引用成员变量f
15 已给B的成员变量sb赋初值：初始sb
16 执行C的static块(C继承B)
17 执行B实例的普通初始化块
18 执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
19 执行C的普通初始化块
20 执行C的构造函数(C继承B)
21 执行D的static块(D继承C)
22 执行B实例的普通初始化块
23 执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
24 执行C的普通初始化块
25 执行C的构造函数(C继承B)
26 执行D实例的普通初始化块
27 执行D的构造函数(D继承C);父类B的实例成员变量sb的值为：null;本类D的static成员变量sd的值为：null;本类D的实例成员变量sd1的值是：null
28 已初始化D的实例成员变量sd1
29 已初始化D的static成员变量sd
30 ====xml配置文件载入完毕，准备获得bean D====
31 ====已经获取bean D，准备运行D中的方法methodOfD====
32 运行D中的方法methodOfD
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　我们来继续分析更改xml配置文件前的输出结果，即第一个输出结果。由第10到12行以及第13到15行可知，无继承关系的类B的初始化顺序为：static初始化块 --> 普通初始化块 --> 构造函数 --> 成员变量实例化和初始化 。另外由第16到20行以及第21到27行可知，如果某个类继承了父类，而父类static块之前已执行，则初始化顺序为：子类static初始化块-->父类普通初始化块-->父类构造函数，如果父类还有父类，则先运行父类的父类的初始化块，这是一个递归的过程。
　　如果初始化子类时，父类的还未初始化，即父类的static块还没有执行过，那顺序应该是怎样的呢？我们将xml配置文件中D类的定义移到B和C的定义之前，再运行，得到如下输出：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 ====运行A中的main函数，准备载入xml配置文件====
 2 2012-10-28 20:55:39 org.springframework.context.support.AbstractApplicationContext prepareRefresh
 3 信息: Refreshing org.springframework.context.support.ClassPathXmlApplicationContext@a01335: startup date [Sun Oct 28 20:55:39 CST 2012]; root of context hierarchy
 4 2012-10-28 20:55:40 org.springframework.beans.factory.xml.XmlBeanDefinitionReader loadBeanDefinitions
 5 信息: Loading XML bean definitions from class path resource [applicationContext1.xml]
 6 2012-10-28 20:55:40 org.springframework.beans.factory.support.DefaultListableBeanFactory preInstantiateSingletons
 7 信息: Pre-instantiating singletons in org.springframework.beans.factory.support.DefaultListableBeanFactory@1fc2fb: defining beans [beane,beanf,beand,beanb,beanc]; root of factory hierarchy
 8 执行E的构造函数
 9 执行F的构造函数
10 执行B类的static块(B包含E类的成员变量,包含静态F类成员变量)
11 执行C的static块(C继承B)
12 执行D的static块(D继承C)
13 执行B实例的普通初始化块
14 执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
15 执行C的普通初始化块
16 执行C的构造函数(C继承B)
17 执行D实例的普通初始化块
18 执行D的构造函数(D继承C);父类B的实例成员变量sb的值为：null;本类D的static成员变量sd的值为：null;本类D的实例成员变量sd1的值是：null
19 已初始化D的实例成员变量sd1
20 已初始化D的static成员变量sd
21 执行B实例的普通初始化块
22 执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
23 已将E类实例注入B的引用成员变量e
24 已将F类实例注入B的static引用成员变量f
25 已给B的成员变量sb赋初值：初始sb
26 执行B实例的普通初始化块
27 执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
28 执行C的普通初始化块
29 执行C的构造函数(C继承B)
30 ====xml配置文件载入完毕，准备获得bean D====
31 ====已经获取bean D，准备运行D中的方法methodOfD====
32 运行D中的方法methodOfD
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　可以看出，实例化E和F之后，准备初始化D，而此时D的父类C还未初始化和实例化(被Spring容器载入)，于是要先初始化C，同样，C的父类B也没有初始化，于是先初始化B，这样就出现了第10到12行的结果。另外static初始化块的执行要早于普通初始化块和构造函数，因此执行完D的static块后才执行其父类的普通初始化块和构造函数。另外还可以看出，如果父类之前已经实例化(执行过普通初始化块了构造函数)，之后又有该父类的子类需要初始化，则该父类的普通初始化块和构造函数又要执行一遍，但该父类的static块不会再执行，我想这是由于父类的构造函数也是子类构造函数的一部分，只是在子类的构造函数中省略了，实际创建子类的实例时，也要运行父类的构造函数才能完整地实例化子类。
　　由输出结果的最后三行可知，运行D d=(D)appContext.getBean("beand")时，并不会再次初始化和实例化D，好像只是将变量d指向了Spring容器中已经存在的D实例。所以我推测，使用Spring时，在配置文件中定义的类都是在运行new ClassPathXmlApplicationContext("applicationContext1.xml")时一起初始化和实例化的，并且完成了实例之间的连接，之后getBean只是引用这些已经存在的实例，这样就比较节约内存。因为传统的new方法每调用一次就会在内存中新建一个实例，这样同一类型的实例会有很多个副本。
**总结**
　　在Spring框架下，类的初始化顺序优先级如下：
 　 1. 按照xml配置文件中类的定义顺序加载类并创建类的实例。
　　2. 假设当前要加载X类，则先运行X的static块。如果此时X的父类Y还没有加载，则先查找配置文件来加载Y，运行Y的static块，这样一直递归下去。当X的所有先驱类的static块运行完毕后，再类似递归地实例化X的先驱类(运行先驱类的普通初始化块和构造函数)，完成所有先驱类的实例化后才运行X的普通初始化块和构造函数。
　　3. 执行完X的构造函数之后，开始根据xml配置文件给X的成员变量赋初值。如果X的引用型成员变量所属的类还未加载，则先查找配置文件来加载(初始化、实例化)该成员变量所属的类，加载完毕后将其注入给X的成员变量，完成成员变量的初始化。
　　在前一篇文章([http://www.cnblogs.com/zhouqing/archive/2012/10/26/2741916.html](http://www.cnblogs.com/zhouqing/archive/2012/10/26/2741916.html))中，如果在定义成员变量的同时对该成员变量实例化，则可以在构造函数中调用成员变量实例的函数，但是使用了Spring后，如果成员变量交给Spring容器实例化，则在构造函数中不能调用该成员变量的实例方法，因为在构造函数完成之前，成员变量还没有实例化，结果就会报空指针异常。在使用Spring时，到底将哪些类交给Spring容器管理，哪些类在代码中管理，不能一概而论，要根据功能需要来考虑。另外如果多次运行new
 ClassPathXmlApplicationContext("applicationContext1.xml")，则会再次重新载入和实例化一遍，这样很木有必要，而且容易导致数据不一致，因此一般将new ClassPathXmlApplicationContext的结果作为某个类的static全局变量，随时被其他的类使用，而包含这个全局变量的类就不要托管给Spring了哟~
