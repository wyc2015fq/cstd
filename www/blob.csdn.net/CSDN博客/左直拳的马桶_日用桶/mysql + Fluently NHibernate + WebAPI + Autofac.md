# mysql + Fluently NHibernate + WebAPI + Autofac - 左直拳的马桶_日用桶 - CSDN博客
2016年10月14日 16:27:52[左直拳](https://me.csdn.net/leftfist)阅读数：1161标签：[mysql																[nhibernate																[webapi																[autofac](https://so.csdn.net/so/search/s.do?q=autofac&t=blog)](https://so.csdn.net/so/search/s.do?q=webapi&t=blog)](https://so.csdn.net/so/search/s.do?q=nhibernate&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[Web																[.NET																[mySql																[nhibernate](https://blog.csdn.net/leftfist/article/category/6387144)](https://blog.csdn.net/leftfist/article/category/2557425)](https://blog.csdn.net/leftfist/article/category/94497)](https://blog.csdn.net/leftfist/article/category/674421)
MySQL、Fluently NHibernate、WebAPI、Autofac，对我来说每一个都是麻烦疙瘩，现在它们为了一个共同的项目而凑合到一起了。一路磕磕碰碰，现在貌似有了一点眉目。
作为一个步入老人痴呆帕金森阶段的老革命，我当然要马上将奋斗过程记录下来：
**1、MySql + Fluently NHibernate**
```
static ISessionFactory sessionFactory;
public static ISession OpenSession(string connString, string[] assemblys)
{
    if (sessionFactory == null)
    {
        sessionFactory = Fluently.Configure()
            .Database(FluentNHibernate.Cfg.Db.MySQLConfiguration.Standard.
                ConnectionString(connString))
            .Mappings(m =>
            {
                foreach (var item in assemblys)
                {
                    m.FluentMappings.AddFromAssembly(Assembly.Load(item));
                }
            }).BuildSessionFactory();
    }
    return sessionFactory.OpenSession();
}
OpenSession((connString: "server=192.168.0.211; user id=root; password=lt1234; database=pnavrds", assemblys: new string[] { "Pnavrds.Data" });
```
.NET和NHibernate并不天然支持mysql，所以要在项目添加对mysql.data.dll的引用。mysql.data.dll在mysql的安装目录里有。 
比如在 C:\Program Files (x86)\MySQL\Connector.NET 6.9\Assemblies\v4.5
**2、WebAPI**
有关路由问题。 
别看api与MVC很像，但是，MVC支持Area，而api并不。 
但是开始时我并不知道。轻车熟路地加了个Area，一访问，直接404。 
路由如下：
```
public override void RegisterArea(AreaRegistrationContext context) 
{
    context.MapRoute(
        "Test_default",
        "Test/{controller}/{id}",
        new { id = UrlParameter.Optional }
    );
}
```
咋办呢？难道各种控制器济济一堂一锅粥？后来网上查了[资料](https://forums.asp.net/t/2004875.aspx?WebApi%20routing%20and%20areas%20why%20it%20works%20)，添加了一个路由，改为：
```
public override void RegisterArea(AreaRegistrationContext context) 
{
    context.Routes.MapHttpRoute(
        "Test_defaultAPI",
        "api/Test/{controller}/{id}",
        new { id = RouteParameter.Optional }
    );
    context.MapRoute(
        "Test_default",
        "Test/{controller}/{action}/{id}",
        new { action = "Index", id = UrlParameter.Optional }
    );
}
```
注意，这样处理之后，同一个控制器，就有两个地址都可以访问。一个有区域，一个没有区域：
```
http://localhost/Pnavrds.API/api/Test/Dev3/10
http://localhost/Pnavrds.API/api/Dev3/10
```
因为asp.net webapi并不支持区域，不管你这个控制器放在哪个文件夹、哪个命名空间下，它都顽强地解释到根目录下。我们上面做的努力，仅仅是多了一个含有区域名称的地址而已。
[参考资料](https://forums.asp.net/t/2004875.aspx?WebApi%20routing%20and%20areas%20why%20it%20works%20)
**3、Autofac**
这个东东是个好东东。我现在都有点离不开它了。不然那么多实例需要构造，然后每个构造函数都N多参数，太麻烦。但是，因为了解不够，每次用它，好像都要费一些周折，并且很难调试。 
这次也不例外。
1）提示System.Web.Http的版本不对。 
引用的system.web.http.dll版本为5.2.3.0，但系统说跟5.2.0.0对应不上，编译时虽然可以通过，但有警告，建议在app.config里写些啥啥啥。我找遍了代码，都看不到哪里声明了5.2.0.0。
后来还是根据编译器的提示，将它给出的代码，加到web.config里，编译警告就没有了，运行就再无这个错误：
```xml
<runtime>    
    <assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1">
      <dependentAssembly>
        <assemblyIdentity name="System.Net.Http.Formatting" culture="neutral" publicKeyToken="31bf3856ad364e35" />
        <bindingRedirect oldVersion="0.0.0.0-5.2.3.0" newVersion="5.2.3.0" />
      </dependentAssembly>
    </assemblyBinding>
    <assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1">
      <dependentAssembly>
        <assemblyIdentity name="Iesi.Collections" culture="neutral" publicKeyToken="aa95f207798dfdb4" />
        <bindingRedirect oldVersion="0.0.0.0-4.0.0.0" newVersion="4.0.0.0" />
      </dependentAssembly>
    </assemblyBinding>
    <assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1">
      <dependentAssembly>
        <assemblyIdentity name="Autofac" culture="neutral" publicKeyToken="17863af14b0044da" />
        <bindingRedirect oldVersion="0.0.0.0-3.5.0.0" newVersion="3.5.0.0" />
      </dependentAssembly>
    </assemblyBinding>
    <assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1">
      <dependentAssembly>
        <assemblyIdentity name="System.Web.Http" culture="neutral" publicKeyToken="31bf3856ad364e35" />
        <bindingRedirect oldVersion="0.0.0.0-5.2.3.0" newVersion="5.2.3.0" />
      </dependentAssembly>
    </assemblyBinding>
  </runtime>
```
附上编译信息：
```xml
4>C:\Program Files (x86)\MSBuild\14.0\bin\Microsoft.Common.CurrentVersion.targets(1819,5): warning MSB3247: 发现同一依赖程序集的不同版本间存在冲突。在 Visual Studio 中，请双击此警告(或选择此警告并按 Enter)以修复冲突；否则，请将以下绑定重定向添加到应用程序配置文件中的“runtime”节点: <assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1"><dependentAssembly><assemblyIdentity name="System.Net.Http.Formatting" culture="neutral" publicKeyToken="31bf3856ad364e35" /><bindingRedirect oldVersion="0.0.0.0-5.2.3.0" newVersion="5.2.3.0" /></dependentAssembly></assemblyBinding><assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1"><dependentAssembly><assemblyIdentity name="Iesi.Collections" culture="neutral" publicKeyToken="aa95f207798dfdb4" /><bindingRedirect oldVersion="0.0.0.0-4.0.0.0" newVersion="4.0.0.0" /></dependentAssembly></assemblyBinding><assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1"><dependentAssembly><assemblyIdentity name="Autofac" culture="neutral" publicKeyToken="17863af14b0044da" /><bindingRedirect oldVersion="0.0.0.0-3.5.0.0" newVersion="3.5.0.0" /></dependentAssembly></assemblyBinding>
```
2）说控制器没有默认构造函数 
这说明autofac没有正常运行，否则不会报这个错。构造实例正是autofac的工作。
后来改了autofac的builder内容。代码如下：
```
public class AutofacConfig
{
    public static void BuildContainer()
    {
        var builder = new ContainerBuilder();
        //Infrastructure objects
        builder.RegisterApiControllers(typeof(WebApiApplication).Assembly);
        builder.RegisterAssemblyTypes(typeof(WebApiApplication).Assembly).AsImplementedInterfaces();
        builder.RegisterModule(new AutofacWebTypesModule());
        //其他代码.....
        builder.RegisterModelBinderProvider();
        builder.RegisterFilterProvider();
        IContainer container = builder.Build();
        //DependencyResolver.SetResolver(new AutofacDependencyResolver(container));
        GlobalConfiguration.Configuration.DependencyResolver = (new AutofacWebApiDependencyResolver(container));
    }
}
```
