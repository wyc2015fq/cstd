# JAVA实现分布式缓存 - weixin_33985507的博客 - CSDN博客
2013年03月12日 11:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
1概述
     通常情况下我们运行程序的过程中会产生一些中间数据，这些中间数据需要在将来的某个时间读取。这就要求我们要把它存在一个提供高速存取的地方，最好的选择就是内存中。基于这个以及多个原因需要我们把这部分存储到其他机器上，这样就产生了分布式缓存的问题。
    实际上分布式缓存根本上就是提供一个附加内存让另一台机器帮忙存储和查找数据。
2实现方法
    首先建立一个集合对象，该集合对象应保证线程安全。代码如下所示
Code
1 public static class MemObject
2     {
3         static MemObject()
4         {
5             MemObjl = new Dictionary<string, object>();
6         }
7 
8         public static Dictionary<string, object> Get()
9         {
10             if (MemObjl == null)
11                 MemObjl = new Dictionary<string, object>();
12             return MemObjl;
13         }
14 
15         public static void Add(string key, object obj)
16         {
17             Dictionary<string, object> obg = Get();
18             if (!obg.ContainsKey(key))
19                 obg.Add(key, obj);
20         }
21 
22         public static void Remove(string key)
23         {
24             Get().Remove(key);
25         }
26 
27         public static int Count()
28         {
29             return Get().Count;
30         }
31 
32         public static object Get(string key)
33         {
34             Dictionary<string, object> obg = Get();
35             if (obg.ContainsKey(key))
36                 return obg[key];
37             return null;
38         }
39 
40         public static bool Exits(string key)
41         {
42             return Get().ContainsKey(key);
43         }
44 
45         [PR](http://pr.knowsky.com/)ivate static Dictionary<string, object> MemObjl;
46     }
接着我们把它包装起来可以通过远程调用，代码如下
Code
1 public class DataCatcher : MarshalByRefObject, ICarrier.ICarrier
2     {
3         public void Set(string key, object value)
4         {
5             //if (Exits(key))
6             //    Remove(key);
7             //MemObjl.Add(key, value);
8             MemObject.Add(key, value);
9         }
10 
11         public bool Exits(string key)
12         {
13             return MemObject.Exits(key);
14         }
15 
16         public void Remove(string key)
17         {
18             MemObject.Remove(key);
19         }
20 
21         public int Count()
22         {
23             return MemObject.Count();
24         }
25 
26         public object Get(string key)
27         {
28             return MemObject.Get(key);
29         }
30     }
为了避免我们的业务逻辑泄露我们向客户端提供接口以便调用
Code
1     public interface ICarrier
2     {
3 
4         void Remove(string key);
5 
6         bool Exits(string key);
7 
8         void Set(string key,object value);
9 
10         object Get(string key);
11 
12         int Count();
13     }
好了。这样我们服务端的代码就算搞定了。
下面我们来发布服务供客户端调用
Code
1 public partial class SharpCatchedService : ServiceBase
2     {
3         public SharpCatchedService()
4         {
5             InitializeComponent();
6         }
7 
8         protected override void OnStart(string[] args)
9         {
10             TcpChannel channel = new TcpChannel(ConfigHelper.Port);
11             ChannelServices.RegisterChannel(channel, false);
12             RemotingConfiguration.RegisterWellKnownServiceType(typeof(DataCatcher),
13                 "SharpCatched", WellKnownObjectMode.Singleton);
14         }
15 
16         protected override void OnStop()
17         {
18         }
19     }
这样客户端就可以通过这个接口来实现远程数据的存取
在客户端首先我们获取远程的对象
Code
        public static ICarrier Carrier()
        {
            ICarrier carrier = (ICarrier)Activator.GetObject(typeof(ICarrier), "tcp://localhost:" + ConfigHelper.Port + "/SharpCatched");
            return carrier;
        }
接着我们包装一下
Code
1 public class SharpCatchedAPI
2     {
3         ICarrier icarrier;
4 
5         public void Init()
6         {
7             icarrier = DoConnect.Carrier();
8         }
9 
10         public void Set(string key, object value)
11         {
12             icarrier.Set(key, value);
13         }
14 
15         public void Remove(string key)
16         {
17             icarrier.Remove(key);
18         }
19 
20         public object Get(string key)
21         {
22             return icarrier.Get(key);
23         }
24 
25         public bool Exits(string key)
26         {
27             return icarrier.Exits(key);
28         }
29     }
3后续
     以上实现的是最基本的分布式缓存解决方案。其实我们可以把这个集合转化为其他集合对象，例如HashTable。在对象启动的时候开启一个守护线程，这个进程做得工作就是把到期的缓存对象追加到一个集合对象中，然后遍历该对象实现缓存对象的销毁。我们还可以把对象进行一次哈希让对像在多台缓存服务器上存储
