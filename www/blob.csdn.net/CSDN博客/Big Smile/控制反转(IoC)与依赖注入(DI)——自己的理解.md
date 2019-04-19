# 控制反转(IoC)与依赖注入(DI)——自己的理解 - Big Smile - CSDN博客
2016年05月03日 15:03:13[王啸tr1912](https://me.csdn.net/tr1912)阅读数：2783
        昨天的验收，师姐提到了这个词控制反转与依赖注入  ，在我们的七层抽象工厂的模式中似乎用到了这个知识，于是乎自己动手查查到底是什么东西，发现了意想不到的东西，特此来分享一下。
# 一、概念(百度)
        控制反转（Inversion of Control，英文缩写为IoC）是一个重要的面向对象编程的法则来削减计算机程序的耦合问题，也是轻量级的Spring框架的核心。 控制反转一般分为两种类型，依赖注入（Dependency Injection，简称DI）和依赖查找（Dependency Lookup）。依赖注入应用比较广泛。
        以上是官方的解释，可能有些不太容易理解，下面是一个大神的博客中的理解性的概念，仅供参考，但是我感觉讲的还是很明白的：
**控制反转（IoC/Inverse Of Control）**:   调用者不再创建被调用者的实例，由spring框架实现（容器创建）所以称为控制反转。
**依赖注入(DI/Dependence injection) **:   容器创建好实例后再注入调用者称为依赖注入。
        我自己的理解：控制反转就是被控制的人的状态不是使用的人决定的，而是由一个第三方控制者决定的。而依赖注入则强调的是第三方控制者的决定方式是注入，所以叫做依赖注入，由百度的概念我们还知道另外一种方式是依赖查找。所以可以理解为控制反转的实现依赖的是注入或者是查找。
        所以我们除了理解控制反转之外，还要理解什么是注入。
注入这个词我们都不陌生，是sql服务的一种简单的攻击方法，其特点就是由于程序的原因将sql语句嵌入到了实现的功能里面去了，得到了访问数据库的机会。
        注入(百度)：所谓的SQL（结构化查询语言）注入，简单来说就是利用SQL语句在外部对SQL数据库进行查询，更新等动作。首先，数据库作为一个网站最重要的组件之一（如果这个网站有数据库的话），里面是储存着各种各样的内容，包括管理员的账号密码。
# 二、理解（demo）
        从上述的概念我们得知了几个关键词：控制反转，注入，外部。也就是说从外部对于一个功能的实现进行控制，这个和我们使用的抽象工厂的反射简直神似加类似啊。
        主要的原理是这样的：当某个角色需要另一个角色的协助时，在传统的程序设计过程中，通常由调用者来创建被调用者的实例,。如果创建被调用者实例的工作不再由调用者来完成，而是由外部容器完成，因此称为控制反转; 创建被调用者 实例的工作通常由外部容器来完成，然后注入调用者，因此也称为依赖注入。
下面看一个小例子：
首先上uml图：
![](https://img-blog.csdn.net/20160503115644480?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码实现：
接口Person：
```
namespace IoCAndDI
{
    public interface Person
    {
        void sayHello();
    }
}
```
实现类Chinese：
```
namespace IoCAndDI
{
    public class Chinese : Person 
    {
        public void sayHello() 
        {
            Console.WriteLine("您好");
        }
    }
}
```
实现类American
```
namespace IoCAndDI
{
    public class American:Person
    {
         public void sayHello() 
         {
                 Console.WriteLine("How do you do .");
           }
    }
}
```
使用者类User
```
namespace IoCAndDI
{
    public class User
    {
        Person p;
        public Person getP()
        {
            return p;
        }
        //使用setter注入
        public void setP(Person p)
        {
            this.p = p;
        }
        //调用person子类重写的sayHello方法，这里的p并没有实例化
        public void function()
        {
            p.sayHello();
        }
    }
}
```
外部第三方容器类Container：
```
namespace IoCAndDI
{
    public class Container
    {
        public static User getBean()
        {
            Person p = new American() ;    //实例化一个实现类
            User user = new User();
            //由容器‘注入’实例
            user.setP(p);
            return user;
        }
    }
}
```
主要调用类Program：
```
namespace IoCAndDI
{
    class Program
    {
        static void Main(string[] args)
        {
            User user = Container.getBean();     //采用第三方容器的注入
            user.function();       //使用者调用功能
            Console.ReadLine();
        }
    }
}
```
以上。
        在这个例子中，不难发现关于最后的显示结果都是依赖于Container的里面的设置的，如果Container里面的p实例化的是Chinese的话，最后控制台输出的就是“你好”，但是现在代码里面的是实例化的American，所以最后输出的就是"How do you do."。
由此可见的话，这个里面Container实际上就是一个反射的选择类，在抽象工厂中扮演了配置文件一职位。User则是扮演的工厂中的额反射的位置，用来把接口和选择的实现类对应上，类似于反射的把接口和相应的dal层中的dal类对应上。这样一来就和我们学过的知识联系起来了。
# 三、总结
![](https://img-blog.csdn.net/20160503145809516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        关于Demo中的例子中的类和7层中的调用对应关系如图所示，所以控制反转依赖注入其实不是什么新鲜的东西了，只是用的地方和方法变化了而已，命名的由来和应用来说很是恰当，真的是反转依赖的是注入，这样才实现的第三方的控制，所以和咱们用的配置文件选择是一样的，这样很好的把调用层进行了解耦，达到了便于修改和扩展的目的。
以上是本人的个人观点，如果错误，欢迎指出。
以上例子出处：http://blog.163.com/taodengwen@126/blog/static/87199341201191383429693/[点击打开链接](http://blog.163.com/taodengwen@126/blog/static/87199341201191383429693/)
