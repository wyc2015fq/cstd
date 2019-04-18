# Autowired的使用：推荐对构造函数进行注释及其他两种方式 - z69183787的专栏 - CSDN博客
2018年08月08日 11:40:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：767
[https://www.cnblogs.com/acm-bingzi/p/springAutowired.html](https://www.cnblogs.com/acm-bingzi/p/springAutowired.html)
在编写代码的时候，使用@Autowired注解是，发现IDE报的一个警告，如下：
![](https://images2015.cnblogs.com/blog/380239/201704/380239-20170428140825334-1173551331.png)
　　Spring Team recommends "Always use constructor based dependency injection in your beans. Always use assertions for mandatory dependencies".
　　翻译：
　　　　Spring建议”总是在您的bean中使用构造函数建立依赖注入。总是使用断言强制依赖”。
这段代码警告原来的写法是：
> 
*@Autowired*
*private EnterpriseDbService service;*
建议后写成下面的样子：
> 
*private final EnterpriseDbService service;*
*@Autowired*
*public EnterpriseDbController(EnterpriseDbService service) {*
*   this.service = service;*
*}*
　　奇怪，为何会有这样的建议。
　　我们知道：@Autowired 可以对成员变量、方法以及构造函数进行注释。那么对成员变量和构造函数进行注释又有什么区别呢？
　　@Autowired注入bean，相当于在配置文件中配置bean，并且使用setter注入。而对构造函数进行注释，就相当于是使用构造函数进行依赖注入了吧。莫非是这两种注入方法的不同。
　　以下是：@Autowired和构造方法执行的**顺序**解析
　　先看一段代码，下面的代码能运行成功吗？
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1 @Autowired
2 private User user;
3 private String school;
4 
5 public UserAccountServiceImpl(){
6     this.school = user.getSchool();
7 }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
　　答案是不能。
　　因为Java类会先执行构造方法，然后再给注解了@Autowired 的user注入值，所以在执行构造方法的时候，就会报错。
　　报错信息可能会像下面：
　　Exception in thread "main" org.springframework.beans.factory.BeanCreationException: Error creating bean with name '...' defined in file [....class]: Instantiation of bean failed; nested exception is org.springframework.beans.BeanInstantiationException: Failed to instantiate [...]: Constructor threw exception; nested exception is java.lang.NullPointerException
　　报错信息说：创建Bean时出错，出错原因是实例化bean失败，因为bean时构造方法出错，在构造方法里抛出了空指针异常。
　　解决办法是，使用构造器注入，如下：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1 private User user;
2 private String school;
3 
4 @Autowired
5 public UserAccountServiceImpl(User user){
6     this.user = user;
7     this.school = user.getSchool();
8 }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
　　可以看出，使用构造器注入的方法，**可以明确成员变量的加载顺序**。
　　PS：Java变量的初始化顺序为：静态变量或静态语句块–>实例变量或初始化语句块–>构造方法–>@Autowired
　　参考：http://blog.csdn.net/ruangong1203/article/details/50992147
　　那么最开始Spring建议，为何要将成员变量加上final类型呢？
　　网上有解释如下：spring配置默认的bean的scope是singleton，也就是启动后一直有。通过设置bean的scope属性为prototype来声明该对象为动态创建。但是，如果你的service本身是singleton，注入只执行一次。
　　@Autowired本身就是单例模式，只会在程序启动时执行一次，即使不定义final也不会初始化第二次，所以这个final是没有意义的吧。
　　可能是为了防止，在程序运行的时候，又执行了一遍构造函数；
　　或者是更容易让人理解的意思，加上final只会在程序启动的时候初始化一次，并且在程序运行的时候不会再改变。
在IDEA升级2017版后，发现以前使用的`@Autowired`出现了个警告`Field injection is not recommended`。
虽然不是异常，但就是看着不舒服，所以google了一下，发现了[stackoverflow](http://stackoverflow.com/questions/39890849/what-exactly-is-field-injection-and-how-to-avoid-it) 已经有人提了这个问题，并得到了解答。
# @Autowired的不推荐用法
在一个Bean内，可以使用`@Autowired`注入另一个Bean。
|123456789|@Componentpublic class Dependency(){}@Componentpublic class DI(){@Autowiredprivate Dependency dependency;}|
事实上，这就是我平常使用的方式，直接在`Field`上添加注解，简洁又好看。
但这是不推荐的使用方法。
# @Autowired的三种使用方式
- 通过构造器注入
- 通过setter方法注入
- 通过field反射注入
|12345678910111213141516171819|public class DI(){//通过构造器注入private DependencyA a;@Autowiredpublic DI(DependencyA a){this.a = a;}//通过setter方法注入private DependencyB b;@Autowiredpublic void setDependencyB(DependencyB b){this.b = b;}//通过field反射注入@Autowiredprivate DependencyC c;}|
# 弊端
**如果你使用的是构造器注入**
恭喜你，当你有十几个甚至更多对象需要注入时，你的构造函数的`参数个数`可能会长到无法想像。
**如果你使用的是field反射注入**
如果不使用Spring框架，这个属性只能通过反射注入，太麻烦了！这根本不符合`JavaBean`规范。
还有，当你不是用过`Spring`创建的对象时，还可能引起`NullPointerException`。
并且，你不能用`final`修饰这个属性。
**如果你使用的是setter方法注入**
那么你将不能将属性设置为`final`。
# 两者取其轻
Spring3.0官方文档建议使用setter注入覆盖构造器注入。
Spring4.0官方文档建议使用构造器注入。
# 结论
如果注入的属性是`必选`的属性，则通过构造器注入。
如果注入的属性是`可选`的属性，则通过setter方法注入。
