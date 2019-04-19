# 小话设计模式原则之（1）：依赖倒置原则DIP - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/5169163.html)
前言：很久之前就想动笔总结下关于软件设计的一些原则，或者说是设计模式的一些原则，奈何被各种bootstrap组件所吸引，一直抽不开身。群里面有朋友问博主是否改行做前端了，呵呵，其实博主是想做“全战”，即各方便都有战斗力。关于设计模式，作为程序猿的我们肯定都不陌生。博主的理解，**所谓设计模式就是前人总结下来的一些对于某些特定使用场景非常适用的优秀的设计思路，“前人栽树，后人乘凉”，作为后来者的我们就有福了，当我们遇到类似的应用场景的时候就可以直接使用了**。关于设计模式的原则，博主将会在接下来的几篇里面根据自己的理解一一介绍，此篇就先来看看设计模式的设计原则之——依赖倒置原则。
# 一、原理介绍
## 1、官方定义
依赖倒置原则，英文缩写**DIP**，全称Dependence Inversion Principle。
原始定义：High level modules should not depend upon low level modules. Both should depend upon abstractions. Abstractions should not depend upon details. Details should depend upon abstractions。
官方翻译：高层模块不应该依赖低层模块，两者都应该依赖其抽象；抽象不应该依赖细节，细节应该依赖抽象。
## 2、自己理解
### 2.1、原理解释
上面的定义不难理解，主要包含两次意思：
1）高层模块不应该直接依赖于底层模块的具体实现，而应该依赖于底层的抽象。换言之，模块间的依赖是通过抽象发生，实现类之间不发生直接的依赖关系，其依赖关系是通过接口或抽象类产生的。
2）接口和抽象类不应该依赖于实现类，而实现类依赖接口或抽象类。这一点其实不用多说，很好理解，“面向接口编程”思想正是这点的最好体现。
### 2.2、被“倒置”的依赖
相比传统的软件设计架构，比如我们常说的经典的三层架构，UI层依赖于BLL层，BLL层依赖于DAL层。由于每一层都是依赖于下层的实现，这样当某一层的结构发生变化时，它的上层就不得不也要发生改变，比如我们DAL里面逻辑发生了变化，可能会导致BLL和UI层都随之发生变化，这种架构是非常荒谬的！好，这个时候如果我们换一种设计思路，**高层模块不直接依赖低层的实现，而是依赖于低层模块的抽象**，具体表现为我们增加一个IBLL层，里面定义业务逻辑的接口，UI层依赖于IBLL层，BLL层实现IBLL里面的接口，所以具体的业务逻辑则定义在BLL里面，这个时候如果我们BLL里面的逻辑发生变化，只要接口的行为不变，上层UI里面就不用发生任何变化。
**在经典的三层里面，高层模块直接依赖低层模块的实现，当我们将高层模块依赖于底层模块的抽象时，就好像依赖“倒置”了。这就是依赖倒置的由来。通过依赖倒置，可以使得架构更加稳定、更加灵活、更好应对需求变化。**
### 2.3、依赖倒置的目的
上面说了，在三层架构里面增加一个接口层能实现依赖倒置，它的目的就是降低层与层之间的耦合，使得设计更加灵活。从这点上来说，依赖倒置原则也是“松耦合”设计的很好体现。
# 二、场景示例
文章最开始的时候说了，依赖倒置是设计模式的设计原则之一，那么在我们那么多的设计模式中，哪些设计模式遵循了依赖倒置的原则呢？这个就多了，比如我们常见的工厂方法模式。下面博主就结合一个使用场景来说说依赖倒置原则如何能够使得设计更加灵活。
设备有很多类型，每种设备都有登录和采集两个方法，通过DeviceService这个服务去启动设备的采集，最开始我们只有MML和TL2这两种类型的设备，那么来看看我们的设计代码。
代码示例：

C#
```
//MML类型的设备
    public class DeviceMML
    {
        public void Login()
        {
            Console.WriteLine("MML设备登录");
        }
        public bool Spider()
        {
            Console.WriteLine("MML设备采集");
            return true;
        }
    }
    //TL2类型设备
    public class DeviceTL2
    {
        public void Login()
        {
            Console.WriteLine("TL2设备登录");
        }
        public bool Spider()
        {
            Console.WriteLine("TL2设备采集");
            return true;
        }
    }
    //设备采集的服务
    public class DeviceService
    {
        private DeviceMML MML = null;
        private DeviceTL2 TL2 = null;
        private string m_type = null;
        //构造函数里面通过类型来判断是哪种类型的设备
        public DeviceService(string type)
        {
            m_type = type;
            if (type == "0")
            {
                MML = new DeviceMML();
            }
            else if (type == "1")
            {
                TL2 = new DeviceTL2();
            }
        }
        public void LoginDevice()
        {
            if (m_type == "0")
            {
                MML.Login();
            }
            else if (m_type == "1")
            {
                TL2.Login();
            }
        }
        public bool DeviceSpider()
        {
            if (m_type == "0")
            {
                return MML.Spider();
            }
            else if (m_type == "1")
            {
                return TL2.Spider();
            }
            else
            {
                return true;
            }
        }
    }
```
在Main函数里面调用

C#
```
class Program
    {
        static void Main(string[] args)
        {
            var oSpider = new DeviceService("1");
            oSpider.LoginDevice();
            var bRes = oSpider.DeviceSpider();
            Console.ReadKey();
        }
```
上述代码经过开发、调试、部署、上线。可以正常运行，貌似一切都OK。
日复一日、年复一年。后来公司又来两种新的设备TELNET和TL5类型设备。于是程序猿们又有得忙了，加班，赶进度！于是代码变成了这样：

C#
```
//MML类型的设备
    public class DeviceMML
    {
        public void Login()
        {
            Console.WriteLine("MML设备登录");
        }
        public bool Spider()
        {
            Console.WriteLine("MML设备采集");
            return true;
        }
    }
    //TL2类型设备
    public class DeviceTL2
    {
        public void Login()
        {
            Console.WriteLine("TL2设备登录");
        }
        public bool Spider()
        {
            Console.WriteLine("TL2设备采集");
            return true;
        }
    }
    //TELNET类型设备
    public class DeviceTELNET
    {
        public void Login()
        {
            Console.WriteLine("TELNET设备登录");
        }
        public bool Spider()
        {
            Console.WriteLine("TELNET设备采集");
            return true;
        }
    }
    //TL5类型设备
    public class DeviceTL5
    {
        public void Login()
        {
            Console.WriteLine("TL5设备登录");
        }
        public bool Spider()
        {
            Console.WriteLine("TL5设备采集");
            return true;
        }
    }
    //设备采集的服务
    public class DeviceService
    {
        private DeviceMML MML = null;
        private DeviceTL2 TL2 = null;
        private DeviceTELNET TELNET = null;
        private DeviceTL5 TL5 = null;
        private string m_type = null;
        //构造函数里面通过类型来判断是哪种类型的设备
        public DeviceService(string type)
        {
            m_type = type;
            if (type == "0")
            {
                MML = new DeviceMML();
            }
            else if (type == "1")
            {
                TL2 = new DeviceTL2();
            }
            else if (type == "2")
            {
                TELNET = new DeviceTELNET();
            }
            else if (type == "3")
            {
                TL5 = new DeviceTL5();
            }
        }
        public void LoginDevice()
        {
            if (m_type == "0")
            {
                MML.Login();
            }
            else if (m_type == "1")
            {
                TL2.Login();
            }
            else if (m_type == "2")
            {
                TELNET.Login();
            }
            else if (m_type == "3")
            {
                TL5.Login();
            }
        }
        public bool DeviceSpider()
        {
            if (m_type == "0")
            {
                return MML.Spider();
            }
            else if (m_type == "1")
            {
                return TL2.Spider();
            }
            else if (m_type == "2")
            {
                return TELNET.Spider();
            }
            else if (m_type == "3")
            {
                return TL5.Spider();
            }
            else
            {
                return true;
            }
        }
    }
比如我们想启动TL5类型设备的采集，这样调用可以实现：
        static void Main(string[] args)
        {
            var oSpider = new DeviceService("3");
            oSpider.LoginDevice();
            var bRes = oSpider.DeviceSpider();
            Console.ReadKey();
        }
```
花了九年二虎之力，总算是可以实现了。可是又过了段时间，又有新的设备类型呢？是不是又要加班，又要改。这样下去，感觉这就是一个无底洞，再加上时间越久，项目所经历的开发人员越容易发生变化，这个时候再改，那维护的成本堪比开发一个新的项目。并且，随着设备类型的增多，代码里面充斥着大量的if…else，这样的烂代码简直让人无法直视。
**基于这种情况，如果我们当初设计这个系统的时候考虑了依赖倒置，那么效果可能截然不同。我们来看看依赖倒置如何解决以上问题的呢？**

C#
```
//定义一个统一接口用于依赖
    public interface IDevice
    {
        void Login();
        bool Spider();
    }
    //MML类型的设备
    public class DeviceMML : IDevice
    {
        public void Login()
        {
            Console.WriteLine("MML设备登录");
        }
        public bool Spider()
        {
            Console.WriteLine("MML设备采集");
            return true;
        }
    }
    //TL2类型设备
    public class DeviceTL2 : IDevice
    {
        public void Login()
        {
            Console.WriteLine("TL2设备登录");
        }
        public bool Spider()
        {
            Console.WriteLine("TL2设备采集");
            return true;
        }
    }
    //TELNET类型设备
    public class DeviceTELNET : IDevice
    {
        public void Login()
        {
            Console.WriteLine("TELNET设备登录");
        }
        public bool Spider()
        {
            Console.WriteLine("TELNET设备采集");
            return true;
        }
    }
    //TL5类型设备
    public class DeviceTL5 : IDevice
    {
        public void Login()
        {
            Console.WriteLine("TL5设备登录");
        }
        public bool Spider()
        {
            Console.WriteLine("TL5设备采集");
            return true;
        }
    }
    //设备采集的服务
    public class DeviceService
    {
        private IDevice m_device;
        public DeviceService(IDevice oDevice)
        {
            m_device = oDevice;
        }
        public void LoginDevice()
        {
            m_device.Login();
        }
        public bool DeviceSpider()
        {
            return m_device.Spider();
        }
    }
```
调用

C#
```
static void Main(string[] args)
  {
      var oSpider = new DeviceService(new DeviceTL5());
      oSpider.Login();
      var bRes = oSpider.Spider();
      Console.ReadKey();
  }
```
代码说明：**上述解决方案中，我们定义了一个IDevice接口，用于上层服务的依赖，也就是说，上层服务（这里指DeviceService）仅仅依赖IDevice接口，对于具体的实现类我们是不管的，只要接口的行为不发生变化，增加新的设备类型后，上层服务不用做任何的修改。这样设计降低了层与层之间的耦合，能很好地适应需求的变化，大大提高了代码的可维护性。呵呵，看着是不是有点眼熟？是不是有点像某个设计模式？其实设计模式的设计原理正是基于此。**
# 三、使用Unity实现依赖倒置
上面说了那么多，都是在讲依赖倒置的好处，那么在我们的项目中究竟如何具体实现和使用呢？
在介绍依赖倒置具体如何使用之前，我们需要引入IOC容器相关的概念，我们先来看看它们之间的关系。
**依赖倒置原则（DIP）：**一种软件架构设计的原则（抽象概念）。
**控制反转（IoC）：**一种反转流、依赖和接口的方式（DIP的具体实现方式）。这是一个有点不太好理解和解释的概念，通俗地说，就是应用程序本身不负责依赖对象的创建和维护，而是将它交给一个外部容器（比如Unity）来负责，这样控制权就由应用程序转移到了外部IoC 容器，即控制权实现了所谓的反转。例如在类型A中需要使用类型B的实例，而B 实例的创建并不由A 来负责，而是通过外部容器来创建。
**依赖注入（DI）：**IoC的一种实现方式，用来反转依赖（IoC的具体实现方式）。园子里面很多博文里面说IOC也叫DI，其实根据博主的理解，DI应该是IOC的具体实现方式，比如我们如何实现控制反转，答案就是通过依赖注入去实现。
**IoC容器：**依赖注入的**框架**，用来映射依赖，管理对象创建和生存周期（DI框架），自动创建、维护依赖对象。
这些名词是不是有点熟呢？博主之前介绍过MEF，之前使用MEF做过依赖注入，详见[MEF实现设计上的“松耦合”（1）](http://blog.jobbole.com/99221/)。其实严格来讲，MEF不能算一种正式的IOC容器，因为它的主要作用还是用于应用程序扩展，避免生成脆弱的硬依赖项，而不是依赖注入。根据博主的了解以及使用经历，常用的IOC容器有：
- **Spring.NET**： [http://www.springframework.net/](http://www.springframework.net/)
- **Unity**：  [http://unity.codeplex.com/](http://unity.codeplex.com/)
- **Autofac**:  [http://code.google.com/p/autofac/](http://code.google.com/p/autofac/)
- **Ninject**:  [http://www.ninject.org/](http://www.ninject.org/)
当然，还有其他的IOC容器这里就不一一列举。Spring.net是从Java的Spring框架移植过来的，功能之强大我们就不多说了，可是自从它宣布不再更新，博主在使用它的时候就非常慎重了。下面博主还是就Unity这种IOC容器来看看依赖倒置的具体实现。
## 1、Unity引入
Unity如何引入？我们神奇的Nuget又派上用场了。最新的Unity版本已经到了4.0.1。
![](http://jbcdn2.b0.upaiyun.com/2016/04/83a8bd2191b48956fbfd0180dfef4253.png)
安装成功后主要引入了三个dll。
![](http://jbcdn2.b0.upaiyun.com/2016/04/52acaad6b5cda027e17085e490385e35.png)
## 2、Unity常用API

C#
```
UnityContainer.RegisterType();
UnityContainer.RegisterType();
UnityContainer.RegisterType("keyName");
IEnumerable databases = UnityContainer.ResolveAll();
IT instance = UnityContainer.Resolve();
T instance = UnityContainer.Resolve("keyName");
UnitContainer.RegisterInstance("keyName",new T());
UnityContainer.BuildUp(existingInstance);
IUnityContainer childContainer1 = parentContainer.CreateChildContainer();
```
## 3、代码注入方式示例
### 3.1、默认注册方式
仍然以上面的场景为例说明，我们注入DeviceMML这个实现类。

C#
```
class Program
    {
        private static IUnityContainer container = null;
        static void Main(string[] args)
        {
            RegisterContainer();
            var oSpider = container.Resolve();
            oSpider.Login();
            var bRes = oSpider.Spider();
            Console.ReadKey();
        }
        /// 
        /// 代码注入
        /// 
        public static void RegisterContainer()
        {
            container = new UnityContainer();
            container.RegisterType();  //默认注册方式,如果后面再次默认注册会覆盖前面的
        }
    }
```
运行结果
![](http://jbcdn2.b0.upaiyun.com/2016/04/a0a8d4223b754c32db07b4b57d029938.png)
### 3.2、带命名方式的注册
上面默认注入的方式中，我们只能注入一种具体的实例，如果我们需要同时注入多个类型的实例呢？看看我们的 RegisterType() 方法有多个重载。

C#
```
class Program
    {
        private static IUnityContainer container = null;
        static void Main(string[] args)
        {
            RegisterContainer();
            var oSpider = container.Resolve("TL5");
            oSpider.Login();
            var bRes = oSpider.Spider();
            Console.ReadKey();
        }
        /// 
        /// 代码注入
        /// 
        public static void RegisterContainer()
        {
            container = new UnityContainer();
           container.RegisterType("MML");  //默认注册（无命名）,如果后面还有默认注册会覆盖前面的
            container.RegisterType("Telnet");  //命名注册
            container.RegisterType("TL2");  //命名注册
            container.RegisterType("TL5");  //命名注册
        }
    }
```
运行结果
![](http://jbcdn2.b0.upaiyun.com/2016/04/02440c97fbadacf14a85a327d6b3a9eb.png)
## 4、配置文件注入方式示例
在App.config或者Web.config里面加入如下配置：


```
<?xml version="1.0" encoding="utf-8" ?>
<configuration>
   <configSections>
    <section name="unity" type="Microsoft.Practices.Unity.Configuration.UnityConfigurationSection,Microsoft.Practices.Unity.Configuration"/>
  </configSections>
  <unity>
    <!--容器-->
    <containers>
      <container name="Spider">
        <!--映射关系-->
        <register type="ESTM.Spider.IDevice,ESTM.Spider"  mapTo="ESTM.Spider.DeviceMML,ESTM.Spider" name="MML"></register>
        <register type="ESTM.Spider.IDevice,ESTM.Spider"  mapTo="ESTM.Spider.DeviceTELNET,ESTM.Spider" name="TELNET"></register>
        <register type="ESTM.Spider.IDevice,ESTM.Spider"  mapTo="ESTM.Spider.DeviceTL2,ESTM.Spider" name="TL2"></register>
        <register type="ESTM.Spider.IDevice,ESTM.Spider"  mapTo="ESTM.Spider.DeviceTL5,ESTM.Spider" name="TL5"></register>
      </container>
    </containers>
  </unity>
</configuration>
```
在代码里面注册配置文件：

C#
```
namespace ESTM.Spider
{
    class Program
    {
        private static IUnityContainer container = null;
        static void Main(string[] args)
        {
            ContainerConfiguration();
            var oSpider = container.Resolve("TL5");
            oSpider.Login();
            var bRes = oSpider.Spider();
            Console.ReadKey();
        }
        /// 
        /// 配置文件注入
        /// 
        public static void ContainerConfiguration()
        {
            container = new UnityContainer();
            UnityConfigurationSection configuration = (UnityConfigurationSection)ConfigurationManager.GetSection(UnityConfigurationSection.SectionName);
            configuration.Configure(container, "Spider");
        }
    }
}
```
运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/04/4a481227ba070491ff55cd46d2bea697.png)
**代码说明**
**（1）**

C#
```
register type="ESTM.Spider.IDevice,ESTM.Spider"  mapTo="ESTM.Spider.DeviceMML,ESTM.Spider" name="MML">register>
```
**节点里面，type对象抽象，mapTo对象具体实例对象，name对象实例的别名。**
**（2）在app.config里面可以配置多个 container name=”Spider”> 节点，不同的name配置不同的依赖对象。**
**（3）配置文件注入的灵活之处在于解耦。为什么这么说呢？试想，如果我们的IDevice接口对应着一个接口层，而DeviceMML、DeviceTELNET、DeviceTL2、DeviceTL5等实现类在另外一个实现层里面，我们的UI层（这里对应控制台程序这一层）只需要添加IDevice接口层的引用，不必添加实现层的引用，通过配置文件注入，在运行的时候动态将实现类注入到UI层里面来。这样UI层就对实现层实现了解耦，实现层里面的具体逻辑变化时，UI层里面不必做任何更改。**
# **四、总结**
到此，依赖倒置原则的讲解基本结束了。根据博主的理解，设计模式的这些原则是设计模式的理论指导，而设计模式则是这些理论的具体运用。说一千道一万，要想搞懂设计模式，必须先了解设计模式遵循的原则，无论是哪种设计模式都会遵循一种或者多种原则。当然文章可能有理解不当的地方，欢迎大牛们指出。
软件设计原则系列文章
- [《小话设计模式原则之（2）：单一职责原则SRP》](http://blog.jobbole.com/99587/)
- [《小话设计模式原则之（3）：接口隔离原则ISP》](http://blog.jobbole.com/99616/)
- [《小话设计模式原则之（4）：开闭原则OCP》](http://blog.jobbole.com/99617/)
