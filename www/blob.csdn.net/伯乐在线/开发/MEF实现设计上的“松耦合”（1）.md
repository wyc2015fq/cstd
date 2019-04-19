# MEF实现设计上的“松耦合”（1） - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4760735.html)
前言：最近去了趟外地出差，介绍推广小组开发的框架类产品。推广对象是本部门在项目上面的同事——1到2年工作经验的初级程序员。在给他们介绍框架时发现很多框架设计层面的知识他们都没有接触过，甚至没听说过，这下囧了~~于是乎在想该如何跟他们解释MEF、AOP、仓储模式等方面的东东。C#进阶系列主要围绕MEF、AOP、仓储模式、Automapper、WCF等展开。本篇先来介绍下MEF的基础知识。
1、什么是MEF
先来看msdn上面的解释：MEF(Managed Extensibility Framework)是一个用于创建可扩展的轻型应用程序的库。 应用程序开发人员可利用该库发现并使用扩展，而无需进行配置。 扩展开发人员还可以利用该库轻松地封装代码，避免生成脆弱的硬依赖项。 通过 MEF，不仅可以在应用程序内重用扩展，还可以在应用程序之间重用扩展。
也有人把MEF解释为“依赖注入”的一种方式，那么什么是“依赖注入”？如果这样解释，感觉越陷越深……根据博主的理解，了解MEF只需要抓住以下几个关键点：
（1）字面意思，可扩展的framework，或者叫可扩展的库。也就是说，使用MEF是为了提高程序的可扩展性。MEF会根据指定的导入导出自动去发现匹配的扩展，不需要进行复杂的程序配置。
（2）在设计层面上来说，为什么要使用MEF？为了“松耦合”！我们知道，程序设计有几个原则，“高内聚，低耦合”就是其中一个。使用MEF可以帮助我们减少内库之间的耦合。
当然，如果你之前压根都没有听说过MEF，那么即使看了我上面的解释，估计也还是云里雾里。没关系，如果此刻你还有兴趣，看了下面的Demo，相信你会有一个初步的认识。
2、为什么要使用MEF：上面已经解释过，为了程序的扩展和“松耦合”。
3、MEF的使用：
（1）MEF基础导入导出的使用：
MEF的使用步骤主要分三步：宿主MEF并组合部件、标记对象的导出、对象的导入使用。
我们先来看一个Demo。

C#
```
class Program2
    {
　　　　 //导入对象使用
        [Import("chinese_hello")]
        public Person oPerson { set; get; }
        static void Main(string[] args)
        {
            var oProgram = new Program2();
            oProgram.MyComposePart();
            var strRes = oProgram.oPerson.SayHello("李磊");
            Console.WriteLine(strRes);
            Console.Read();
        }
　　　　 //宿主MEF并组合部件
        void MyComposePart()
        {
            var catalog = new AssemblyCatalog(Assembly.GetExecutingAssembly());
            var container = new CompositionContainer(catalog);
            //将部件（part）和宿主程序添加到组合容器
            container.ComposeParts(this);
        }
    }
 　　public interface Person
    {
        string SayHello(string name);
    }
　　 //声明对象可以导出
    [Export("chinese_hello", typeof(Person))]
    public class Chinese : Person
    {
        public string SayHello(string name)
        {
            return "你好：" + name ;
        }
    }
    [Export("american_hello", typeof(Person))]
    public class American : Person
    {
        public string SayHello(string name)
        {
            return "Hello:" + name ;
        }
    }
```
得到结果：
![](http://images2015.cnblogs.com/blog/459756/201508/459756-20150830144340000-1716942438.png)
我们来分析下这段代码：

C#
```
//宿主MEF并组合部件
        void MyComposePart()
        {
            var catalog = new AssemblyCatalog(Assembly.GetExecutingAssembly());
            var container = new CompositionContainer(catalog);
            //将部件（part）和宿主程序添加到组合容器
            container.ComposeParts(this);
        }
```
这个方法表示添加当前Program2这个类到组合容器，为什么要添加到组合容器？是因为只要添加到组合容器中之后，如果该类里面有Import，MEF才会自动去寻找对应的Export。这也就是为什么使用MEF前必须要组合部件的原因。

C#
```
[Export("chinese_hello", typeof(Person))]
    public class Chinese : Person
    {
        public string SayHello(string name)
        {
            return "你好：" + name ;
        }
    }
```
这里的[Export(“chinese_hello”, typeof(Person))]这个特性表示标记Chinese类的导出。
将Export转到定义可以看到：

C#
```
//
        // 摘要: 
        //     通过在指定协定名称下导出指定类型，初始化 System.ComponentModel.Composition.ExportAttribute 类的新实例。
        //
        // 参数: 
        //   contractName:
        //     用于导出使用此特性标记的类型或成员的协定名称，或 null 或空字符串 ("") 以使用默认协定名称。
        //
        //   contractType:
        //     要导出的类型。
        public ExportAttribute(string contractName, Type contractType);
```
这里的两个参数：第一个表示协定名称，如果找到名称相同的Import，那么就对应当前的Chinese对象；第二个参数表示要导出的类型。

C#
```
[Import("chinese_hello")]
 public Person oPerson { set; get; }
```
这里的chinese_hello是和Export里面的chinese_hello对应的，由此可知，每一个[Import(“chinese_hello”)]这种Import一定可以找到一个对应的Export，如果找不到，程序就会报异常。当然如果这里的Import如果改成[Import(“american_hello”)]，那么oPerson肯定就对应一个American对象。
通过上面的程序可以知道，我们使用[Import]这个特性，它的底层其实就是给我们初始化了一个对象。例如上面的[Import(“chinese_hello”)]等价于Person oPerson=new Chinese();。看到这里可能有人就会说这个Import是多此一举了，既然我们可以new，为什么非要用这种奇怪的语法呢，怪别扭的。其实如果我们站在架构的层面，它的好处就是可以减少dll之间的引用。这个留在下一篇来讲。
（2）MEF导入导出扩展：
按照MEF的约定，任何一个类或者是接口的实现都可以通过[System.ComponentModel.Composition.Export] 属性将其他定义组合部件(Composable Parts)，在任何需要导入组合部件的地方都可以通过在特定的组合部件对象属性上使用[System.ComponentModel.Composition.Import ]实现部件的组合，两者之间通过契约(Contracts)进行通信。通过上面的例子我们可以知道，对象是可以通过Import和Export来实现导入和导出的，那么我们进一步扩展，对象的属性、字段、方法、事件等是否也可以通过[ImportAttribute]进行导入呢？

C#
```
class Program2
    {
        [Import("TestProperty")]
        public string ConsoleTest { get; set; }
        static void Main(string[] args)
        {
            var oProgram = new Program2();
            oProgram.MyComposePart();
            Console.WriteLine(oProgram.ConsoleTest);
            Console.Read();
        }
        void MyComposePart()
        {
            var catalog = new AssemblyCatalog(Assembly.GetExecutingAssembly());
            var container = new CompositionContainer(catalog);
            //将部件（part）和宿主程序添加到组合容器
            container.ComposeParts(this);
        }
    }
    public class TestPropertyImport
    {
        [Export("TestProperty")]
        public string TestMmport { get { return "测试属性可以导入导出"; } }
    }
复制代码
```
得到结果：
![](http://jbcdn2.b0.upaiyun.com/2016/04/56d9d47b48fea6ecf863a2706e444cfa.png)
由此说明，属性也是可以导入导出的。原理与上类似。既然属性可以，那么字段就不用演示了，它和属性应该是类似的。
下面来看看方法是否可以呢？

C#
```
class Program2
    {
        [Import("chinese_hello")]
        public Person oPerson { set; get; }
        [Import("TestProperty")]
        public string ConsoleTest { get; set; }
        [Import("helloname")]
        public Action<string> TestFuncImport { set; get; }
        static void Main(string[] args)
        {
            var oProgram = new Program2();
            oProgram.MyComposePart();
            oProgram.TestFuncImport("Jim");
            //Console.WriteLine(oProgram.ConsoleTest);
            //var strRes = oProgram.oPerson.SayHello("李磊");
            //Console.WriteLine(strRes);
            Console.Read();
        }
        void MyComposePart()
        {
            var catalog = new AssemblyCatalog(Assembly.GetExecutingAssembly());
            var container = new CompositionContainer(catalog);
            //将部件（part）和宿主程序添加到组合容器
            container.ComposeParts(this);
        }
    }
    public class TestPropertyImport
    {
        [Export("TestProperty")]
        public string TestMmport { get { return "测试属性可以导入导出"; } }
        [Export("helloname", typeof(Action<string>))]
        public void GetHelloName(string name)
        {
            Console.WriteLine("Hello：" + name);
        }
    }
```
由此可知，方法的导入和导出是通过匿名委托的方式实现的，那么由此类推，事件应该也是可以的，有兴趣的朋友可以一试。原理和上面是一样一样的。
既然属性、字段、方法、事件都可以通过Import和Export实现单一对象或变量的导入和导出，那么如果我们想要一次导入多个对象呢？嘿嘿，微软总是体贴的，它什么都为我们考虑到了。我们来看看如何实现。

C#
```
class Program2
    {
        [ImportMany]
        public IEnumerable<Person> lstPerson { set; get; }
        static void Main(string[] args)
        {
            var oProgram = new Program2();
            oProgram.MyComposePart();
            Console.WriteLine(oProgram.lstPerson.Count());
            Console.Read();
        }
        void MyComposePart()
        {
            var catalog = new AssemblyCatalog(Assembly.GetExecutingAssembly());
            var container = new CompositionContainer(catalog);
            //将部件（part）和宿主程序添加到组合容器
            container.ComposeParts(this);
        }
    }
public interface Person
    {
        string SayHello(string name);
    }
    [Export(typeof(Person))]
    public class Chinese : Person
    {
        public string SayHello(string name)
        {
            return "你好：" + name ;
        }
    }
    [Export(typeof(Person))]
    public class American : Person
    {
        public string SayHello(string name)
        {
            return "Hello:" + name ;
        }
    }
```
得到的结果为2。这里有一点需要注意的，使用ImportMany的时候对应的Export不能有chinese_hello这类string参数，否则lstPerson的Count()为0.
（3）MEF的延迟加载
我们知道，当装配一个组件的时候，当前组件里面的所有的Import的变量都自动去找到对应的Export而执行了实例化，有些时候，出于程序效率的考虑，不需要立即实例化对象，而是在使用的时候才对它进行实例化。MEF里面也有这种延迟加载的机制。

C#
```
class Program2
    {
        [Import("chinese_hello")]
        public Person oPerson { set; get; }
        [Import("american_hello")]
        public Lazy<Person> oPerson2 { set; get; }
　　　　 static void Main(string[] args)
        {
            var oProgram = new Program2();
            oProgram.MyComposePart();
            var strRes = oProgram.oPerson.SayHello("李磊");
            var strRes2 = oProgram.oPerson2.Value.SayHello("Lilei");
            Console.WriteLine(strRes);
            Console.Read();
        }
        void MyComposePart()
        {
            var catalog = new AssemblyCatalog(Assembly.GetExecutingAssembly());
            var container = new CompositionContainer(catalog);
            //将部件（part）和宿主程序添加到组合容器
            container.ComposeParts(this);
        }
    }
　　 public interface Person
    {
        string SayHello(string name);
    }
    [Export("chinese_hello", typeof(Person))]
    public class Chinese : Person
    {
        public string SayHello(string name)
        {
            return "你好：" + name ;
        }
    }
    [Export("american_hello", typeof(Person))]
    public class American : Person
    {
        public string SayHello(string name)
        {
            return "Hello:" + name ;
        }
    }
```
通过调试可知，当程序运行到var strRes = oProgram.oPerson.SayHello(“李磊”);这一行的时候
![](http://jbcdn2.b0.upaiyun.com/2016/04/227da4b036ec2091101541b3d3398d3e.png)
oPerson对象已经实例化了，而oPerson2.Value对象没有实例化，当程序执行var strRes2 = oProgram.oPerson2.Value.SayHello(“Lilei”)这一句的时候，oPerson2.Value对象才进行实例化。这种需要在某些对程序性能有特殊要求的情况下面有一定的作用。
讲到这里，我们再来看前面关于理解MEF的两个关键点：
（1）可扩展的库：由于MEF允许通过Import的方式直接导入对象、属性、方法等，试想，有人开发了一个组件，他们事先定义好了一系列的导出(Export)，我们只需要将它的组件引进来，使用Import的方式按照他们Export的约定导入对象即可，不用做其他复杂的配置。
（2）能更好的实现“松耦合”：比如我们项目按照面向接口编程的方式这样分层：UI层、BLL接口层、BLL实现层……UI层只需要引用BLL接口层即可，我们在BLL实现层里面定义好Export的导出规则，然后再UI层里面使用Import导入BLL实现层的对象即可，这样UI层就不需要添加BLL实现层的引用。减少了dll之间的依赖。
以上就是MEF的一些基础用法。当然在实际使用中可能不会这么简单，但是再复杂的用法都是在这些简单基础上面扩展起来的。后面还有两篇会继续分享MEF在项目设计层面的用法以及带来的好处。欢迎各位拍砖斧正~~
