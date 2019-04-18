# C#中如何获取系统环境变量等 - weixin_33985507的博客 - CSDN博客
2016年05月17日 09:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：21
C#中获取系统环境变量需要用到**Environment 类**。
**其中****提供了有关当前环境和平台的信息以及操作它们的方法**。该类不能被继承
以下代码得到%systemdrive%的值，即“C:”
```
string sPath = Environment.GetEnvironmentVariable("systemdrive");
Console.WriteLine(sPath);
```
更多使用方法：
```
//--------------获取当前程序信息
//当前程序启动的命令，字符串
Console.WriteLine("CommandLine:{0}", Environment.CommandLine);
string[] arguments = Environment.GetCommandLineArgs();
Console.WriteLine("GetCommandLineArgs:{0}", string.Join(", ", arguments));
//获取当前程序目录
Console.WriteLine("CurrentDirectory:{0}", Environment.CurrentDirectory);
//获取当前程序，工作的物理内存大小
Console.WriteLine("WorkingSet: {0}", Environment.WorkingSet);
//退出码
Console.WriteLine("ExitCode:{0}", Environment.ExitCode);
//获取当前程序是否开机启动
Console.WriteLine("HasShutdownStarted: {0}", Environment.HasShutdownStarted);
//获取当前跟踪信息
Console.WriteLine("StackTrace: '{0}'", Environment.StackTrace);
//获取换行符
Console.WriteLine("NewLine: {0}  first line{0}  second line{0}  third line",
                Environment.NewLine);
//--------------获取当前用户信息
//当前用户的域名称
Console.WriteLine("UserDomainName: {0}", Environment.UserDomainName);
//当前用户是否交互
Console.WriteLine("UserInteractive: {0}", Environment.UserInteractive);
//当前用户名称
Console.WriteLine("UserName: {0}", Environment.UserName);
//--------------获取当前系统信息
//获取当前计算机名称
Console.WriteLine("MachineName: {0}", Environment.MachineName);
//获取系统镜像等版本
Console.WriteLine("Version: {0}", Environment.Version.ToString());
//获取当前操作系统版本
Console.WriteLine("OSVersion: {0}", Environment.OSVersion.ToString());
//获取当前系统目录
Console.WriteLine("GetFolderPath: {0}",
                Environment.GetFolderPath(Environment.SpecialFolder.System));
//获取当前计算机逻辑磁盘名称列表
String[] drives = Environment.GetLogicalDrives();
Console.WriteLine("GetLogicalDrives: {0}", String.Join(", ", drives));
//获取系统目录
String str;
String nl = Environment.NewLine;
String query = "My system drive is %SystemDrive% and my system root is %SystemRoot%";
str = Environment.ExpandEnvironmentVariables(query);
Console.WriteLine("ExpandEnvironmentVariables: {0}  {1}", nl, str);
//获取系统临时文件夹
Console.WriteLine("GetEnvironmentVariable: {0}  My temporary directory is {1}.", nl,
                        Environment.GetEnvironmentVariable("TEMP"));
//--------------遍历所有信息
Console.WriteLine("GetEnvironmentVariables: ");
IDictionary environmentVariables = Environment.GetEnvironmentVariables();
foreach (DictionaryEntry de in environmentVariables)
{
    Console.WriteLine("  {0} = {1}", de.Key, de.Value);
}
```
