# C#中易混淆概念：解析里氏替换原则，虚方法 - 文章 - 伯乐在线
原文出处： [zhiqiang21](http://blog.csdn.net/yisuowushinian/article/details/18976629)
今天开始上班了。这几天研究学习了一下思维导图，感觉用它整理自己的知识非常的方便。所以，以后写博客完成一个知识点，都会用思维导图做一个总结。也能让大家对所要读的内容有一个整体的把握。
我用的思维导图软件是FreeMind（免费的，但是得装JDK）,因为刚开始学习使用，很多操作技巧不是很熟练，做出来的导图估计也不是很好，希望大家见谅。
**首先，里氏替换原则。**
这是理解多态所必须掌握的内容。对于里氏替换原则维基百科给出的定义如下：
![](http://jbcdn2.b0.upaiyun.com/2016/05/157181854dd05a06f771d222cf5d2fbd.png)
**为什么子类可以替换父类的位置，而程序的功能不受影响呢？**
当满足继承的时候，父类肯定存在非私有成员，子类肯定是得到了父类的这些非私有成员**（****假设，父类的的成员全部是私有的，那么子类没办法从父类继承任何成员，也就不存在继承的概念了）**。既然子类继承了父类的这些非私有成员，那么父类对象也就可以在子类对象中调用这些非私有成员。所以，子类对象可以替换父类对象的位置。
来看下面的一段代码：
```
C#
			
			
```
class Program
    {
        static void Main(string[] args)
        {
            Person p = new Person();
            Person p1 = new Student();
            Console.ReadKey();
        }
    }
    class Person
    {
　　　　//父类的私有成员
　　　　private int nAge;
        public Person()
        {
            Console.WriteLine("我是Person构造函数，我是一个人！");
        }
        public void Say()
        {
            Console.WriteLine("我是一个人！");
        }
    }
    class Student : Person
    {
        public Student()
        {
            Console.WriteLine("我是Student构造函数，我是一个学生！");
        }
        public void SayStude()
        {
            Console.WriteLine("我是一个学生！");
        }
    }
    class SeniorStudent : Student
    {
        public SeniorStudent()
        {
            Console.WriteLine("我是SeniorStudent构造函数，我是一个高中生！");
        }
        public  void SaySenior()
        {
            Console.WriteLine("我是一个高中生！");
        }
    }
```
```
我们运行打印出的结果是：
![](http://jbcdn2.b0.upaiyun.com/2016/05/319a46fc0975fc5225feca73e3f0aec5.png)
根据前面的构造函数的知识很容易解释这个结果。那么我们在Main（）函数中添加如下的代码：
```
C#
			
			
```
static void Main(string[] args)
        {
            Person p = new Person();
            p.Say();
　　
            Person p1 = new Student();
            p1.Say();
            Console.ReadKey();
        }
```
```
**在访问的过程中，可以发现p只可以访问父类的say**
![](http://jbcdn2.b0.upaiyun.com/2016/05/c9de49feecfc947001d326fa21f63bbb.png)
**而p1也只可以访问父类的Say方法**
![](http://jbcdn2.b0.upaiyun.com/2016/05/f7407030b93abc6357013d9438844681.png)
**其实在上面的代码中，就满足了里氏替换原则。子类的Student对象，替换了父类Person对象的位置。**
那么它们在内存中发生了些什么呢？如下图：
![](http://jbcdn2.b0.upaiyun.com/2016/05/302b92dd7f8d3700d1b35006551604d8.png)
**由上可以知道，当一个父类的变量指向一个子类对象的时候只能通过这个父类变量调用父类成员，子类独有的成员无法调用。**
同理我们可以推理出，子类的变量是不可以指向一个父类的对像的
![](http://jbcdn2.b0.upaiyun.com/2016/05/b56da7b6310f54d0e726dda6a94944aa.png)
但是当父类变量指向一个子类变量的时候，可以不可以把父类的变量转化成子类的对象呢？看下图
![](http://jbcdn2.b0.upaiyun.com/2016/05/1bb6e27734d5bd07db55e63ba08597bc.png)
**关于引用类型的两种转换方式：**
由上面的代码我们已经知道了一种转换，就是在变量钱直接加需要转换的类型，如下代码：
```
C#
			
			
```
Student s2 = (Student)p1;
```
```
那么第二种转换方式就是使用as关键字，如下代码：
```
C#
			
			
```
//将指向子类对象的变量转化成子类类型
            Student s2 = (Student)p1;
            //使用as关键字，转换失败返回一个null值
            Student s3 = p1 as Student;
```
```
使用as关键字和第一种强制转换的区别就是，第一种如果转换失败会抛异常，第二种转换失败则返回一个null值。
**思维导图总结如下：**
![](http://jbcdn2.b0.upaiyun.com/2016/05/57a64da118b927a73803afdfa28ba745.png)
** 二，虚方法**
使用virtual关键字修饰的方法，叫做虚方法（一般都是在父类中）。
看下面的一段代码：
```
C#
			
			
```
class Person
    {
        private int nAge;
        public Person()
        {
            Console.WriteLine("我是Person构造函数，我是一个人！");
        }
        //这里定义了一个虚方法
        public virtual void Say()
        {
            Console.WriteLine("我是一个人！");
        }
    }
    class Student : Person
    {
        //子类使用override关键字改写了父类的虚方法
        public override void Say()
        {
            Console.WriteLine("我是一个学生！");
        }
        public Student()
        {
            Console.WriteLine("我是Student构造函数，我是一个学生！");
        }
        public void SayStude()
        {
            Console.WriteLine("我是一个学生！");
        }
    }
```
```
紧接着在main（）函数中添加如下的代码：
```
C#
			
			
```
static void Main(string[] args)
        {
            Person p = new Person();
            p.Say();
            Person p1 = new Student();
            p1.Say();
            Student s = new Student();
            s.Say();
            Console.ReadKey();
        }
```
```
打印结果如下:
![](http://jbcdn2.b0.upaiyun.com/2016/05/fdf5bba3198386fad7836590239d36e0.png)
我们很明显的可以发现，第二个表达式满足里氏替换原则，p1.Say()执行的应该是父类的Say()方法，但是这里却执行了子类的Say()方法。
这就是子类使用override关键字的Say()方法覆盖了父类的用Virtual关键字修饰的Say()方法。
我们使用动态图片看一下调试过程，
**①首先是没有使用任何关键字：**
![](http://jbcdn2.b0.upaiyun.com/2016/05/57974c311be2447ef7ed861adfa4388d.gif)
**由上可以看出直接跳入父类，执行了父类的Say()方法；**
**②再看使用virtual和override关键字的动态调试图片，如下**：
![](http://jbcdn2.b0.upaiyun.com/2016/05/9680d3c31ef5649f370fc86087be4e0b.gif)
**可以看到直接到子类去执行override关键字修饰的Say()方法。**
那么如果父类使用virtual关键字修饰，而子类没有重写该方法时会怎么样呢？如下面的代码：
```
C#
			
			
```
class Program
    {
        static void Main(string[] args)
        {
           
            Person p1 = new Student();
            p1.Say();
            Console.ReadKey();
        }
    }
    class Person
    {
        private int nAge;
        public Person()
        {
            Console.WriteLine("我是Person构造函数，我是一个人！");
        }
        //这里定义了一个虚方法
        public virtual void Say()
        {
            Console.WriteLine("我是一个人！");
        }
    }
    class Student : Person
    {
        //子类中没有出现override关键字修饰的方法
        public void SayStude()
        {
            Console.WriteLine("我是一个学生！");
        }
    }
```
```
执行结果如下：
![](http://jbcdn2.b0.upaiyun.com/2016/05/1f0bab533c7cd7396f9f6f0e8444bc86.png)
所以，如果子类找不到override方法，则会回溯到该子类的父类去找是否有override方法，知道回溯到自身的虚方法，并执行。
虚方法知识总结的思维导图如下：
![](http://jbcdn2.b0.upaiyun.com/2016/05/ca156d3c8997559c8034920e4b10956b.png)
