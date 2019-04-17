# Java-唠下《内部类》 - DEVELOPER - CSDN博客





2018年03月14日 11:10:53[学术袁](https://me.csdn.net/u012827205)阅读数：97
所属专栏：[JAVA-语言](https://blog.csdn.net/column/details/20397.html)









> 
在一个类的内部定义的类称之为内部类；内部类允许将一些逻辑相关的类组织在一起，并控制内部类代码的可视性。


![这里写图片描述](https://img-blog.csdn.net/20180314100758181?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 要点事项注意
- **实例内部类自动持有当前实例的引用。详细见 代码块-A**
- **在实例内部类中不能定义静态成员，而只能定义实例成员。详细见 代码块-B**
- **静态内部类的实例不会自动持有外部类的特定实例的引用。详细见 代码块-C**
- **局部内部类定义在一个方法中，可见范围只限当前方法且不能使用访问控制修饰符。详细见 代码块-D**
- **内部类的继承。详细见 代码块-E**

### 代码块详解

**代码块-A**

实例内部类Innter.class 自动持有当前实例Outer.class 的引用，即Innter.class 可访问到Outer.class 实例所能访问到的所有成员；当然，在这里会联想到安卓开发过程中的问题，内部类的编写导致了应用的内存泄漏。 
![这里写图片描述](https://img-blog.csdn.net/20180314102114433?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180314102121967?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
在类JavaTest 的main 方法中创建了 Inner.class 实例，是通过outer.new Inner() 或者 new Outer().new Inner()方式，所以实例Inner 必然会引用当前的实例Outer 。


**代码块-B**

实例内部类中定义静态变量、静态方法和静态的内部类都是不合法的。 
![这里写图片描述](https://img-blog.csdn.net/20180314103320329?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**代码块-C**
![这里写图片描述](https://img-blog.csdn.net/20180314104132240?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018031410412555?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
静态内部类的实例不会自动持有外部类的特定实例的引用，因为在创建内部类的实例时不必创建外部类的实例。并且在内部类中，可直接访问外部类的静态成员，而通过外部类的实例可以直接访问外部类的非静态实例成员。


**代码块-D**

访问控制修饰符**public-private-protected**   ——外加修饰符**static**
![这里写图片描述](https://img-blog.csdn.net/20180314105507244?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180314110744401?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
(1)局部内部类只能在当前方法中使用，当前方法以外的位置不可访问；(2)局部内部类和实例内部类一样不能包含静态成员；(3)局部内部类和实例内部类一样可以访问外部类的所有成员。也可以访问所在的方法中使用final修饰过的变量；


**代码块-E**

![这里写图片描述](https://img-blog.csdn.net/2018031411300457?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![这里写图片描述](https://img-blog.csdn.net/20180314113014494?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
使用一个外部类JavaTest继承另一个外部类的内部类，在创建外部类的内部类InnerCls时必定会创建一个外部类Outer的实例。因此，外部类JavaTest和外部类Outer就产生了关联。而产生这种关联前提是，java编译器要求外部类JavaTest必须通过构造方法传递一个外部类Outer实例的引用。否则，会出现编译错误。








