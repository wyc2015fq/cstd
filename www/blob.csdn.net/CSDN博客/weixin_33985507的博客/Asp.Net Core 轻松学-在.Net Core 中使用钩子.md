# Asp.Net Core 轻松学-在.Net Core 中使用钩子 - weixin_33985507的博客 - CSDN博客
2018年12月24日 09:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
原文:[Asp.Net Core 轻松学-在.Net Core 中使用钩子](https://www.cnblogs.com/viter/p/10140697.html)
## 前言
    Host startup hook，是2.2中提供的一项新的功能，通过使用主机启动钩子，允许开发人员在不修改代码的情况下，在服务启动之前注入代码；通过使用钩子，可以对已部署好的服务在服务启动期间自定义托管程序的行为；通过使用钩子，可以对服务进行跟踪或者遥测，也可以在服务启动前对托管环境进行健康检查；还可以通过钩子动态加载程序集进行依赖注入等功能。
## 什么是钩子
> 
钩子的作用原理是通过设置环境变量 DOTNET_STARTUP_HOOKS 的值将钩子程序挂载到托管程序之中，在托管程序启动的时候，CoreCLR 将按照钩子列表顺序进行检查，初始化后执行每个钩子程序，当钩子列表中的钩子程序被逐一执行完成后，托管程序将返回到程序主入口 Main 方法，进入一系列的启动，钩子程序可以是任何 .Net Core 版本的类库项目，在项目内必须包含类 StartupHook 这是固定命名，且 StartupHook 必须是一个没有命名空间的内部类，包含默认的静态方法 Initialize()，符合此规范即可作为钩子程序进行托管挂载
## 使用钩子
###### 1.首先创建一个控制台项目 Ron.HooksDemo ，作为托管主机，用于挂载钩子程序 Ron.Init
> 
Ron.HooksDemo 的代码非常简单，仅仅输出一句话
```
class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("\n程序已启动");
            Console.ReadKey();
        }
    }
```
##### 2. 创建钩子程序，Ron.Init
2.1 按照钩子程序的规范，创建一个无命名空间的内部类 StartupHook ，且包含默认静态方法 Initialize()
```
internal class StartupHook
{
    public static void Initialize()
    {
        Console.WriteLine("程序集：Ron.Init.dll");
        Console.WriteLine("正在获取服务器信息.....");
        string[] drives = Environment.GetLogicalDrives();
        Console.WriteLine("machineName:{0},\nOSVersion:{1},\nversion:{2},\nuserName:{3},\nCurrentDirectory:{4}\nCore Count:{5}\nWorkSet:{6}\nDrives:{7}",
            Environment.MachineName,
            Environment.OSVersion,
            Environment.Version,
            Environment.UserName,
            Environment.CurrentDirectory,
            Environment.ProcessorCount,
            Environment.WorkingSet,
            string.Join(",", drives));
        Console.WriteLine("\n\n正在获取网络配置.....");
        var hostName = Dns.GetHostName();
        Console.WriteLine("HostName:{0}", hostName);
        var addresses = Dns.GetHostAddresses(hostName);
        foreach (var item in addresses)
        {
            IPAddress ip = item.MapToIPv4();
            Console.WriteLine("AddressFamily:{0} \tAddress:{1}", ip.AddressFamily, ip);
        }
        Console.WriteLine("\n\n正在上报启动信息.....");
        Console.WriteLine("=========== Ron.Init.dll 结束 ===========");
    }
}
```
> 
上面的代码即表示一个标准的钩子程序，在 Initialize() 内部，进行托管主机检查，获取网络配置等行为，最好，还打印一条上报到遥测服务器的信息，这里是模拟上报检查报告，最后输出结束信息
代码非常检查，现在打开 Ron.HooksDemo 项目属性页进行钩子挂载
![26882-20181218225040995-1824464537.png](https://img2018.cnblogs.com/blog/26882/201812/26882-20181218225040995-1824464537.png)
> 
上图添加环境变量 DOTNET_STARTUP_HOOKS ，并设置其值为 C:\Users\Administrator\Source\Repos\Ron.HooksDemo\Ron.Init\bin\Debug\netcoreapp2.2\Ron.Init.dll，这是本次示例的钩子程序绝对路径
注意：该环境变量的值不支持相对路径，如果尝试使用相对路径，托管主机将抛出 ArgumentException 异常
2.2 运行程序，看看是否正确挂载了钩子程序 Ron.Init
![26882-20181218225050798-174625122.png](https://img2018.cnblogs.com/blog/26882/201812/26882-20181218225050798-174625122.png)
> 
上图红色部分输出信息表示钩子程序挂载成功，蓝色部分表示托管主机已启动，可以看到，托管主机启动是在挂载钩子之后运行的
一定要注意，钩子是在托管程序的 Main 方法之前运行的
## 3. 挂载多个钩子
##### 3.1 一个托管程序可以挂载多个钩子
> 
挂载多个钩子的方法是设置环境变量 DOTNET_STARTUP_HOOKS 的值，多个钩子按顺序执行，其中 Windows 和 Unix 挂载多个钩子的方式基本相同，这其中，有一点微小的区别
- Windows 平台挂载方式
`DOTNET_STARTUP_HOOKS = C:\Hooks_1.dll;C:\Hooks_2.dll`- Unix 平台挂载方式
`DOTNET_STARTUP_HOOKS =/data/Hooks_1.dll:/data/Hooks_2.dll`
> 
以上 DOTNET_STARTUP_HOOKS 变量的值包含两个钩子程序，其中 Windows 平台的值为使用分号（;）进行分隔，Unix 平台使用冒号（:）进行分隔，这于传统使用方式一致
##### 3.2 运行挂载了多个钩子的托管程序
- 
下面把两个钩子挂载到 Ron.HooksDemo 项目后,他们分别是：Ron.Init 和 Ron.License
> 
Ron.Init 钩子输出的是检查服务器信息，这个信息在之前已经演示，这里不再重复，下面看 Ron.License 代码
```
public static void Initialize()
    {
        Console.WriteLine("\n\n程序集：Ron.License.dll");
        Console.WriteLine("作者：Ron.liang");
        Console.WriteLine("博客地址：https://www.cnblogs.com/viter/\n\n");
        Console.WriteLine("=========== Ron.License.dll 结束 ===========");
    }
```
- 钩子程序的 Ron.License 代码也非常简单，结构和 Ron.Init 钩子程序一致，只是简单的输出版权信息
##### 3.3 运行 Ron.HooksDemo 程序，看下图输出结果
![26882-20181218225103571-96414424.png](https://img2018.cnblogs.com/blog/26882/201812/26882-20181218225103571-96414424.png)
> 
红色部分是 Ron.Init 钩子输出信息，黄色部分是 Ron.License 输出信息，蓝色部分是托管主机 Ron.HooksDemo 输出信息
可以看到，钩子上安装挂载的顺序执行的
## 4. 在钩子中加载额外的程序集
> 
我们应该这么理解，钩子程序也是一个普通的应用程序集；所以一个普通的程序集能做到事情，钩子也一样可以
##### 4.1 在 Ron.License 加载一个程序集 Ron.Service，Ron.Service 中定义了一个类 UserService，继承自并实现 IDisposable 接口
```
public class UserService : IDisposable
    {
        public void Dispose()
        {
            Console.WriteLine("程序集：Ron.Service.dll");
            Console.WriteLine("动态加载程序集，执行清理任务已完成\n\n");
            Console.WriteLine("=========== Ron.Service.dll 结束 ===========");
        }
    }
```
##### 4.2 在 Ron.License 的钩子方法中加载 Ron.Service 程序集，创建 IDisposable 的实现，并调用 Dispose() 方法
```
internal class StartupHook
{
    public static void Initialize()
    {
        Console.WriteLine("\n\n程序集：Ron.License.dll");
        Console.WriteLine("作者：Ron.liang");
        Console.WriteLine("博客地址：https://www.cnblogs.com/viter/\n\n");
        string path = @"C:\Users\Administrator\Source\Repos\Ron.HooksDemo\Ron.Service\bin\Debug\netcoreapp2.2\Ron.Service.dll";
        var assembly = AssemblyLoadContext.Default.LoadFromAssemblyPath(path);
        dynamic obj = assembly.CreateInstance("Ron.Service.UserService");
        obj.Dispose();
        Console.WriteLine("=========== Ron.License.dll 结束 ===========");
    }
}
```
##### 4.3 运行程序 Ron.HooksDemo
![26882-20181218225115976-1935002632.png](https://img2018.cnblogs.com/blog/26882/201812/26882-20181218225115976-1935002632.png)
> 
从输出结果看到，Ron.Service 程序集已被成功加载并调用，控制台红色输出信息部分表示加载成功
## 5. 在 Asp.Net Web Api 项目中使用钩子
> 
Web Api 项目挂载钩子的方式和控制台方式相同，首先我们还是创建一个 Web Api 项目 Ron.HooksDemo.Web
接着挂载钩子
`  "DOTNET_STARTUP_HOOKS": "C:\\Users\\Administrator\\Source\\Repos\\Ron.HooksDemo\\Ron.Init\\bin\\Debug\\netcoreapp2.2\\Ron.Init.dll;C:\\Users\\Administrator\\Source\\Repos\\Ron.HooksDemo\\Ron.License\\bin\\Debug\\netcoreapp2.2\\Ron.License.dll"`
##### 5.1 运行 Web Api 项目 Ron.HooksDemo.Web
![26882-20181218225127589-657692431.png](https://img2018.cnblogs.com/blog/26882/201812/26882-20181218225127589-657692431.png)
> 
红色输出部分表示 Web Api 程序的 Main 方法在钩子列表执行完成之后成功启动，这表示在 .Net Core 中，挂载钩子的方式是一致的，其行为也相同
## 结束语
> 
使用钩子程序注意事项
- 钩子程序不能依赖于托管主机的TPA列表之外的任何程序集，否则会抛出 FileNotFoundException 的异常
- 不要挂载过多的钩子程序，这可能会出现兼容性问题，如果要使用多个钩子，必须确保每个钩子程序的行为都是独立的，互不干扰的，如果一定要使用，建议修改托管主机的代码，使用依赖注入的方式而不是钩子
- StartupHook 类应该是 internal 类型的，如果是使用 public 进行修饰，还是可以正常加载钩子程序
## 演示代码下载
https://files.cnblogs.com/files/viter/Ron.HooksDemo.zip
