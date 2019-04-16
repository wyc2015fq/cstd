# 面试题收集——Java基础部分(一) - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [面试题收集——Java基础部分(一)](https://www.cnblogs.com/xdp-gacl/p/3641769.html)



## 1、一个".java"源文件中是否可以包括多个类（不是内部类）？有什么限制？ 

**可以有多个类，但只能有一个public的类，并且public的类名必须与文件名相一致。**

## 2、Java有没有goto? 

**java中的保留字，现在没有在java中使用。**

## 3、说说&和&&的区别。 

**&和&&都可以用作逻辑与的运算符，表示逻辑与（and），当运算符两边的表达式的结果都为true时，整个运算结果才为true，否则，只要有一方为false，则结果为false。**

**&&还具有短路的功能，即如果第一个表达式为false，则不再计算第二个表达式**，例如，对于if(str != null && !str.equals(“”))表达式，当str为null时，后面的表达式不会执行，所以不会出现NullPointerException如果将&&改为&，则会抛出NullPointerException异常。If(x==33 & ++y>0) y会增长，If(x==33 && ++y>0)不会增长

**&还可以用作位运算符，当&操作符两边的表达式不是boolean类型时，&表示按位与操作**，我们通常使用0x0f来与一个整数进行&运算，来获取该整数的最低4个bit位，例如，0x31 & 0x0f的结果为0x01。 

## 4、switch语句能否作用在byte上，能否作用在long上，能否作用在String上? 

**在switch（expr1）中，expr1只能是一个整数表达式或者枚举常量，整数表达式可以是int基本类型或Integer包装类型**，由于，byte,short,char都可以隐含转换为int，所以，这些类型以及这些类型的包装类型也是可以的。显然，long和String类型都不符合switch的语法规定，并且不能被隐式转换成int类型，所以，它们不能作用于swtich语句中。 

## 5、short s1 = 1; s1 = s1 + 1;有什么错? short s1 = 1; s1 += 1;有什么错? 

　　对于short s1 = 1; s1 = s1 + 1; 由于s1+1运算时会自动提升表达式的类型，所以结果是int型，再赋值给short类型s1时，编译器将报告需要强制转换类型的错误。

**对于short s1 = 1; s1 += 1;由于 += 是java语言规定的运算符，java编译器会对它进行特殊处理，因此可以正确编译。 **

## 6、char型变量中能不能存贮一个中文汉字?为什么? 

**char型变量是用来存储Unicode编码的字符的，unicode编码字符集中包含了汉字，所以，char型变量中当然可以存储汉字啦。不过，如果某个特殊的汉字没有被包含在unicode编码字符集中，那么，这个char型变量中就不能存储这个特殊汉字**。补充说明：**unicode编码占用两个字节，所以，char类型的变量也是占用两个字节。**

## 7、使用final关键字修饰一个变量时，是引用不能变，还是引用的对象不能变？ 

**使用final关键字修饰一个变量时，是指引用变量不能变，引用变量所指向的对象中的内容还是可以改变的**。

　　例如，对于如下语句：

 　　　　final StringBuffer a=new StringBuffer("immutable");
　　执行如下语句将报告编译期错误：

　　　　a=new StringBuffer("");
　　但是，执行如下语句则可以通过编译：

　　　　a.append(" broken!"); 

　　有人在定义方法的参数时，可能想采用如下形式来阻止方法内部修改传进来的参数对象：

	　　　　public void method(final  StringBuffer  param){

	　　　　} 

　　实际上，这是办不到的，在该方法内部仍然可以增加如下代码来修改参数对象：

		　　　　param.append("a");

## 8、"=="和equals方法究竟有什么区别？

**“==”操作符专门用来比较两个变量的值是否相等，也就是用于比较变量所对应的内存中所存储的数值是否相同，要比较两个基本类型的数据或两个引用变量是否相等，只能用==操作符。**

**如果一个变量指向的数据是对象类型的，那么，这时候涉及了两块内存，对象本身占用一块内存（堆内存），变量也占用一块内存**(栈内存)，例如Objet obj = new Object();变量obj是一个内存，new Object()是另一个内存，此时，变量obj所对应的内存中存储的数值就是对象占用的那块内存的首地址。**对于指向对象类型的变量，如果要比较两个变量是否指向同一个对象，即要看这两个变量所对应的内存中的数值是否相等，这时候就需要用==操作符进行比较**。

**equals方法是用于比较两个独立对象的内容是否相同，就好比去比较两个人的长相是否相同，它比较的两个对象是独立的**。例如，对于下面的代码：

**String a=new String("foo");**

**　　String b=new String("foo");**

**　　两条new语句创建了两个对象，然后用a,b这两个变量分别指向了其中一个对象，这是两个不同的对象，它们的首地址是不同的，即a和b中存储的数值是不相同的，所以，表达式a==b将返回false，而这两个对象中的内容是相同的，所以，表达式a.equals(b)将返回true。**

　　在实际开发中，我们经常要比较传递进行来的字符串内容是否等，例如，String input = …;input.equals(“quit”)，许多人稍不注意就使用==进行比较了，这是错误的，记住，字符串的比较基本上都是使用equals方法。

　　如果一个类没有自己定义equals方法，那么它将继承Object类的equals方法，Object类的equals方法的实现代码如下：

**boolean equals(Object o){**

**　　　　return this==o;**

**　　}**

　　这说明，**如果一个类没有自己定义equals方法，它默认的equals方法（从Object 类继承的）就是使用==操作符，也是在比较两个变量指向的对象是否是同一对象，这时候使用equals和使用==会得到同样的结果，如果比较的是两个独立的对象则总返回false。****如果你编写的类希望能够比较该类创建的两个实例对象的内容是否相同，那么你必须覆盖equals方法，由你自己写代码来决定在什么情况即可认为两个对象的内容是相同的。**

## 9、静态变量和实例变量的区别？

**在语法定义上的区别：****静态变量前要加static****关键字，而实例变量前则不加**。

**在程序运行时的区别：实例变量属于某个对象的属性，必须创建了实例对象，其中的实例变量才会被分配空间，才能使用这个实例变量**。**静态变量不属于某个实例对象，而是属于类，所以也称为类变量，只要程序加载了类的字节码，不用创建任何实例对象，静态变量就会被分配空间，静态变量就可以被使用了**。总之，**实例变量必须创建对象后才可以通过这个对象来使用，静态变量则可以直接使用类名来引用。**

　　例如，对于下面的程序，无论创建多少个实例对象，永远都只分配了一个staticVar变量，并且每创建一个实例对象，这个staticVar就会加1；但是，每创建一个实例对象，就会分配一个instanceVar，即可能分配多个instanceVar，并且每个instanceVar的值都只自加了1次。

public class VariantTest{

　　public static int staticVar = 0; 

　　public int instanceVar = 0; 

　　public VariantTest(){

　　　　staticVar++;

　　　　instanceVar++;

　　　　System.out.println(“staticVar=” + staticVar + ”,instanceVar=” + instanceVar);

　　}

}

## 10、是否可以从一个static方法内部发出对非static方法的调用？

　　不可以。因为**非static方法是要与对象关联在一起的，必须创建一个对象后，才可以在该对象上进行方法调用，而static方法调用时不需要创建对象，可以直接调用**。也就是说，当一个static方法被调用时，可能还没有创建任何实例对象，如果从一个static方法中发出对非static方法的调用，那个非static方法是关联到哪个对象上的呢？这个逻辑无法成立，所以，**一个static方法内部不可以发出对非static****方法的调用**。

## 11、Integer与int的区别

**int是java提供的8种原始数据类型之一。Java为每个原始类型提供了封装类，Integer是java为int提供的封装类。****int的默认值为0，而Integer的默认值为null，即Integer可以区分出未赋值和值为0的区别，int则无法表达出未赋值的情况**，例如，要想表达出没有参加考试和考试成绩为0的区别，则只能使用Integer。**在JSP开发中，Integer的默认为null，所以用el表达式在文本框中显示时，值为空白字符串，而int默认的默认值为0，所以用el表达式在文本框中显示时，结果为0，所以，int不适合作为web层的表单数据的类型。**

　　在Hibernate中，如果将OID定义为Integer类型，那么Hibernate就可以根据其值是否为null而判断一个对象是否是临时的，如果将OID定义为了int类型，还需要在hbm映射文件中设置其unsaved-value属性为0。

　　另外，Integer提供了多个与整数相关的操作方法，例如，将一个字符串转换成整数，Integer中还定义了表示整数的最大值和最小值的常量。

## 12、请说出作用域public，private，protected，以及不写时的区别

这四个作用域的可见范围如下表所示。

说明：**如果在修饰的元素上面没有写任何访问修饰符，则表示friendly。**

![](data:image/png;base64,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)

## 13、Overload和Override的区别。Overloaded的方法是否可以改变返回值的类型? 

**Overload是重载**的意思，**Override是覆盖**的意思，**也就是重写**。

**重载Overload表示同一个类中可以有多个名称相同的方法，但这些方法的参数列表各不相同（即参数个数或类型不同）。**

**重写Override表示子类中的方法可以与父类中的某个方法的名称和参数完全相同，通过子类创建的实例对象调用这个方法时，将调用子类中的定义方法，这相当于把父类中定义的那个完全相同的方法给覆盖了，这也是面向对象编程的多态性的一种表现。****子类覆盖父类的方法时，只能比父类抛出更少的异常，或者是抛出父类抛出的异常的子异常，因为子类可以解决父类的一些问题，不能比父类有更多的问题。子类方法的访问权限只能比父类的更大，不能更小。****如果父类的方法是private类型，那么，子类则不存在覆盖的限制，相当于子类中增加了一个全新的方法。**

　　至于Overloaded的方法是否可以改变返回值的类型这个问题，要看你倒底想问什么呢？这个题目很模糊。如果几个Overloaded的方法的参数列表不一样，它们的返回者类型当然也可以不一样。但我估计你想问的问题是：**如果两个方法的参数列表完全一样，是否可以让它们的返回值不同来实现重载**Overload？这是不行的，我们可以用反证法来说明这个问题，**因为我们有时候调用一个方法时也可以不定义返回结果变量，即不要关心其返回结果**，例如，我们调用map.remove(key)方法时，虽然remove方法有返回值，但是我们通常都不会定义接收返回结果的变量，这时候**假设该类中有两个名称和参数列表完全相同的方法，仅仅是返回类型不同，java就无法确定编程者倒底是想调用哪个方法了，因为它无法通过返回结果类型来判断。 **

**override可以翻译为覆盖，从字面就可以知道，它是覆盖了一个方法并且对其重写，以求达到不同的作用。**对我们来说**最熟悉的覆盖就是对接口方法的实现，在接口中一般只是对方法进行了声明，而我们在实现时，就需要实现接口声明的所有方法**。除了这个典型的用法以外，我们**在继承中也可能会在子类覆盖父类中的方法，在覆盖要注意以下的几点：**

**1、覆盖的方法的标志必须要和被覆盖的方法的标志完全匹配，才能达到覆盖的效果；**

**　　2、覆盖的方法的返回值必须和被覆盖的方法的返回一致；**

**　　3、覆盖的方法所抛出的异常必须和被覆盖方法的所抛出的异常一致，或者是其子类；**

**　　4、被覆盖的方法不能为private，否则在其子类中只是新定义了一个方法，并没有对其进行覆盖。**

**overload**对我们来说可能比较熟悉，**可以翻译为重载**，它是**指我们可以定义一些名称相同的方法，通过定义不同的输入参数来区分这些方法，然后再调用时，VM就会根据不同的参数样式，来选择合适的方法执行。****在使用重载要注意以下的几点：**
- **在使用重载时只能通过不同的参数样式。**例如，不同的参数类型，不同的参数个数，不同的参数顺序（当然，同一方法内的几个参数类型必须不一样，例如可以是fun(int,float)，但是不能为fun(int,int)）；
- **不能通过访问权限、返回类型、抛出的异常进行重载；**
- **方法的异常类型和数目不会对重载造成影响；**
- **对于继承来说，如果某一方法在父类中是访问权限是priavte，那么就不能在子类对其进行重载，如果定义的话，也只是定义了一个新方法，而不会达到重载的效果。**

## 14、has a与is a的区别

**is-a表示的是属于的关系**。比如兔子属于一种动物（继承关系）。

**has-a表示组合，包含关系**。比如兔子包含有腿，头等组件。

## 15、ClassLoader如何加载class？

**jvm里有多个类加载器，每个类加载器可以负责加载特定位置的类**，例如，**bootstrap类加载负责加载jre/lib/rt.jar中的类， 我们平时用的jdk中的类都位于rt.jar中。extclassloader负责加载jar/lib/ext/*.jar中的类，appclassloader负责classpath指定的目录或jar中的类**。**除了bootstrap之外，其他的类加载器本身也都是java类，它们的父类是ClassLoader。**

## 16、分层设计的好处

**把各个功能按调用流程进行了模块化，模块化带来的好处就是可以随意组合**，举例说明：如果要注册一个用户，流程为显示界面并通过界面接收用户的输入，接着进行业务逻辑处理，在处理业务逻辑又访问数据库，如果我们将这些步骤全部按流水帐的方式放在一个方法中编写，这也是可以的，但这其中的坏处就是，当界面要修改时，由于代码全在一个方法内，可能会碰坏业务逻辑和数据库访问的码，同样，当修改业务逻辑或数据库访问的代码时，也会碰坏其他部分的代码。**分层就是要把界面部分、业务逻辑部分、数据库访问部分的代码放在各自独立的方法或类中编写，这样就不会出现牵一发而动全身的问题了。**这样分层后，还可以方便切换各层，譬如原来的界面是Swing，现在要改成BS界面，如果最初是按分层设计的，这时候不需要涉及业务和数据访问的代码，只需编写一条web界面就可以了。

**分层的好处：**

**1.实现了软件之间的解耦；**

**　　2.便于进行分工**

**　　3.便于维护**

**　　4.提高软件组件的重用**

**　　5.便于替换某种产品**，比如持久层用的是hibernate,需要更换产品用toplink，就不用该其他业务代码，直接把配置一改。

**　　6.便于产品功能的扩展。**

**　　7.便于适用用户需求的不断变化**

## 17、hashCode方法的作用？

**hashcode这个方法是用来鉴定2个对象是否相等的。**

**equals方法和hashCode方法这2个方法都是用来判断2个对象是否相等的，但是他们是有区别的。**

**一般来讲，equals这个方法是给用户调用的，如果你想判断2个对象是否相等，你可以重写equals方法，然后在代码中调用，就可以判断他们是否相等了。简单来讲，equals方法主要是用来判断从表面上看或者从内容上看，2个对象是不是相等。**举个例子，有个学生类，属性只有姓名和性别，那么我们可以认为只要姓名和性别相等，那么就说这2个对象是相等的。

**hashcode方法一般用户不会去调用**，比如在hashmap中，由于key是不可以重复的，他在判断key是不是重复的时候就判断了hashcode这个方法，而且也用到了equals方法。这里不可以重复是说equals和hashcode只要有一个不等就可以了！所以简单来讲，**hashcode****相当于是一个对象的编码，就好像文件中的****md5，他和equals不同就在于他返回的是int型的，比较起来不直观。****我们一般在覆盖equals的同时也要覆盖hashcode，让他们的逻辑一致**。举个例子，还是刚刚的例子，如果姓名和性别相等就算2个对象相等的话，那么hashcode的方法也要返回姓名的hashcode值加上性别的hashcode值，这样从逻辑上，他们就一致了。

**要从物理上判断2个对象是否相等，用==就可以了，如果两个对象的物理(内存)地址相等，那么这两个对象肯定就是同一个对象。**

## 18、什么是AOP？

### **1.AOP概念介绍**

**所谓AOP，即Aspect orientied program,就是面向方面(切面)的编程。**

**　　面向切面编程Aspect-Orlented-Programming，即AOP是对面向对象的思维方式的有力补充。**

**　　AOP的好处是可以动态地添加和删除在切面上的逻辑而不影响原来的执行代码**

### **2.解释什么是方面(切面)**

**所谓方面(切面)**，**指的是贯穿到系统的各个模块中的系统一个功能就是一个方面****(切面****)**，比如，记录日志，统一异常处理，事务处理，权限检查，这些功能都是软件系统的一个面，而不是一点，在各个模块中都要出现。

### **3.什么是面向方面编程**

　　把系统的一个方面的功能封装成对象的形式来处理就是面向方面(切面)编程

### **4.怎么进行面向方面编程**

**把功能模块对应的对象作为切面嵌入到原来的各个系统模块中，采用代理技术，代理会调用目标，同时把切面功能的代码（对象）加入进来。**所以，用spring配置代理对象时只要要配两个属性，分别表示目标和切面对象（Advisor）。

##  19.谈谈你对mvc的理解

　　MVC是Model—View—Controler的简称。即模型—视图—控制器。MVC是一种设计模式，它强制性的把应用程序的输入、处理和输出分开。

　　MVC中的模型、视图、控制器它们分别担负着不同的任务。
- 
视图: 视图是用户看到并与之交互的界面。视图向用户显示相关的数据，并接受用户的输入。视图不进行任何业务逻辑处理。
- 
模型: 模型表示业务数据和业务处理。相当于JavaBean。一个模型能为多个视图提供数据。这提高了应用程序的重用性
- 
控制器: 当用户单击Web页面中的提交按钮时,控制器接受请求并调用相应的模型去处理请求。然后根据处理的结果调用相应的视图来显示处理的结果。


　　MVC的处理过程：首先控制器接受用户的请求，调用相应的模型来进行业务处理，并返回数据给控制器。控制器调用相应的视图来显示处理的结果。并通过视图呈现给用户。

 　　IT公司面试手册：http://www.mianwww.com/










