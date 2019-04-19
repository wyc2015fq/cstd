# MEF实现设计上的“松耦合”（终结篇）面向接口编程 - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4770609.html)
序：忙碌多事的八月带着些许的倦意早已步入尾声，金秋九月承载着抗战胜利70周年的喜庆扑面而来。没来得及任何准备，似乎也不需要任何准备，因为生活不需要太多将来时。每天忙着上班、加班、白加班，忘了去愤，忘了去算计所谓的价值。天津爆炸事故时刻警示着我们生命的无常，逝者安息，活着的人生活还得继续，珍惜生命，远离伤害。武汉，这座炙热的城市，虽值金秋，却依然经受着“秋老虎”的烘烤，马路上蒸腾的热气迎面袭来，全身毛孔张开，汗流不止，在这般高温下，似乎汗水都要被榨干，其实，被榨干的何止是汗水！！！吁！吁！吁！说好的MEF呢？说好的面向接口编程呢？都快奔三张的人了，还学着小年轻玩无病呻吟，有点装嫩的味道。没办法，思想脱缰了，有点野性难驯的意思了。好啦，不扯啦，进入今天的正题吧。
前面两篇分别介绍了下MEF的简单用法和MEF与仓储模式的结合使用，这章来个终结吧。毛爷爷教导我们，做事要有始有终。本篇，博主打算通过分享一个面向接口编程的框架来说明使用MEF的灵活性。
1、面向接口编程：有一定编程经验的博友应该都熟悉或者了解这种编程思想，层和层之间通过接口依赖，下层不是直接给上层提供服务，而是定义一组接口供上层调用。至于具体的业务实现，那是开发中需要做的事情，在项目架构阶段，只需要定义好层与层之间的接口依赖，将框架搭起来，编译可以直接通过。为什么要有这么一种设计？既然是架构设计，当然是为了提高架构的灵活性，降低层和层之间的依赖（耦合）。
2、博主本着“不讲清楚誓不罢休”的原则，自己从零开始搭了一个简单的框架Demo，当然，可能对于大牛们来说是没太大价值的，但请不要笑话博主不断探索的勇气。先来看看框架大概的结构吧。
![](http://jbcdn2.b0.upaiyun.com/2016/04/a1e6896e9944f76cfc19d78aa631bdc2.png)
首先说明下各层次的意思：
一、ESTM.Client
　　ESTM.Client.Winform：Winform项目，用户UI展现，这个没什么好说的。
　　ESTM.Client.IBLL：客户端IBLL接口层，用于定义客户端的业务接口，记住这里仅仅是向UI层提供接口功能。
　　ESTM.Client.BLL：客户端BLL实现层，用于客户端IBLL接口层的实现，提供UI层真是业务逻辑。
二、ESTM.Common
　　ESTM.Common.Model：通用DTOModel层，注意，这里不是EF的实体Model，而是另外定义的一个数据转换的Model层。
三、ESTM.Service
　　ESTM.Service.WCF：WCF宿主项目，用于提供WCF的接口契约和实现。这里用WCF的目的是为了隔离客户端和服务端的代码。
　　ESTM.Service.IBLL：服务端IBLL接口层，用于定义WCF层的业务接口，和ESTM.Client.IBLL层的功能类似。
　　ESTM.Service.BLL：服务端BLL实现层，实现服务端IBLL接口层。
　　ESTM.Service.DAL：服务端DAL数据访问层，里面使用EF建立数据库连接。
再来看看各层次之间的调用关系：
![](http://jbcdn2.b0.upaiyun.com/2016/04/487a715eaccf2fab7efed8d42caa72ce.png)
最后说说这样设计的好处：
（1）整个框架采用面向接口编程模式，每个层次不是直接向其上层提供服务（即不是直接实例化在上层中），而是通过定义一组接口，仅向上层暴露其接口功能，上层对下层仅仅是接口依赖，而不依赖具体实现。如是说，客户端IBLL接口层仅仅提供一套接口供UI层调用，对于UI层来说，它根本感觉不到客户端BLL实现层的存在，极端点说，即使不写BLL实现层，项目也可以编译通过，因为接口的功能已经定义好了。至于具体的实现，那就是业务的问题了。当我们需要更改业务逻辑时，只需要更改BLL实现层的代码就好了，对于IBLL接口层和上层UI不用做任何的改变，更进一步说，甚至将客户端BLL实现层全部重写或者整个替换掉，IBLL和UI层都可以不做任何改变。这也正是面向接口编程最大的优势。
（2）上张图里面也提到了DTOModel层，为什么要有DTOModel这么一个对象，而不是直接将EF的实体Model传到前端来呢？个人觉得原因有两点：一是上文提到的安全性问题，客户端永远只能操作DTOmodel，当客户端提交数据到后台来时，永远都是先将DTOmodel转换位EF的model，然后去操作数据库，试想，如果UI表现层能直接操作EF的model，是否会造成操作数据库的入口的不唯一的问题；二是，比如数据库里面有A和B两张表，我们前端需要展示A表的A.1、A.2两字段，还需要展示B表的B.3、B.4字段，当我们使用DTOmodel的时候，只需要构造好一个DTO_Model，里面有4个字段，前端可以直接拿来用就好了，如果不用DTO，要么直接传object，要么将A、B两张表的模型传过来在前端构造，无论哪种方式应该都没有使用DTO方便吧。
当然这些都是博主自己的理解，如果博友们觉得有问题可以指出~~
好了，说了这么多框架，下面进入今天的正题。看看MEF是如何在项目中飞的吧~~先来看看各层的代码：
（1）ESTM.Service.DAL里面通过EF建立数据库的连接 ：博主为了测试随便拖了一张用户表进来。
![](http://jbcdn2.b0.upaiyun.com/2016/04/404b9de37635cbdea66dbaa8212c2ee0.png)
**Base.cs里面通过MEF导入EF的上下文对象：**

C#
```
public class Base
    {
        [Import]
        public DbContext EntityFramework { set; get; }
        public Base()
        {
            //因为这里有Import，所以需要装配MEF
            regisgter().ComposeParts(this);
        }
        public CompositionContainer regisgter()
        {
            var catalog = new AssemblyCatalog(Assembly.GetExecutingAssembly());
            var container = new CompositionContainer(catalog);
            return container;
        }
    }
```
对应在Export在edmx文件下面的MyModel.Context.cs里面

C#
```
[Export(typeof(DbContext))]
    public partial class Entities : DbContext
    {
        public Entities()
            : base("name=Entities")
        {
        }
        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            throw new UnintentionalCodeFirstException();
        }
        public DbSet TB_USERS { get; set; }
    }
```
（2）ESTM.Service.IBLL服务端IBLL接口层定义服务端接口：

C#
```
public interface IServiceUser
    {
        List GetAllUser();
        void AddUser(DTO_USERS oUser);
    }
```
（3）ESTM.Service.BLL服务端BLL实现层定义接口实现：

C#
```
[Export("Users",typeof(IServiceUser))]
    public class ServiceUser : IServiceUser
    {
        //需要注意：1.添加服务引用在Client.Bll里面，所以，WCF连接的配置要拷贝到Winform项目下面的App.Config里面
        //2.DAL里面的连接字符串也要拷贝到WCF里面，原因同上
        public List GetAllUser()
        {
            var lstRes = new List();
            var oService = new DAL.ServiceUser();
            var lstEFModel = oService.GetAllUsers();
            //一般用AutoMapper将EF的Model转换成DTO的Model.z这里为了测试，我们暂且手动转换。使用反射转换
            var lstEFModelProp = typeof(TB_USERS).GetProperties();
            var lstDTOModelProp = typeof(DTO_USERS).GetProperties();
            foreach (var oEFModel in lstEFModel)
            {
                var oResUser = new DTO_USERS();
                foreach (var oProp in lstEFModelProp)
                {
                    var oDTOMOdelProp = lstDTOModelProp.FirstOrDefault(x => x.Name == oProp.Name);
                    if (oDTOMOdelProp == null)
                    {
                        continue;
                    }
                    oDTOMOdelProp.SetValue(oResUser, oProp.GetValue(oEFModel));
                }
                lstRes.Add(oResUser);
            }
            return lstRes;
        }
        public void AddUser(DTO_USERS oUser)
        {
        }
```
**注意在BLL实现层里面有EF的Model和DTOmodel之间的转换，因为在DAL里面取到的是EF的实体模型，而需要传到前端的是DTOmodel的模型，项目中一般用AutoMapper等第三方工具转换对象，我这里为了简单自己手动通过反射转了下。**
（4）ESTM.Service.WCF服务端WCF宿主层，定义WCF的接口契约。

C#
```
static void Main(string[] args)
        {
            var strUri = "http://127.0.0.1:1234/MyWCF.Server";
            Uri httpAddress = new Uri(strUri);
            using (ServiceHost host = new ServiceHost(typeof(CSOAService)))//需要添加System.SystemModel这个dll。。。。CSOAService这个为实现ICSOAService的实现类，WCF真正的实现方法再这个类里面
            {
                ///////////////////////////////////////添加服务节点///////////////////////////////////////////////////
                host.AddServiceEndpoint(typeof(ICSOAService), new WSHttpBinding(), httpAddress);//ICSOAService这个为向外暴露的接口
                if (host.Description.Behaviors.Find<ServiceMetadataBehavior>() == null)
                {
                    ServiceMetadataBehavior behavior = new ServiceMetadataBehavior();
                    behavior.HttpGetEnabled = true;
                    behavior.HttpGetUrl = httpAddress;
                    host.Description.Behaviors.Add(behavior);
                }
                host.Opened += delegate
                {
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.WriteLine("MyWCF.Server服务已经启动成功。" + strUri);
                };
                host.Open();
                while (true)
                {
                    Console.ReadLine();
                }
            }
        }
```


C#
```
[ServiceContract]
    public interface ICSOAService
    {
        [OperationContract]
        List<DTO_USERS> GetAllUsers();
    }
```


C#
```
public class CSOAService:ICSOAService
    {
        [Import("Users")]
        public IServiceUser Service { set; get; }
        public CSOAService()
        {
            regisgterAll().ComposeParts(this);
        }
        public List<DTO_USERS> GetAllUsers()
        {
            return Service.GetAllUser();
        }
        public CompositionContainer regisgterAll()
        {
            AggregateCatalog aggregateCatalog = new AggregateCatalog();
            var thisAssembly = new DirectoryCatalog(AppDomain.CurrentDomain.BaseDirectory, "*.dll");
            aggregateCatalog.Catalogs.Add(thisAssembly);
            var _container = new CompositionContainer(aggregateCatalog);
            return _container;
        }
```
**代码没什么复杂的逻辑，就是先注册MEF实例化变量，然后取值。[Import(“Users”)]这里有导入，根据我们前两篇的讲解，那么肯定是存在一个[Export(“Users”)]这样的导出，于是乎，我们可以根据IServiceUser 接口往下找，最后可以找到在ESTM.Service.BLL这个里面有一个如下的导出：**

C#
```
[Export("Users",typeof(IServiceUser))]
    public class ServiceUser : IServiceUser
    {
       //........  
    }
```
（5）ESTM.Client.IBLL客户端IBLL接口层

C#
```
public interface IManagerUser
    {
        List<DTO_USERS> GetAllUser();
    }
```
（6）ESTM.Client.BLL客户端BLL实现层

C#
```
[Export("Users",typeof(IManagerUser))]
    public class ManagerUser : IManagerUser
    {
        public List<Common.Model.DTO_USERS> GetAllUser()
        {
　　　　　　　//WCF服务对象
            var oWCFService = new ServiceReference_MyWCF.CSOAServiceClient();
            return oWCFService.GetAllUsers().ToList();
        }
    }
```
**在这个层里面是通过WCF服务去调用数据的，所以需要添加WCF的服务引用。**
（7）ESTM.Client.Winform客户端UI层：定义一个DataGridView展示列表：

C#
```
public partial class Form1 : Form
    {
        [Import("Users")]
        public IManagerUser Manager { set; get; }
        public Form1()
        {
            InitializeComponent();
            regisgterAll().ComposeParts(this);
            this.dataGridView1.DataSource = Manager.GetAllUser();
        }
        public CompositionContainer regisgterAll()
        {
            AggregateCatalog aggregateCatalog = new AggregateCatalog();
            var thisAssembly = new DirectoryCatalog(AppDomain.CurrentDomain.BaseDirectory, "*.dll");
            aggregateCatalog.Catalogs.Add(thisAssembly);
            var _container = new CompositionContainer(aggregateCatalog);
            return _container;
        }
    }
```
得到结果：
![](http://jbcdn2.b0.upaiyun.com/2016/04/dca470b9b1a321156bec8d928fc2f4d9.png)
前面MEF的第一篇中已经说过使用MEF的优势之一就是降低层与层之间的耦合，我们现在来结合框架说说它是如何作业的。首先我们来看看ESTM.Client.Winform这个项目的引用：
![](http://jbcdn2.b0.upaiyun.com/2016/04/26e33c7e689dbf30927acb67de4e3085.png)
它是没有添加ESTM.Client.BLL这一层的引用的，可是我们在Form1.cs里面有如下代码：
程序运行起来，走完注册MEF以后可以看到Manager的变量值就是ESTM.Client.BLL里面的ManagerUser对象。这就是MEF的功劳，当调用regisgterAll()这个方法的时候，MEF会根据导入导出自动去寻找匹配，并且自动实例化。如果是没有MEF，我们UI层就必须要添加ESTM.Client.BLL的引用了。**当然有一点需要注意的地方，虽然UI层不用添加ESTM.Client.BLL的引用，但是由于在UI里面使用了ManagerUser这个对象，所以UI层bin目录下面必须要有ESTM.Client.BLL.dll这个文件以及ESTM.Client.BLL项目所必须的dll，你可以手动拷贝这些dll到UI的bin目录下面。甚至为了简单，你也可以在UI层上面添加ESTM.Client.BLL这个的引用，但是博主觉得，这样貌似违背了面向接口编程的原则，不爽，奈何没想到更好的解决方案。**
**对于上面UI层必须要添加BLL实现层这一问题找到解决方案了，在此记录下：**

C#
```
public partial class Form1 : Form
    {
        [Import("Users")]
        public IManagerUser Manager { set; get; }
        public Form1()
        {
            InitializeComponent();
            regisgterAll().ComposeParts(this);
            this.dataGridView1.DataSource = Manager.GetAllUser();
        }
        public CompositionContainer regisgterAll()
        {
            AggregateCatalog aggregateCatalog = new AggregateCatalog();
            var thisAssembly = new DirectoryCatalog(AppDomain.CurrentDomain.BaseDirectory, "*.dll");
            aggregateCatalog.Catalogs.Add(thisAssembly);
            var _container = new CompositionContainer(aggregateCatalog);
            return _container;
        }
    }
```
**ESTM.Client.BLL项目右键→属性**
**![](http://jbcdn2.b0.upaiyun.com/2016/04/06b64655aa0283031b014116d48b6c48.png)**
**输出路径改成UI层的bin目录下面即可。2015年9月16日加。**
**在搭建这个小框架过程中，博主遇到几个问题在此和博友分享下：**
**1.添加服务引用在Client.Bll里面，由于Client.BLL是一个内库，最终它会生成一个dll，所以，WCF连接的配置要拷贝到Winform项目下面的App.Config里面。**
**2.DAL里面的连接字符串也要拷贝到WCF的App.Config里面，原因同上。**
**3.注册MEF的方法**

C#
```
public CompositionContainer regisgterAll()
        {
            AggregateCatalog aggregateCatalog = new AggregateCatalog();
            var thisAssembly = new DirectoryCatalog(AppDomain.CurrentDomain.BaseDirectory, "*.dll");
            aggregateCatalog.Catalogs.Add(thisAssembly);
            var _container = new CompositionContainer(aggregateCatalog);
            return _container;
        }
```
**可以抽到一个公共的地方，不用每个地方都写。注意由于MEF的导入导出涉及到多个内库，所以这里要遍历bin目录下面所有的dll去寻找匹配。**
**4.DAL层可以还做一下封装，博主的项目是用的仓储模式封装EF，然后在Service.BLL里面调用仓储的服务去访问数据库。**
