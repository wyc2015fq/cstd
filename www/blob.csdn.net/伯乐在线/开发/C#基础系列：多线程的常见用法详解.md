# C#基础系列：多线程的常见用法详解 - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4704139.html)
前言：此篇就主要从博主使用过的几种多线程的用法从应用层面大概介绍下。文中观点都是博主个人的理解，如果有不对的地方望大家指正~~
1、多线程：使用多个处理句柄同时对多个任务进行控制处理的一种技术。据博主的理解，多线程就是该应用的主线程任命其他多个线程去协助它完成需要的功能，并且主线程和协助线程是完全独立进行的。不知道这样说好不好理解，后面慢慢在使用中会有更加详细的讲解。
2、多线程的使用：
（1）最简单、最原始的使用方法：Thread oGetArgThread = new Thread(new ThreadStart(() =>{});这种用法应该大多数人都使用过，参数为一个ThreadStart类型的委托。将ThreadStart转到定义可知：

C#
```
public delegate void ThreadStart();
```
它是一个没有参数，没有返回值的委托。所以他的使用如下：

C#
```
static void Main(string[] args)
{
 　Thread oGetArgThread = new Thread(new ThreadStart(Test));
   oGetArgThread.IsBackground = true;
   oGetArgThread.Start();   
　 for (var i = 0; i < 1000000; i++)
　 {
　　　Console.WriteLine("主线程计数" + i);
　　　//Thread.Sleep(100);
　 }
}
private static void Test()
{
   for (var i = 0; i < 1000000; i++)
   {
      Console.WriteLine("后台线程计数" + i);
      //Thread.Sleep(100);
   }
}
```
定义一个没有参数没有返回值的方法传入该委托。当然也可以不定义方法写成匿名方法：

C#
```
static void Main(string[] args)
 {
      Thread oGetArgThread = new Thread(new System.Threading.ThreadStart(() =>
      {
          for (var i = 0; i < 1000000; i++)
          {
             Console.WriteLine("后台线程计数" + i);
             //Thread.Sleep(100);
          }
      }));
      oGetArgThread.IsBackground = true;
      oGetArgThread.Start();
 }
```
这个和上面的意义相同。得到的结果如下：
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/02/9c4c6cb327225c743d87d24fde268bfd.png)
说明主线程和后台线程是互相独立的。由系统调度资源去执行。
如果这样那有人就要问了，如果我需要多线程执行的方法有参数或者有返回值或者既有参数又有返回值呢。。。别着急我们来看看new Thread()的几个构造函数：

C#
```
public Thread(ParameterizedThreadStart start);
public Thread(ThreadStart start);
public Thread(ParameterizedThreadStart start, int maxStackSize);
public Thread(ThreadStart start, int maxStackSize);
```
转到定义可知参数有两类，一类是无参无返回值的委托，另一类是有参无返回值的委托。对于有参数的委托使用方法：

C#
```
static void Main(string[] args)
{
     Thread oThread = new Thread(new ParameterizedThreadStart(Test2));     
     oThread.IsBackground = true;
     oThread.Start(1000);
 }
 private static void Test2(object Count)
 {
     for (var i = 0; i < (int)Count; i++)
     {
         Console.WriteLine("后台线程计数" + i);
         //Thread.Sleep(100);
      }
  }
```
对于有参又有返回值的委托，很显然使用new Thread()这种方式是没有解决方案的。其实对于有参又有返回值的委托可以使用异步来实现：

C#
```
public delegate string MethodCaller(string name);//定义个代理 
MethodCaller mc = new MethodCaller(GetName); 
string name = "my name";//输入参数 
IAsyncResult result = mc.BeginInvoke(name,null, null); 
string myname = mc.EndInvoke(result);//用于接收返回值 
public string GetName(string name)    // 函数
{
    return name;
}
```
关于这种方式还有几点值得一说的是：
①Thread oGetArgThread = new Thread(new ThreadStart(Test));
oGetArgThread.Join();//主线程阻塞，等待分支线程运行结束，这一步看功能需求进行选择，主要为了多个进程达到同步的效果
②线程的优先级可以通过Thread对象的Priority属性来设置，Priority属性对应一个枚举：

C#
```
public enum ThreadPriority
{
        // 摘要: 
        //     可以将 System.Threading.Thread 安排在具有任何其他优先级的线程之后。
        Lowest = 0,
        //
        // 摘要: 
        //     可以将 System.Threading.Thread 安排在具有 Normal 优先级的线程之后，在具有 Lowest 优先级的线程之前。
        BelowNormal = 1,
        //
        // 摘要: 
        //     可以将 System.Threading.Thread 安排在具有 AboveNormal 优先级的线程之后，在具有 BelowNormal 优先级的线程之前。
        //     默认情况下，线程具有 Normal 优先级。
        Normal = 2,
        //
        // 摘要: 
        //     可以将 System.Threading.Thread 安排在具有 Highest 优先级的线程之后，在具有 Normal 优先级的线程之前。
        AboveNormal = 3,
        //
        // 摘要: 
        //     可以将 System.Threading.Thread 安排在具有任何其他优先级的线程之前。
        Highest = 4,
 }
```
从0到4，优先级由低到高。
③关于多个线程同时使用一个对象或资源的情况，也就是线程的资源共享，为了避免数据紊乱，一般采用.Net悲观锁lock的方式处理。

C#
```
private static object oLock = new object();
 private static void Test2(object Count)
 {
      lock (oLock)
      {
          for (var i = 0; i < (int)Count; i++)
          {
               Console.WriteLine("后台线程计数" + i);
               //Thread.Sleep(100);
           }
         }
  }
```
（2）Task方式使用多线程：这种方式一般用在需要循环处理某项业务并且需要得到处理后的结果。使用代码如下：

C#
```
List<Task> lstTaskBD = new List<Task>();
foreach (var bd in lstBoards)
    {
         var bdTmp = bd;//这里必须要用一个临时变量
         var oTask = Task.Factory.StartNew(() =>
         {
              var strCpBdCmd = "rm -Rf " + bdTmp.Path + "/*;cp -R " + CombineFTPPaths(FTP_EMULATION_BD_ROOT,
"bd_correct") + "/* " + bdTmp.Path + "/";
              oPlink.Run(bdTmp.EmulationServer.BigIP, bdTmp.EmulationServer.UserName, bdTmp.EmulationServer.Password,
strCpBdCmd);
              Thread.Sleep(500);
           });
           lstTaskBD.Add(oTask);
    }
Task.WaitAll(lstTaskBD.ToArray());//等待所有线程只都行完毕
```
使用这种方式的时候需要注意这一句 var bdTmp = bd;这里必须要用一个临时变量，要不然多个bd对象容易串数据。如果有兴趣可以调试看看。这种方法比较简单，就不多说了。当然Task对象的用法肯定远不止如此，还涉及到任务的调度等复杂的逻辑。博主对这些东西理解有限，就不讲解了。
（3）线程池的用法：一般由于考虑到服务器的性能等问题，保证一个时间段内系统线程数量在一定的范围，需要使用线程池的概念。大概用法如下：

C#
```
public class CSpiderCtrl
{
       //将线程池对象作为一个全局变量
        static Semaphore semaphore;
        public static void Run()
        {
            //1. 创建 SuperLCBB客户端对象
            var oClient = new ServiceReference_SuperLCBB.SOAServiceClient();
　　　　    //2.初始化的时候new最大的线程池个数255（这个数值根据实际情况来判断，如果服务器上面的东西很少，则可以设置大点）
            semaphore = new Semaphore(250, 255);
            CLogService.Instance.Debug("又一轮定时采集...");
            _TestBedGo(oClient);
  }
　　  //执行多线程的方法
　　  private static void _TestBedGo(ServiceReference_SuperLCBB.SOAServiceClient oClient)
        {
            List<string> lstExceptPDUs = new List<string>(){
                "SUPERLABEXP"
            };
            var oTestBedRes = oClient.GetTestBedExceptSomePDU(lstExceptPDUs.ToArray(), true);
            if (CKVRes.ERRCODE_SUCCESS != oTestBedRes.ErrCode)
            {
                CLogService.Instance.Error("xxx");
                return;
            }
            var lstTestBed = oTestBedRes.ToDocumentsEx();
            System.Threading.Tasks.Parallel.ForEach(lstTestBed, (oTestBed) =>
            {
　　　　　　　　　//一次最多255个线程，超过255的必须等待线程池释放一个线程出来才行
                semaphore.WaitOne();
                //CLogService.Instance.Info("开始采集测试床：" + oTestBed[TBLTestBed.PROP_NAME]);
                //Thread.Sleep(2000);
                var strTestBedName = oTestBed[TBLTestBed.PROP_NAME] as string;
                var strSuperDevIP = oTestBed[TBLTestBed.PROP_SUPERDEVIP] as string;
                var strTestBedGID = oTestBed[TBLTestBed.PROP_GID] as string;
                var strPdu = oTestBed[TBLTestBed.PROP_PDUGID] as string;
                Thread.Sleep(new Random().Next(1000, 5000));
                var oGetRootDevicesByTestBedGIDRes = oClient.GetRootDevicesByTestBedGID(strTestBedGID);
                CLogService.Instance.Debug(strPdu + "——测试床Name：" + strTestBedName + "开始");
                Stopwatch sp = new Stopwatch();
                sp.Start();
                if (oGetRootDevicesByTestBedGIDRes.ErrCode != CKVRes.ERRCODE_SUCCESS || oGetRootDevicesByTestBedGIDRes.Documents.Count < 2)
                {
                    CLogService.Instance.Debug("shit -- 3实验室中测试床Name：" + strTestBedName + "2完成异常0");
　　　　　　　//这里很重要的一点，每一次return 前一定要记得释放线程，否则这个一直会占用资源
                    semaphore.Release();
                    return;
                }
                var strXML = oGetRootDevicesByTestBedGIDRes.Documents[0];
                var strExeName = oGetRootDevicesByTestBedGIDRes.Documents[1];
                //var strExeName = "RateSpider";
                var oSuperDevClient = new SuperDevClient(CSuperDev.ENDPOINT, string.Format(CSuperDev.SuperDevURL, strSuperDevIP));
                try
                {
                    oSuperDevClient.IsOK();
                }
                catch (Exception)
                {
                    CLogService.Instance.Error("测试床Name：" + strTestBedName + "异常,插件没起");
                    semaphore.Release();
                    return;
                }
                //2.3.1.请求SuperDev.Server(SuperDevIP)，发送Run(XML和Exename)
                var oRunExeRes = new CKVRes();
                try
                {
                    oRunExeRes = oSuperDevClient.RunExeEx(strExeName, false, new string[] { strXML });
                }
                catch
                {
                    //CLogService.Instance.Debug("测试床Name：" + strTestBedName + "异常:" + ex.Message);
                }
                sp.Stop();
                CLogService.Instance.Debug(strPdu + "——测试床Name：" + strTestBedName + "完成时间" + sp.Elapsed);
　　　　　　　　　 //每一个线程完毕后记得释放资源
                semaphore.Release();
            });
        }
   }
```
需要注意：Semaphore对象的数量需要根据服务器的性能来设定；System.Threading.Tasks.Parallel.ForEach这种方式表示同时启动lstTestBed.Length个线程去做一件事情，可以理解为

C#
```
foreach(var oTestbed in lstTestBed)
{
    Thread oThread=new Thread(new ThreadStart({   ...}));          
}
```
（4） 多线程里面还有一个值得一说的SpinWait类，用于提供对基于自旋的等待的支持。也就是说支持重复执行一个委托，知道满足条件就返回，我们来看它的用法：

C#
```
public static void SpinUntil(Func<bool> condition); 
 public static bool SpinUntil(Func<bool> condition, int millisecondsTimeout);
 public static bool SpinUntil(Func<bool> condition, TimeSpan timeout);
```
这个方法有三个构造函数，后两个需要传入一个时间，表示如果再规定的时间内还没有返回则自动跳出，防止死循环。

C#
```
SpinWait.SpinUntil(() =>
{
     bIsworking = m_oClient.isworking(new isworking()).result;
     return bIsworking == false;
}, 600000);
//如果等了10分钟还在跳纤则跳出
if (bIsworking)
{
    oRes.ErrCode = "false交换机跳纤时间超过10分钟，请检查异常再操作";
    return oRes;
}
```
博主使用过的多线程用法大概就这么三大类，当然这些其中还涉及很多细节性的东西，博主原来使用这些的时候经常出现各种莫名的问题，可能还是没用好的原因，对这些东西理解还不够深刻。如果大家也遇到类似的问题可以拿出来探讨！！
### 本系列：
- [C#基础系列：再也不用担心面试官问我”事件”了](http://blog.jobbole.com/98200/)
- [C#基础系列：扩展方法的使用](http://blog.jobbole.com/98230/)
- [C#基础系列：Linq to Xml读写xml](http://blog.jobbole.com/98243/)
- [C#基础系列：反射笔记](http://blog.jobbole.com/98266/)
- [C#基础系列：Attribute特性使用](http://blog.jobbole.com/98348/)
- [C#基础系列：小话泛型](http://blog.jobbole.com/98359/)
- [C#基础系列：委托实现简单设计模式（1）](http://blog.jobbole.com/98378/)
- [C#基础系列：委托和设计模式（2）](http://blog.jobbole.com/98388/)
- [C#基础系列：序列化效率比拼](http://blog.jobbole.com/98397/)
- [C#基础系列：异步编程初探async和await](http://blog.jobbole.com/98405/)
