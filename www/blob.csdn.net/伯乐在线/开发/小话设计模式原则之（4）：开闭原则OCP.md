# 小话设计模式原则之（4）：开闭原则OCP - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/5272144.html)
# 一、原理介绍
## 1、官方定义
开闭原则，英文缩写**OCP**，全称Open Closed Principle。
原始定义：Software entities (classes, modules, functions) should be open for extension but closed for modification。
字面翻译：软件实体（包括类、模块、功能等）应该对扩展开放，但是对修改关闭。
## 2、自己理解
### 2.1、原理解释
- 对扩展开放。模块对扩展开放，就意味着需求变化时，可以对模块扩展，使其具有满足那些改变的新行为。换句话说，模块通过扩展的方式去应对需求的变化。
- 对修改关闭。模块对修改关闭，表示当需求变化时，关闭对模块源代码的修改，当然这里的“关闭”应该是尽可能不修改的意思，也就是说，应该尽量在不修改源代码的基础上面扩展组件。
### **2.2、为什么要“开”和“闭”**
一般情况，我们接到需求变更的通知，通常方式可能就是修改模块的源代码，然而修改已经存在的源代码是存在很大风险的，尤其是项目上线运行一段时间后，开发人员发生变化，这种风险可能就更大。所以，为了避免这种风险，在面对需求变更时，我们一般不修改源代码，即所谓的对修改关闭。不允许修改源代码，我们如何应对需求变更呢？答案就是我们下面要说的对扩展开放。
通过扩展去应对需求变化，就要求我们必须要面向接口编程，或者说面向抽象编程。所有参数类型、引用传递的对象必须使用抽象（接口或者抽象类）的方式定义，不能使用实现类的方式定义；通过抽象去界定扩展，比如我们定义了一个接口A的参数，那么我们的扩展只能是接口A的实现类。**总的来说，开闭原则提高系统的可维护性和代码的重用性。**
# 二、场景示例
## 1、对实现类编程，你死得很惨
下面就结合之前博主在园子里面看到的一个使用场景来一步步呈现使用实现类编程的弊端。
场景说明：马上中秋节了， **公司希望研发部门研发一套工具，实现给公司所有员工发送祝福邮件。
接到开发需求，研发部立刻开会成立研发小组，进入紧张的开发阶段，经过1个月的艰苦奋战，系统顺利上线。代码实现如下：
### 1.1 EmailMessage工具类

C#
```
namespace Utility
{
    //发送邮件的类
    public class EmailMessage
    {
        //里面是大量的SMTP发送邮件的逻辑
        //发送邮件的方法
        public void SendMessage(string strMsg)
        {
            Console.WriteLine("Email节日问候：" + strMsg);
        }
    }
}
```
### 1.2 MessageService服务

C#
```
namespace Service
{
    public class MessageService
    {
        private EmailMessage emailHelper = null;
        public MessageService()
        {
            emailHelper = new EmailMessage();
        }
        //节日问候
        public void Greeting(string strMsg)
        {
            emailHelper.SendMessage(strMsg);
        }
    }
}
```
### 1.3 业务调用模块

C#
```
class Program
    {
        static void Main(string[] args)
        {
            Service.MessageService oService = new Service.MessageService();
            oService.Greeting("祝大家中秋节快乐。");
            Console.ReadKey();
        }
    }
```
一切都很顺利，系统也得到公司好评。
日复一日，年复一年，随着时间的推移，公司发现邮件推送的方式也存在一些弊病，比如某些网络不发达地区不能正常地收到邮件，并且在外出差人员有时不能正常收到邮件。这个时候公司领导发现短信推送是较好的解决办法。于是乎，需求变更来了：增加短信推送节日祝福的功能，对于行政部等特殊部门保留邮件发送的方式。
研发部的同事们虽然已有微言，但是没办法，也只有咬着牙忙了，于是代码变成了这样。
### 1.1 工具类里面增加了发送短信的帮助类

C#
```
namespace Utility
{
    //发送邮件的类
    public class EmailMessage
    {
        //里面是大量的SMTP发送邮件的逻辑
        //发送邮件的方法
        public void SendMessage(string strMsg)
        {
            Console.WriteLine("Email节日问候：" + strMsg);
        }
    }
    //发送短信的类
    public class PhoneMessage
    { 
        //手机端发送短信的业务逻辑
        //发送短信的方法
        public void SendMessage(string strMsg)
        {
            Console.WriteLine("短信节日问候：" + strMsg);
        }
    }
}
```
### 1.2 MessageService服务里面增加了一个枚举类型MessageType判断是哪种推送方式

C#
```
namespace Service
{
    public enum MessageType
    { 
        Email,
        Phone
    }
    public class MessageService
    {
        private EmailMessage emailHelper = null;
        private PhoneMessage phoneHelper = null;
        private MessageType m_oType;
        public MessageService(MessageType oType)
        {
            m_oType = oType;
            if (oType == MessageType.Email)
            {
                emailHelper = new EmailMessage();
            }
            else if (oType == MessageType.Phone)
            {
                phoneHelper = new PhoneMessage();
            }
        }
        //节日问候
        public void Greeting(string strMsg)
        {
            if (m_oType == MessageType.Email)
            {
                emailHelper.SendMessage(strMsg);
            }
            else if (m_oType == MessageType.Phone)
            {
                phoneHelper.SendMessage(strMsg);
            }
        }
    }
}
```
### 1.3 业务调用模块

C#
```
class Program
    {
        static void Main(string[] args)
        {
            Service.MessageService oEmaliService = new Service.MessageService(Service.MessageType.Email);
            oEmaliService.Greeting("祝大家中秋节快乐。");
            Service.MessageService oPhoneService = new Service.MessageService(Service.MessageType.Phone);
            oPhoneService.Greeting("祝大家中秋节快乐。");
            Console.ReadKey();
        }
    }
```
经过一段时间的加班、赶进度。终于大功告成。
随着公司的不断发展，很多产品、平台都融入了微信的功能，于是乎公司领导又希望在保证原有功能的基础上增加微信的推送方式。这个时候研发部的同事们就怨声载道了，这样一年改一次，何时是个头？并且随着时间的推移，研发部员工可能发生过多次变换，现在维护这个系统的员工早已不是当初的开发者，在别人的代码上面改功能，做过开发的应该都知道，简直苦不堪言，因为你不知道别人哪里会给你埋一个“坑”。并且在现有代码上面改，也存在很大的风险，即使做好之后所有的功能都必须重新经过严格的测试。
事情发展到这里，就可以看出使用实现类去编程，你会因为需求变更而死得很惨，这个时候我们就能看出遵守开闭原则的重要性了，如果这个系统设计之初就能考虑这个原则，所有的可变变量使用抽象去定义，可能效果截然不同。
## 2、对抽象编程，就是这么灵活
如果项目设计之初我们定义一个ISendable接口，我们看看效果怎样呢？
### 2.1 工具类

C#
```
namespace IHelper
{
    public interface ISendable
    {
        void SendMessage(string strMsg);
    }
}
namespace Utility
{//发送邮件的类
    public class EmailMessage:ISendable
    {
        //里面是大量的SMTP发送邮件的逻辑
        //发送邮件的方法
        public void SendMessage(string strMsg)
        {
            Console.WriteLine("Email节日问候：" + strMsg);
        }
    }
    //发送短信的类
    public class PhoneMessage:ISendable
    { 
        //手机端发送短信的业务逻辑
        //发送短信的方法
        public void SendMessage(string strMsg)
        {
            Console.WriteLine("短信节日问候：" + strMsg);
        }
    }
    //发送微信的类
    public class WeChatMessage:ISendable
    {
        //微信消息推送业务逻辑
        //发送微信消息的方法
        public void SendMessage(string strMsg)
        {
            Console.WriteLine("短信节日问候：" + strMsg);
        }
    }
}
```
### 2.2 MessageService服务

C#
```
namespace Service
{
    public class MessageService
    {
        private ISendable m_oSendHelper = null;
        public MessageService(ISendable oSendHelper)
        {
            m_oSendHelper = oSendHelper;
        }
        //节日问候
        public void Greeting(string strMsg)
        {
            m_oSendHelper.SendMessage(strMsg);
        }
    }
}
```
### 2.3 业务调用模块

C#
```
class Program
    {
        static void Main(string[] args)
        {
            var strMsg = "祝大家中秋节快乐。";
            ISendable oEmailHelper = new EmailMessage();
            Service.MessageService oEmaliService = new Service.MessageService(oEmailHelper);
            oEmaliService.Greeting(strMsg);
            ISendable oPhoneHelper = new PhoneMessage();
            Service.MessageService oPhoneService = new Service.MessageService(oPhoneHelper);
            oPhoneService.Greeting(strMsg);
            ISendable oWeChatHelper = new WeChatMessage();
            Service.MessageService oWeChatService = new Service.MessageService(oWeChatHelper);
            oWeChatService.Greeting(strMsg);
            Console.ReadKey();
        }
    }
```
设计分析：在MessageService服务类中，我们定义了ISendable的接口变量m_oSendHelper，通过这个接口变量，我们就能很方便的通过扩展去应对需求的变化，而不必修改原来的代码。比如，我们现在再增加一种新的推送方式，对于我们的MessageService服务类来说，不用做任何修改，只需要扩展新的推送消息的工具类即可。从需要抽象的角度来说，开闭原则和依赖倒置原则也有一定的相似性，不过博主觉得，开闭原则更加偏向的是使用抽象来避免修改源代码，主张通过扩展去应对需求变更，而依赖倒置更加偏向的是层和层之间的解耦。当然，我们也不必分得那么细，往往，一个好的设计肯定是遵循了多个设计原则的。
上面的设计，很好的解决了MessageService服务类中的问题，但是对于调用方（比如上文中的Main函数里面），很显然是违背了依赖倒置原则的，因为它既依赖接口层ISendable，又依赖接口实现层EmailMessage、PhoneMessage等。这肯定是不合适的。我们引入MEF，稍作修改。

C#
```
namespace Utility
{
    //发送邮件的类
    [Export("Email", typeof(ISendable))]
    public class EmailMessage:ISendable
    {
        //里面是大量的SMTP发送邮件的逻辑
        //发送邮件的方法
        public void SendMessage(string strMsg)
        {
            Console.WriteLine("Email节日问候：" + strMsg);
        }
    }
    //发送短信的类
    [Export("Phone", typeof(ISendable))]
    public class PhoneMessage:ISendable
    { 
        //手机端发送短信的业务逻辑
        //发送短信的方法
        public void SendMessage(string strMsg)
        {
            Console.WriteLine("短信节日问候：" + strMsg);
        }
    }
    //发送微信的类
    [Export("WeChat", typeof(ISendable))]
    public class WeChatMessage:ISendable
    {
        //微信消息推送业务逻辑
        //发送微信消息的方法
        public void SendMessage(string strMsg)
        {
            Console.WriteLine("短信节日问候：" + strMsg);
        }
    }
}
```
Main函数里面

C#
```
class Program
    {
        [Import("Email",typeof(ISendable))]
        public ISendable oEmailHelper { get; set; }
        [Import("Phone", typeof(ISendable))]
        public ISendable oPhoneHelper { get; set; }
        [Import("WeChat", typeof(ISendable))]
        public ISendable oWeChatHelper { get; set; }
        static void Main(string[] args)
        {
            //使用MEF装配组件
            var catalog = new AssemblyCatalog(Assembly.GetExecutingAssembly());
            var container = new CompositionContainer(catalog);
            var oProgram = new Program();
            container.ComposeParts(oProgram);
            var strMsg = "祝大家中秋节快乐。";
            Service.MessageService oEmaliService = new Service.MessageService(oProgram.oEmailHelper);
            oEmaliService.Greeting(strMsg);
            Service.MessageService oPhoneService = new Service.MessageService(oProgram.oPhoneHelper);
            oPhoneService.Greeting(strMsg);
            Service.MessageService oWeChatService = new Service.MessageService(oProgram.oWeChatHelper);
            oWeChatService.Greeting(strMsg);
            Console.ReadKey();
        }
    }
```
如果你使用Unity，直接用配置文件注入的方式更加简单。
# 三、总结
至此开闭原则的示例就基本完了。文中观点有不对的地方，欢迎指出，博主在此多谢了。
软件设计原则系列文章
- [《小话设计模式原则之（1）：依赖倒置原则DIP》](http://blog.jobbole.com/99563/)
- [《小话设计模式原则之（2）：单一职责原则SRP》](http://blog.jobbole.com/99587/)
- [《小话设计模式原则之（3）：接口隔离原则ISP》](http://blog.jobbole.com/99616/)
