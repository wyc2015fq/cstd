# Membership三步曲之进阶篇 - 文章 - 伯乐在线
原文出处： [腾飞（Jesse)](http://www.cnblogs.com/jesse2013/p/membership-part2.html)
　　Membership 三步曲之进阶篇 – 深入剖析Provider Model
本文的目标是让每一个人都知道Provider Model 是什么，并且能灵活的在自己的项目中使用它。
在入门篇中我们已经从0开始将Membership集成到一个空的MVC站点中，并且与ASP.NET的权限管理体系相结合。本篇(进阶篇)将剖析Membership的设计理念以及它的结构。别忘了我们还有高级篇我们将会扩展自己的MembershipProvider和RolesProvider，目地是直接利用我们老系统中已经存在的用户表和角色表，也就是用我们已经存在的数据库去集成Membership。
我们了解到Membership的重要组成部分是MembershipProviders。 要理解Membership的架构设计，我们首先要理解Provider。Provider 的全称是 Provider Model （中文是提供程序模型），它早就已经不是什么新鲜事了，它是在ASP.NET 1.1的时候被 [**Rob Howard **](http://grokable.com/)设计出来的，从ASP.NET 2.0时代，它就已经开始大行其道了。到现在的MVC中，各种Provider已经随处可见了, 貌似微软对它特别青睐。下面我们就来好好看一看Provider到底是个神马东西，Membership是如何利用它的！
# 目录
- [随处可见的Provider](#everywhere-is-provider)
- [Provider并不是一种设计模式](#provider-is-not-a-designpattern)- [策略模式](#strategy)
- [工厂方法](#factory-method)
- [单件模式](#singleton)
- [外观模式](#facade)
- [Provider Model 的配置及初始化框架](#configuration-and-initialization)
- [利用Provider Model实现记日志组件](#demo-of-log)
- [参考引用](#reference)
# 随处可见的Provider
各位Provider们，先来和大家混个眼熟吧，相信下面的几种Provider大家并不会觉得陌生：
- [Membership Providers](http://msdn.microsoft.com/en-us/library/aa479021.aspx)
- [Role Providers](http://msdn.microsoft.com/en-us/library/aa479022.aspx)
- [Session State Providers](http://msdn.microsoft.com/en-us/library/aa479024.aspx)
- [Profile Providers](http://msdn.microsoft.com/en-us/library/aa479025.aspx)
前面两种Provider我们在[Membership入门篇](http://www.cnblogs.com/jesse2013/p/membership.html)里面已经接触到了，后面两种我想也不用说了吧？ 而在MVC中的下面几个兄弟，你们都见识过吧？
- [FilterProviders](http://msdn.microsoft.com/en-us/library/system.web.mvc.filterproviders(v=vs.118).aspx)
- [ModelBinderProviders](http://msdn.microsoft.com/en-us/library/system.web.mvc.modelbinderproviders(v=vs.118).aspx)
- [ModelValidatorProviders](http://msdn.microsoft.com/en-us/library/system.web.mvc.modelvalidatorproviders(v=vs.118).aspx)
- [ModelMetadataProviders](http://msdn.microsoft.com/en-us/library/system.web.mvc.modelmetadataproviders(v=vs.118).aspx)
那么Provider到底是起了一个什么样的作用呢？为什么微软要如此广泛的使用它？又或者我们是不是可以在自己的项目中使用这种设计呢？
首先，一个Provider可以是一个类，或者好几个类共同组成的一个模块，它们提供了一些特定的功能，这些特定的功能要么用接口中的方法或者抽象类的抽象方法暴露出来(.NET框架中一般使用抽象类)。这就是ProvderModel的第一要素：**具要良好定义的公有API**。比如说我们的MembershipProvider提供了GetUser, CreateUser, ChangePassword等与用户管理息息相关的功能，这些API并不是越全越好，而是越精越好，只定义最核心的功能，其它的可以添加自己的扩展去实现，正是小巧又精悍呀。
其次，**要有一种配置机制。**这种配置机制能够将具体的Provider与我们定义的功能集绑定起来。比如说我们有SqlMembershipProvider，还有ActiveDirectoryMemberProvider，他们都是抽象类MembershipProvider的子类。我还可以自己去添加其它的子类以不同的方式实现同样的功能，那么我就需要一种灵活的方法将我的子类加入到程序中。ASP.NET是通过web.config里面的配置结点实现的。
![](http://jbcdn2.b0.upaiyun.com/2016/05/b1c9a24590c9622272c6bc2a19c0cfa4.png)
最后，我们还需要有一种加载机制，通过这种加载机制(我们下面会探讨这种加载机制)，我们可以读取配置并创建Provider的具体实例传递给我们的功能API。.NET已经为我们提供好了一些类来帮助我们达到简单配置和初始化Providers的目地，我们马上就会说到。
那么Provider model能给我们带来哪些好处呢？
- 提高灵活性和可扩展性
- 隔绝功能代码和具体的数据访问代码
- 让扩展变得简单。既可以从抽象类继承，也可以从其它具体Provider继承，只实现不一样的地方，最后只需要轻松配置就可以搞定。
- ？
拿Membership来举例，核心功能通过Membership调用完成。而Membership并不负责具体的实现，它定义了公开良好的接口在MembershipProvider中。 所以Membership实际上并不处理业务代码，大量的功能是调用具体的Provider来实现的。这就起到了隔绝的作用，用户信息可以保存在SQL数据库中，还可以保存在域控制器中，这些Membership不用关心，只需要交给具体的Provider就可以了。同理，如果我们想让用户保存到其它地方，只需要添加自己的Provider来实现就可以了。
![](http://jbcdn2.b0.upaiyun.com/2016/05/fc41cf680ec8d8ef401b462737310998.png)
*Membership.GetUser的代码*

```
public static MembershipUser GetUser(string username, bool userIsOnline)
{
    SecUtility.CheckParameter(ref username, true, false, true, 0, "username");
    return Provider.GetUser(username, userIsOnline);
}
```
所以最后怎么样去实现，实现上还是看我们具体配置的Provider如何去做。
# Provider并不是一种设计模式
Provider在最开始(ASP.NET 1.1)的时候其实是一种设计模式，全称是 [Provider Model Design Pattern](http://msdn.microsoft.com/en-us/library/ms972319.aspx)，但是到了ASP.NET2.0 的时候其实就直接叫[Provider Model](http://msdn.microsoft.com/en-us/library/aa479020.aspx) 了(具体叫不叫设计模式，是不明确的，还有的地方叫Provider Pattern，在这里我们就不做争辩了)。 但是实际上我们会在Provider Model身上发现好几种设计模式的影子，比如说：
- 策略模式
- 工厂方法模式
- 单一模式
- 外观模式
本文不会详细讨论这四种设计模式，但是为了让大家更好的理解Provider Model，我们来简单的结合Provider Model的结构提一下每个设计模式的定义。
## 2.1 策略模式
关于策略模式有一个故事，话说老王有时候开车特别快，开的快有时候就会被警察逮住。但是有时候遇到的是比较好说话的警察可能不会扣他的分，只是口头警告一下，有时候会遇到比较严格又正直的警察一定要扣分。但是在没有被逮住之前，我们都不知道那个会是一个好说话的警察还是一个严格又正直的警察。
![](http://jbcdn2.b0.upaiyun.com/2016/05/ef71452a0a63b57aed0d0168c530db12.png)
有没有发现这个类图和我们上面的Membership的类图很像？ 简单一句话概括策略模式：将一个特性的主要功能抽象出来，允许不同的实现方式，在运行时可以任意切换到不同的实现。MembershipProvider抽象类为我们定义了一组具体的API， 而Membership类则负责调用这些API，但是Membership并不在乎现在是哪一个实现。
## 2.2 工厂方法模式
[工厂方法](http://terrylee.cnblogs.com/archive/2006/01/04/310716.html)（如果想详细了解工厂方法的同学，可以参考Terry Lee的这篇博客）的关键在于: **使用者不需要知道也不用知道哪一个具体对象会被创建。**对于Membership类来说，它会调用具体的Provider相关的方法而不用知道当前是哪一个Provider在工作。 在Membership内部有一个名叫Provider的属性，它的类型是基类- 抽象类MembershipProvider。而它的初始化是借助于.NET为我们提供的一套配置和初始化的基础框架来实现的，这两套框架我们马上就会说到了。
## 2.3 单件模式
单件模式简单的说即在整个应用程序的生命周期内，某个类的实例至始至终都是同一个。在Membership的内部，维护着一个Provider的属性，这就是singleton的实现。
## 2.4 外观模式
对外观模式浅显易懂的定义是“为一组分布在不同的子系统或者不同地方的接口提供一个统一的入口点”。大家可以对比一下，所有的和Membership相关的功能都是通过调用Membership完成的，即使里面有一些功能是由MembershipUser和其它的类来实现，但是没有关系，Membership给这些所有的功能作了一个统一。这就是传说中的外观模式。
![](http://jbcdn2.b0.upaiyun.com/2016/05/80bda2c5bbb72305503706adc365a625.png)
# Provider Model的配置及初始化框架
Provider Model之所以能在.NET的世界里面大放光彩，不仅仅是因为它提供了我们上述的功能及特性，而是它还提供了这样的一套配置及初始化框架，从而使得我们在.NET中使用Provider Model中使用Provider Model是那么的容易。下面我们就来看看它是如何做到的。
![](http://jbcdn2.b0.upaiyun.com/2016/05/fbba5be04887be8f58153b84ca8c9bd4.png)
我们在上一篇中列举了Membership中使用到的一些类型，但是用到起到读取配置作用的实际上是MembershipSection这个类，它会帮助我们把配置文件中membership结点信息加载到一个MembershipSection的实例中。我们可以来看看Membership中的Initialize方法：

```
private static MembershipProvider s_Provider;
private static MembershipProviderCollection s_Providers;
private static void Initialize()
{
    MembershipSection membershipSection = null;
    if (!_initialized)
    {
        // 这里是加载配置信息
        membershipSection = (MembershipSection)ConfigurationManager.GetSection("membership");
        if (membershipSection == null)
            throw new Exception("没有找到Membership的配置");
        s_Providers = new MembershipProviderCollection();
        // 这里是创建Provider实例
        ProvidersHelper.InstantiateProviders(
            membershipSection.Providers, 
            s_Providers, 
            typeof(MembershipProvider));
        
        // 将当前Provider指向默认的Provider
        s_Provider = s_Providers[membershipSection.DefaultProvider];
        _initialized = true;
    }
}
public static MembershipProvider Provider
{
    get
    {
        Initialize();
        return s_Provider;
    }
}
```
而初始化是由System.Web.Configuration.ProvidersHelper来完成的，它里面只包含两个公有的静态方法: InstantiateProviders和InstantiateProvider。大家可以看到前者只不过是一个包装方法，其实是通过遍历调用后者来加载所有的provider实例的。后者的代码我就不贴了，其实就是用了反射去创建对象而已，还记得我们已经将具体类型配置到config文件去了么？

```
public static void InstantiateProviders(
    ProviderSettingsCollection configProviders, 
    ProviderCollection providers, 
    Type providerType)
{
    foreach (ProviderSettings settings in configProviders)
    {
        providers.Add(InstantiateProvider(settings, providerType));
    }
}
```
有了这一套配置和初始化框架，才有了我们在config文件中的简单操作。
# 利用Provider Model实现记日志组件
记日志组件也已经是被炒了很多次的话题，既然都炒了那么多次了，也不在乎我再炒一次吧？ 其实.NET本身提供的一些事件记录器也是基于Provider Model来实现的，既然我们学习了Provider Model，那就来自己动手实现一个简单的日志组件小小实战一下吧。
我们要实现的功能很简单，只有能够写入消息到一个文件就可以了，但是我们要求可以按不同的格式写入，可以是纯文本的，可以是XML的，当然你还可以扩展成任意其它你想要的格式。 对于调用者来说，它只需要轻松调用就可以了。并且我们要实现可以在config文件中灵活配置使用哪一种格式来记录我们的日志。
![](http://jbcdn2.b0.upaiyun.com/2016/05/28ed8422f5bdef8daf25cc179656c39c.png)

```
public ActionResult Index()
{
    MyLog.LogManager.Log(MyLog.LogType.Info, "我要开始记日志了");
    return View();
}
```
我们的LogManager是对外暴露的唯一对象，里面的方法和属性都是静态的，和Membership类一样，它不负责具体的实现，所有的实现都是转交给具体的Provider的。
![](http://jbcdn2.b0.upaiyun.com/2016/05/812f8d19df2b044ac2e748c95db23e8a.png)
*　LogManager.Log的代码*

```
public static void Log(LogType logType, string Message)
{
    Provider.WriteLog(logType, Message);
}
public static LogProvider Provider
{
    get
    {
        Initialize();
        return _provider;
    }
}
```
*初始化Provider的代码*

```
private static LogProvider _provider;
private static LogProviderCollection _providers;
// 是否已经初始化
private static bool _initialized = false;
private static void Initialize()
{
    LogProviderConfigurationSection myLogSection = null;
    if (!_initialized)
    {
        // 读取myLog结点的配置
        myLogSection = (LogProviderConfigurationSection)ConfigurationManager.GetSection("myLog");
        if (myLogSection == null)
            throw new Exception("没有找到myLog的配置");
        
        // 初始化providers
        _providers = new LogProviderCollection();
        ProvidersHelper.InstantiateProviders(myLogSection.Providers, _providers, typeof(LogProvider));
        _provider = _providers[myLogSection.DefaultProvider];
        _initialized = true;
    }
}
```
*　　TextLogProvider.WriteLog的代码*

```
public override void WriteLog(LogType logType, string message)
{
    var dir = Path.GetDirectoryName(FilePath);
    if (!Directory.Exists(dir))
        Directory.CreateDirectory(dir);
    using (var sw = new StreamWriter(FilePath, true))
    {
        string s = string.Format("{0}, {1}, {2}", DateTime.Now, logType.ToString(), message);
        sw.WriteLine(s);
    }
}
```
XmlLogProvider我就不实现了，如果大家有兴趣可以自己动手尝试一下，还是老样子附上整个项目的源码。Membership三步曲最后一篇，我们将实现自己的MembershipProvider利用EF来完成的用户的管理。下周见！
下载源码：[http://pan.baidu.com/s/1jGI9dhs](http://pan.baidu.com/s/1jGI9dhs) （觉得有用的同学就帮助点个推荐吧，您的满意，我的动力！）
# 参考引用
- [http://msdn.microsoft.com/en-us/library/aa479046.aspx](http://msdn.microsoft.com/en-us/library/aa479046.aspx)
- [http://msdn.microsoft.com/en-us/library/ms972319.aspx](http://msdn.microsoft.com/en-us/library/ms972319.aspx)
- [http://msdn.microsoft.com/en-us/library/aa479020.aspx](http://msdn.microsoft.com/en-us/library/aa479020.aspx)
