# ASP.NET Core默认提供的这个跨平台的服务器——KestrelServer - 文章 - 伯乐在线
原文出处： [蒋金楠](http://www.cnblogs.com/artech/p/KestrelServer.html)
跨平台是ASP.NET Core一个显著的特性，而KestrelServer是目前微软推出了唯一一个能够真正跨平台的Server。KestrelServer利用一个名为KestrelEngine的网络引擎实现对请求的监听、接收和响应。KetrelServer之所以具有跨平台的特质，源于KestrelEngine是在一个名为libuv的跨平台网络库上开发的。
# 一、libuv
![image](http://jbcdn2.b0.upaiyun.com/2016/06/8eac031c902d695b2da8e9b326da6eb8.png)
说起libuv，就不得不谈谈libev，后者是Unix系统上一个事件循环和事件模型的网络库。libev因其具有的高性能成为了继lievent和Event perl module之后一套最受欢迎的网络库。由于Libev不支持Windows，有人在libev之上创建了一个抽象层以屏蔽平台之间的差异，这个抽象层就是libuv。libuv在Windows平台上是采用IOCP的形式实现的，右图揭示了libuv针对Unix和Windows的跨平台实现原理。到目前为止，libuv支持的平台已经不限于Unix和Windows了，包括Linux（2.6）、MacOS和Solaris （121以及之后的版本）在内的平台在libuv支持范围之内。
# 二、KestrelServer
如下所示的代码片段体现了KestrelServer这个类型的定义。除了实现接口IServer定义的Features属性之外，KestrelServer还具有一个类型为KestrelServerOptions的只读属性Options。这个属性表示对KestrelServer所作的相关设置，我们在调用构造函数时通过输入参数options所代表的IOptions对象对这个属性进行初始化。构造函数还具有另两个额外的参数，它们的类型分别是IApplicationLifetime和ILoggerFactory，后者用于创建记录日志的Logger，前者与应用的生命周期管理有关。

C#
```
public class KestrelServer : IServer
    {   
        public IFeatureCollection       Features { get; }
        public KestrelServerOptions     Options { get; }
     
        public KestrelServer(IOptions<KestrelServerOptions> options,IApplicationLifetime applicationLifetime, ILoggerFactory loggerFactory);
        public void Dispose();
        public void Start<TContext>(IHttpApplication<TContext> application);
    }
```
我们一般通过调用WebHostBuilder的扩展方法UseKestrel方法来完成对KestrelServer的注册。如下面的代码片段所示，UseKestrel方法具有两个重载，其中一个具有同一个类型为Action的参数，我们可以利用这个参数直接完成对KestrelServerOptions的设置。

C#
```
public static class WebHostBuilderKestrelExtensions
    {
       public static IWebHostBuilder UseKestrel(this IWebHostBuilder hostBuilder);
       public static IWebHostBuilder UseKestrel(this IWebHostBuilder hostBuilder, Action<KestrelServerOptions> options);
    }
```
# 三、KestrelServerOptions
由于Server负责请求的监听、接收和响应，所以Server是影响整个Web应用响应能力和吞吐量最大的因素之一，为了更加有效地使用Server，我们往往针对具体的网络负载状况对其作针对性的设置。对于KestrelServer来说，在构造函数中作为参数指定的KestrelServerOptions对象代表针对它所做的设置。我们针对KestrelServer所做的设置主要体现在KestrelServerOptions类型的如下5个属性上。

C#
```
public class KestrelServerOptions
   {   
        //省略其他成员
       public int          MaxPooledHeaders { get; set; }
       public int          MaxPooledStreams { get; set; }
       public bool         NoDelay { get; set; }
       public TimeSpan     ShutdownTimeout { get; set; }
       public int          ThreadCount { get; set; }
   }
```
KestrelServerOptions注册的KetrelServer在管道中会以依赖注入的方式被创建，并采用构造器注入的方式提供其构造函数的参数options，由于这个参数类型为IOptions，所以我们利用Options模型以配置的方式来指定KestrelServerOptions对象承载的设置。比如我们可以将KestrelServer的相关配置定义在如下一个JSON文件中。

C#
```
{ 
       "noDelay"         : false, 
       "shutdownTimeout" : "00:00:10", 
       "threadCount"     : 10 
   }
```
为了让应用加载这么一个配置文件（文件名假设为“KestrelServerOptions.json”），我们只需要在启动类型（Startup）类的ConfigureServces方法中按照如下的方式利用ConfigurationBuilder加载这个配置文件并生成相应的Configuration对象，最后按照Options模型的编程方式完成KestrelServerOptions类型和该对象的映射即可。

C#
```
public class Startup
    {
        //其他成员
        public void ConfigureServices(IServiceCollection services)
        {
            IConfiguration configuration = new ConfigurationBuilder()
                .AddJsonFile("KestrelServerOptions.json")
                .Build();
            services.Configure<KestrelServerOptions>(configuration);
       }
   }
```
# 四、ApplicationLifetime
我们将所有实现了IApplicationLifetime接口的所有类型及其对应对象统称为ApplicationLifetime。从命名的角度来看，ApplicationLifetime貌似是对当前应用生命周期的描述，而实际上它存在的目的仅仅是在应用启动和关闭（只要是关闭）时对相关组件发送通知而已。
如下面的代码片段所示，IApplicationLifetime接口具有三个CancellationToken类型的属性（ApplicationStarted、ApplicationStopping和ApplicationStopped），我们可以利用它们是否已经被取消（Cancel）确定当前应用的状态（已经开启、正在关闭和已经关闭）。如果试图关闭应用，StopApplication方法应该被调用以发出应用正在被关闭的通知。对于KestrelServer来说，如果请求处理线程中发生未被处理异常，它会调用这个方法。

C#
```
public interface IApplicationLifetime
    {
        CancellationToken ApplicationStarted { get; }
        CancellationToken ApplicationStopping { get; }
        CancellationToken ApplicationStopped { get; }
     
        void StopApplication();
    }
```
ASP.NET Core默认使用的ApplicationLifetime是具有如下定义的一个同名类型。可以看出它实现的三个属性返回的CancellationToken对象是通过三个对应的CancellationTokenSource生成。除了实现IApplicationLifetime接口的StopApplication方法用于发送“正在关闭”通知之外，这个类型还定义了额外两个方法（NotifyStarted和NotifyStopped）用于发送“已经开启/关闭”的通知。

C#
```
public class ApplicationLifetime : IApplicationLifetime
    {
        private readonly CancellationTokenSource _startedSource = new CancellationTokenSource();
        private readonly CancellationTokenSource _stoppedSource = new CancellationTokenSource();
        private readonly CancellationTokenSource _stoppingSource = new CancellationTokenSource();    
     
        public CancellationToken ApplicationStarted
        {
            get { return this._startedSource.Token; }
       }
       public CancellationToken ApplicationStopped
       {
           get { return this._stoppedSource.Token; }
       }
       public CancellationToken ApplicationStopping
       {
           get { return this._stoppingSource.Token; }
   }
    
       public void NotifyStarted()
       {
           this._startedSource.Cancel(false);
       }
       public void NotifyStopped()
       {
           this._stoppedSource.Cancel(false);
       }
       public void StopApplication()
       {
           this._stoppingSource.Cancel(false);
       }
   }
```
一个ASP.NET Core应用利用管道处理请求，所以管道的生命周期等同于应用自身的生命周期。当我们调用Run方法开启WebHost时，请求处理管道被构建出来。如果管道在处理请求时发生未被处理的异常，管道的Sever会调用ApplicationLifeTime对象的StopApplication方法向WebHost发送关闭应用的通知以便后者执行一些回收释放工作。
# 五、设置监听地址
在演示的实例中，我们实际上并不曾为注册的KestrelServer指定一个监听地址，从运行的效果我们不难看出，WebHost在这种情况下会指定“http://localhost:5000”为默认的监听地址，Server的监听地址自然可以显式指定。在介绍如何通过编程的方式为Server指定监听地址之前，我们有先来认识一个名为ServerAddressesFeature的特性。
我们知道表示Server的接口IServer中定义了一个类型为IFeatureCollection 的只读属性Features，它表示用于描述当前Server的特性集合，ServerAddressesFeature作为一个重要的特性，就包含在这个集合之中。我们所说的ServerAddressesFeature对象是对所有实现了IServerAddressesFeature接口的所有类型及其对应对象的统称，该接口具有一个唯一的只读属性返回Server的监听地址列表。ASP.NET Core默认使用的ServerAddressesFeature是具有如下定义的同名类型。

C#
```
public interface IServerAddressesFeature
    {
        ICollection<string> Addresses { get; }
    }
     
    public class ServerAddressesFeature : IServerAddressesFeature
    {
        public ICollection<string> Addresses { get; }
    }
```
对于WebHost在通过依赖注入的方式创建的Server，由它的Features属性表示的特性集合中会默认包含这么一个ServerAddressesFeature对象。如果没有一个合法的监听地址被添加到这个 ServerAddressesFeature对象的地址列表中，WebHost会将显式指定的地址（一个或者多个）添加到该列表中。我们显式指定的监听地址实际上是作为WebHost的配置保存在一个Configuration对象上，配置项对应的Key为“server.urls”，WebHostDefaults的静态只读属性ServerUrlsKey返回的就是这么一个Key。

C#
```
new WebHostBuilder()
        .UseSetting(WebHostDefaults.ServerUrlsKey, "http://localhost:3721/")
        .UseMyKestrel()
        .UseStartup<Startup>()
        .Build()
        .Run();
```
WebHost的配置最初来源于创建它的WebHostBuilder，后者提供了一个UseSettings方法来设置某个配置项的值，所以我们可以采用如下的方式来指定监听地址（“http://localhost:3721/”）。不过，针对监听地址的显式设置，最直接的编程方式还是调用WebHostBuilder的扩展方法UseUrls，如下面的代码片段所示，该方法的实现逻辑与上面完全一致。

C#
```
public static class WebHostBuilderExtensions
    {
        public static IWebHostBuilder UseUrls(this IWebHostBuilder hostBuilder, params string[] urls) 
        =>hostBuilder.UseSetting(WebHostDefaults.ServerUrlsKey, string.Join(ServerUrlsSeparator, urls)) ;    
    }
```
