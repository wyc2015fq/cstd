# C#学习笔记19——Windows服务创建及安装 - 深之JohnChen的专栏 - CSDN博客

2011年09月01日 23:25:53[byxdaz](https://me.csdn.net/byxdaz)阅读数：1095标签：[windows																[c#																[timer																[service																[microsoft																[数据库](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[.Net & C#](https://blog.csdn.net/byxdaz/article/category/628880)


我们将研究如何创建一个作为Windows服务的应用程序。内容包含什么是Windows服务，如何创建、安装和调试它们。会用到System.ServiceProcess.ServiceBase命名空间的类。

什么是Windows服务？ 

　　 Windows服务应用程序是一种需要长期运行的应用程序，它对于服务器环境特别适合。它没有用户界面，并且也不会产生任何可视输出。任何用户消息都会被写进Windows事件日志。计算机启动时，服务会自动开始运行。它们不要用户一定登录才运行，它们能在包括这个系统内的任何用户环境下运行。通过服务控制管理器，Windows服务是可控的，可以终止、暂停及当需要时启动。

　　 Windows 服务，以前的NT服务，都是被作为Windows NT操作系统的一部分引进来的。它们在Windows 9x及Windows Me下没有。你需要使用NT级别的操作系统来运行Windows服务，诸如：Windows NT、Windows 2000 Professional或Windows 2000 Server。举例而言，以Windows服务形式的产品有：Microsoft Exchange、SQL Server，还有别的如设置计算机时钟的Windows Time服务。

创建一个Windows服务 

　　我们即将创建的这个服务除了演示什么也不做。服务被启动时会把一个条目信息登记到一个数据库当中来指明这个服务已经启动了。在服务运行期间，它会在指定的时间间隔内定期创建一个数据库项目记录。服务停止时会创建最后一条数据库记录。这个服务会自动向Windows应用程序日志当中登记下它成功启动或停止时的记录。

　　VisualStudio .NET能够使创建一个Windows服务变成相当简单的一件事情。启动我们的演示服务程序的说明概述如下。

1. 新建一个项目

2. 从一个可用的项目模板列表当中选择Windows服务

3. 设计器会以设计模式打开

4. 从工具箱的组件表当中拖动一个Timer对象到这个设计表面上 (注意: 要确保是从组件列表而不是从Windows窗体列表当中使用Timer) 

5. 设置Timer属性，Enabled属性为False，Interval属性30000毫秒

6. 切换到代码视图页(按F7或在视图菜单当中选择代码)，然后为这个服务填加功能

Windows服务的构成 

　　在你类后面所包含的代码里，你会注意到你所创建的Windows服务扩充了System.ServiceProcess.Service类。所有以.NET方式建立的Windows服务必须扩充这个类。它会要求你的服务重载下面的方法，VisualStudio默认时包括了这些方法。

Dispose –清除任何受控和不受控资源(managed and unmanagedresources)

OnStart –控制服务启动

OnStop –控制服务停止

Windows服务样例 

下面是一个测试案例，服务名为Service1黑色部分为自动生成，红色部分为我加进去的代码，绿色为我加入的注释，此案例没有其他意义，只是将记录插入到数据库中。

using System;

using System.Collections;

using System.ComponentModel;

using System.Data;

using System.Diagnostics;

using System.ServiceProcess;

using System.Configuration.Install;

using SysData.Db;

namespace serverTest

{

public class Service1 :System.ServiceProcess.ServiceBase

{

private System.Timers.Timer timer1;

/// <summary> 

/// 必需的设计器变量。

/// </summary>

private System.ComponentModel.Containercomponents = null;

public Service1()

{

// 该调用是 Windows.Forms 组件设计器所必需的。

InitializeComponent();

// TODO: 在 InitComponent 调用后添加任何初始化

}

// 进程的主入口点

static void Main()

{

System.ServiceProcess.ServiceBase[]ServicesToRun;

// 同一进程中可以运行多个用户服务。若要将另一个服务添加到此进程，请更改下行以创建另一个服务对象。例如，

//

// ServicesToRun = New System.ServiceProcess.ServiceBase[]

 {newService1(), new MySecondUserService()};

//

ServicesToRun = newSystem.ServiceProcess.ServiceBase[] { new Service1() };

System.ServiceProcess.ServiceBase.Run(ServicesToRun);

}

/// <summary> 

/// 设计器支持所需的方法 - 不要使用代码编辑器 

/// 修改此方法的内容。

/// </summary>

private void InitializeComponent()

{

this.timer1 = new System.Timers.Timer();

((System.ComponentModel.ISupportInitialize)(this.timer1)).BeginInit();

// 

// timer1

// 

this.timer1.Enabled = true;

this.timer1.Interval = 30000;

this.timer1.Elapsed += newSystem.Timers.ElapsedEventHandler(this.timer1_Elapsed);

// 

// Service1

// 

this.ServiceName = "Service1";

((System.ComponentModel.ISupportInitialize)(this.timer1)).EndInit();

}

/// <summary>

/// 清理所有正在使用的资源。

/// </summary>

protected override void Dispose( booldisposing )

{

if( disposing )

{

if (components != null) 

{

components.Dispose();

}

}

base.Dispose( disposing );

}

/// <summary>

/// 设置具体的操作，以便服务可以执行它的工作。

/// </summary>

protected override void OnStart(string[]args)

{

// TODO: 在此处添加代码以启动服务。

this.timer1.Enabled = true;

this.LogMessage("ServiceStarted");

}

/// <summary>

/// 停止此服务。

/// </summary>

protected override void OnStop()

{

// TODO: 在此处添加代码以执行停止服务所需的关闭操作。

this.timer1.Enabled = false;

this.LogMessage("ServiceStopped");

}

private void LogMessage(string xMsg)

{

try

{

//这里向数据库中插入一条信息为 xMsg的记录，下边是我调用事先写好的Db类添加记录的方法，您也可以使用其他办法来写入数据库。

//Db.QuerySQL("Insert into SysMsg(SysMsg) values ('"+xMsg+"')");

}

catch

{

//不做任何操作

}

}

private void timer1_Elapsed(object sender,System.Timers.ElapsedEventArgs e)

{

LogMessage("检查服务运行！");

}

}

}

安装Windows服务 

　　Windows服务不同于普通Windows应用程序。不可能简简单单地通过运行一个EXE就启动Windows服务了。安装一个Windows服务应该通过使用.NET Framework提供的InstallUtil.exe来完成，或者通过诸如一个Microsoft Installer(MSI)这样的文件部署项目完成。

添加服务安装程序 

　　创建一个Windows服务，仅用InstallUtil程序去安装这个服务是不够的。你必须还要把一个服务安装程序添加到你的Windows服务当中，这样便于InstallUtil或是任何别的安装程序知道应用你服务的是怎样的配置设置。

1. 将这个服务程序切换到设计视图

2. 右击设计视图选择“添加安装程序”

3. 切换到刚被添加的ProjectInstaller的设计视图

4. 设置serviceInstaller1组件的属性： 

1) ServiceName = My Sample Service

2) StartType = Automatic

5. 设置serviceProcessInstaller1组件的属性 

1) Account = LocalSystem

6. 生成解决方案

用InstallUtil安装Windows服务 

　　现在这个服务已经生成，你需要把它安装好才能使用。下面操作会指导你安装你的新服务。

1. 打开Visual Studio .NET命令提示

2. 改变路径到你项目所在的bin\Debug文件夹位置(如果你以Release模式编译则在bin\Release文件夹)

3. 执行命令“InstallUtil.exe MyWindowsService.exe”注册这个服务，使它建立一个合适的注册项。

注意：直接运行InstallUtil.exe 可能会失败，提示：

'InstallUtil.exe' 不是内部或外部命令，也不是可运行的程序或批处理文件。

您可以在C:\WINNT\Microsoft.NET\Framework\v1.1.4322 找到这个可执行程序

4. 右击桌面上“我的电脑”，选择“管理”就可以打计算机管理控制台

5. 在“服务和应用程序”里面的“服务”部分里，你可以发现你的Windows服务已经包含在服务列表当中了

6. 右击你的服务选择启动就可以启动你的服务了

　　在每次需要修改Windows服务时，这就会要求你卸载和重新安装这个服务。不过要注意在卸载这个服务前，最好确保服务管理控制台已经关闭，这会是一个很好的习惯。如果没有这样操作的话，你可能在卸载和重安装Windows服务时会遇到麻烦。仅卸载服务的话，可以执行相的InstallUtil命令用于注销服务，不过要在后面加一个/u命令开关。

调试Windows服务 

　　从另外的角度度看，调试Windows服务绝不同于一个普通的应用程序。调试Windows服务要求的步骤更多。服务不能象你对普通应用程序做的那样，只要简单地在开发环境下执行就可以调试了。服务必须首先被安装和启动，这一点在前面部分我们已经做到了。为了便于跟踪调试代码，一旦服务被启动，你就要用Visual Studio把运行的进程附加进来(attach)。记住，对你的Windows服务做的任何修改都要对这个服务进行卸载和重安装。

附加正在运行的Windows服务 

　　为了调试程序，有些附加Windows服务的操作说明。这些操作假定你已经安装了这个Windows服务并且它正在运行。

1. 用Visual Studio装载这个项目 

2. 点击“调试”菜单

3. 点击“进程”菜单

4. 确保显示系统进程 被选

5. 在可用进程 列表中，把进程定位于你的可执行文件名称上点击选中它

6. 点击附加 按钮

7. 点击确定

8. 点击关闭

9. 在timer1_Elapsed方法里设置一个断点，然后等它执行

总结 

　　现在你应该对Windows服务是什么，以及如何创建、安装和调试它们有一个粗略的认识了。Windows服务的额处的功能你可以自行研究。这些功能包括暂停(OnPause)和恢复(OnContinue)的能力。暂停和恢复的能力在默认情况下没有被启用，要通过Windows服务属性来设置。


