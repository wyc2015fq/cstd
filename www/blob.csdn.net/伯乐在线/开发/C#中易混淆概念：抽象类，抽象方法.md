# C#中易混淆概念：抽象类，抽象方法 - 文章 - 伯乐在线
原文出处： [zhiqiang21](http://blog.csdn.net/yisuowushinian/article/details/19023869)
**在介绍抽象类和抽象方法之前还是先提一下多态的基本概念。**
其实在上一篇关于[里氏替换原则](http://blog.jobbole.com/101265/)就已经说明了多态的精髓“子类对象可以替换父类对象的位置，而程序的功能不受影响”。还是来看一段代码吧：
```
C#
			
			
```
/// <summary>
    /// Create By:ZhiQiang
    /// Create Time:2014-2-9
    /// </summary>
    class Person
    {
       
       //定义虚方法以备子类重写，当子类替换父类对象的位置时，可以表现出多态
        public virtual void Run()
        {
            Console.WriteLine("我是人，我会跑！");
        }
        public virtual void Say()
        {
            Console.WriteLine("我是人，我会说话！");
        }
    }
```
```
子类的代码如下:
```
C#
			
			
```
//定义Teacher类继承Person
    class Teacher:Person
    {
        public override void Run()
        {
            Console.WriteLine("我是老师，我必须慢速跑");
        }
        public override void Say()
        {
            Console.WriteLine("我是老师，我得说表扬的话！");
        }
    }
    //定义Student类继承Person
    class Student : Person
    {
        //子类重写了父类的虚方法
        public override void Run()
        {
            Console.WriteLine("我是学生，我会加速跑！");
        }
        public override void Say()
        {
            Console.WriteLine("我是学生，我会说英语！");
        }
    }
```
```
下面需要一个实现多态的类，代码如下：
```
C#
			
			
```
//实现多态的类
    class FeatureHuman
    {
        /// <summary>
        /// 这个方法就提现了多态，当传入的是子类的对象的时候，p指向的是子类对象，就可以调用子类重写父类方法后的方法
        /// </summary>
        /// <param name="p">父类或者子类对象</param>
        public void OutPutFeature(Person p)
        {
            p.Run();
            p.Say();
        }
    }
```
```
主体代码和实现多态的方法如下：
```
C#
			
			
```
static void Main(string[] args)
        {
            FeatureHuman fea = new FeatureHuman();
            //人的特点
            Person p = new Person();
            Program pro = new Program();
            fea.OutPutFeature(p);
            //学生的特点
            Student s = new Student();
            fea.OutPutFeature(s);
            //老师的特点
            Teacher t = new Teacher();
            fea.OutPutFeature(t);
            Console.ReadKey();
        }
```
```
运行，打印结果如下：
![1](http://jbcdn2.b0.upaiyun.com/2016/05/28c8edde3d61a0411511d3b1866f063621.png)
**这里可以发现，我们outputFeature方法根据传入的实体对象不同（父类变量指向了子类的对象），而打印出了不同人物的特点，这就是多态。**
代码图解如下：
![2](http://jbcdn2.b0.upaiyun.com/2016/05/665f644e43731ff9db3d341da5c827e119.png)
多态总结如下：
![3](http://jbcdn2.b0.upaiyun.com/2016/05/38026ed22fc1a91d92b5d2ef93540f2017.png)
**二，抽象类和抽象方法**
在C#中使用abstract关键字修饰的类和方法，叫做抽象类和抽象方法。
**1)抽象类中可以拥有没抽象成员，为了继承给他的子类调用 (抽象类就是为了定义抽象成员，继承给子类去实现，同时子类也可以调用父类的非抽象成员)**
```
C#
			
			
```
abstract  class Person
    {
       //private int nAge;
       //abstract string strName;
       //抽象类可以包含不抽象的成员，可以给继承的子类使用
       public void Say()
       {
           Console.WriteLine("我是父类，我是人！");
       }
       public virtual void Sing()
       {
           Console.WriteLine("我是父类，我是人，我可以唱歌！");
       }
       //Run的抽象方法
       public abstract void Run();
    }
```
```
**2)抽象类中可以有virtual修饰的虚方法**
如上面的代码，在抽象类中定义了virtual修饰的方法，编译通过。抽象类就是为了定义抽象成员，继承给子类去实现，所以子类也可以实现抽象类中的虚方法。
**3)抽象类不能实例化，因为有抽象成员，而抽象成员没有方法体，如下图，**
![4](http://jbcdn2.b0.upaiyun.com/2016/05/011ecee7d295c066ae68d4396215c3d013.png)
**4)抽象成员不能私有，如果私有子类没有办法访问 **
我们可以在抽象类中定义私有成员，但是没有意义。因为子类根本访问不到这些私有成员，而抽象类本身也不能实例化，所以私有成员访问不到。
**5)子类必须重写父类的抽象方法 **
在上面代码的基础上，我们定义一个Student类，继承抽象类，但是不实现抽象类的抽象方法，编译报错。代码如下：
![5](http://jbcdn2.b0.upaiyun.com/2016/05/4e44f1ac85cd60e3caa56bfd4afb675e13.png)
**6)在子类中没有办法通过base关键字调用父类抽象方法 **
原理同上，抽象类的抽象发放没有实现语句，就算调用也没有意义。但是可以使用base关键字调用非抽象方法，代码如下：
```
C#
			
			
```
class Program
    {
        static void Main(string[] args)
        {
            //Person p = new Person();
            Student s = new Student();
            s.Run();
            Console.ReadLine();
        }
    }
    class Student : Person
    {
       
        public override void Run()
        { 
            base.Say();
            Console.WriteLine("我是学生，继承了父类，我可以跑！");
        }
    }
```
```
打印结果如下：
![6](http://jbcdn2.b0.upaiyun.com/2016/05/3d2f8900f2e49c02b481c2f717aa902010.png)
抽象类思维导图总结如下：
![7](http://jbcdn2.b0.upaiyun.com/2016/05/cd7fd1517e323f26c6f1b0b6b96e3b3d10.png)
抽象方法
**1）抽象方法必须定义在抽象类中，**
```
C#
			
			
```
class Student : Person
    {
        public abstract void Swiming();
        public override void Run()
        { 
            base.Say();
            Console.WriteLine("我是学生，继承了父类，我可以跑！");
        }
    }
```
```
代码编译会报错，如下图：
![8](http://jbcdn2.b0.upaiyun.com/2016/05/815e6212def15fe76ed27cec7a393d599.png)
**2）抽象方法必须使用关键字修饰，示例代码如下：**
```
C#
			
			
```
abstract class Person
    {
        //private int nAge;
        //abstract string strName;
        //抽象类可以包含不抽象的成员，可以给继承的子类使用
        public void Say()
        {
            Console.WriteLine("我是父类，我是人！");
        }
        public virtual void Sing()
        {
            Console.WriteLine("我是父类，我是人，我可以唱歌！");
        }
        //Run的抽象方法，不能有方法体，留给子类实现
        public abstract void Run();
```
```
抽象方法思维导图总结如下：
![9](http://jbcdn2.b0.upaiyun.com/2016/05/4c0d13d3ad6cc317017872e51d01b2388.png)
** 那么什么时候使用抽象类呢？**
**①子类必须重写父类的方法（相当于定义了一个标准，规范）**
**②父类没有必要实例化，就用抽象类**
**③抽象类是为了继承，为了多态**
最后来看一个示例代码：
定义一个抽象类，其中包含抽象方法Run()
```
C#
			
			
```
abstract class Person
    {//Run的抽象方法，只要是继承我的子类都要实现这个方法
        public abstract void Run();
    }
```
```
分别定义两个子类，继承抽象类Person
```
C#
			
			
```
class Student : Person
    {
        //public abstract void Swiming();
        public override void Run()
        { 
           // base.Say();
            Console.WriteLine("我是学生，继承了父类，我可以跑！");
        }
    }
    class Worker:Person
    {
        public override void Run()
        {
            Console.WriteLine("我是工人，继承了父类，我每天在厂区跑！");
        }
    }
```
```
为了表现多态，我们编写一个方法如下：
```
C#
			
			
```
//该方法变现了多态，根据需要返回子类的对象
        public static Person GetEntity(string str)
        {
            if(str=="学生")
            {
                return new Student();
            }
            else if(str=="工人")
            {
                return new Worker();
            }
            return null;
        }
```
```
main函数中的代码如下：
```
C#
			
			
```
static void Main(string[] args)
        {
            //不直接实例化父类对象，只是声明一个父类对象的变量来接收方法的返回值
            Person p = GetEntity(Console.ReadLine());
            p.Run();
            Console.ReadLine();
        }
```
```
运行，分别输入“工人”和“学生”的打印结果如下：
![10](http://jbcdn2.b0.upaiyun.com/2016/05/8d8e353b98d5191d5ceea1aa3eb05d436.png)
