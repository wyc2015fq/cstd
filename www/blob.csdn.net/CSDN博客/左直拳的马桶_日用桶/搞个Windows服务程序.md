# 搞个Windows服务程序 - 左直拳的马桶_日用桶 - CSDN博客
2011年11月23日 11:01:15[左直拳](https://me.csdn.net/leftfist)阅读数：3639
windows服务程序方便，可以自动启动，也不受登录用户影响。没有界面，不大方便，但也有好处，眼不见心不烦。
说说它怎么安装：
1、添加一个项：安装程序类
![](http://hi.csdn.net/attachment/201111/23/0_1322017097T39T.gif)
2、在这个安装程序页面上添加两个组件ServiceInstaller,ServiceProcessInstaller，并设置
![](http://hi.csdn.net/attachment/201111/23/0_1322017167G9nN.gif)
![](http://hi.csdn.net/attachment/201111/23/0_13220171773hAW.gif)
![](http://hi.csdn.net/attachment/201111/23/0_1322017187B67J.gif)
**3、编译好的exe要用InstallUtil.exe来进行安装。**
installUtil.exe为windows自带，搜一下就能得到。
命令格式为：installutil ***.exe 
这里记得待安装的服务程序一定要写全称，就是要带后缀。
代码部分，要在program写一点东西才能启动：
```
using System.ServiceProcess;
    class Program
    {
        static void Main(string[] args)
        {
            ServiceBase[] ServicesToRun;
            // 同一进程中可以运行多个用户服务。若要将
            // 另一个服务添加到此进程中，请更改下行以
            // 创建另一个服务对象。例如，
            //
            //   ServicesToRun = new ServiceBase[] {new Service1(), new MySecondUserService()};
            //
            ServicesToRun = new ServiceBase[] { new Catch() };
            ServiceBase.Run(ServicesToRun);
        }
    }
```
安装的时候，如果提示
“安装因缺少 ServiceProcessInstaller而失败”，
可以改   this.Installers.AddRange(new System.Configuration.Install.Installer[] {this.serviceInstaller1,this.serviceProcessInstaller1});  少了“this.serviceProcessInstaller1”
**4、ACCOUNT**
ServiceProcessInstaller.Account
一般都是LocalSystem。如果指定为USER，则安装的时候需要指定账户名和密码。
注意安装的时候，填写账户名称时，即使是本机的账户，也应该带有域信息，格式是 .\UserName
密码是必须的，如果账户没有密码，也需要设置一个再安装，否则失败。
但是指定这个ACCOUNT似乎没有多大意义。比如说想满足这种需求：这个服务只在某个WINDOWS账户下使用，是办不到的。WINDOWS服务与普通应用程序不同，其所处的环境自成体系。好比你在服务里用 Environment.User，永远都是得到 SYSTEM 或是你指定的这个ACCOUNT，根本无法得知当前登录WINDOWS的账户是谁。
**5、服务中显示窗体**
从网上照抄的代码
```
#region Form
        void FormShow()
        {
            if (Form1.IsDirty)
            {
                return;
            }
            GetDesktopWindow(); 
            IntPtr hwinstaSave = GetProcessWindowStation(); 
            IntPtr dwThreadId = GetCurrentThreadId(); 
            IntPtr hdeskSave = GetThreadDesktop(dwThreadId); 
            IntPtr hwinstaUser = OpenWindowStation("WinSta0", false,33554432); 
            if (hwinstaUser == IntPtr.Zero) 
            { 
                RpcRevertToSelf(); 
                return ;
            } 
            SetProcessWindowStation(hwinstaUser); 
            IntPtr hdeskUser = OpenDesktop("Default", 0, false, 33554432); 
            RpcRevertToSelf(); 
            if (hdeskUser == IntPtr.Zero) 
            { 
                SetProcessWindowStation(hwinstaSave); 
                CloseWindowStation(hwinstaUser); 
                return ; 
            } 
            SetThreadDesktop(hdeskUser);
            IntPtr dwGuiThreadId = dwThreadId;
            Form1 f = new Form1();
            System.Windows.Forms.Application.Run(f);
            dwGuiThreadId = IntPtr.Zero;
            SetThreadDesktop(hdeskSave);
            SetProcessWindowStation(hwinstaSave);
            CloseDesktop(hdeskUser);
            CloseWindowStation(hwinstaUser);
        }
        [DllImport("user32.dll")]
        static extern int GetDesktopWindow();
        [DllImport("user32.dll")]
        static extern IntPtr GetProcessWindowStation();
        [DllImport("kernel32.dll")]
        static extern IntPtr GetCurrentThreadId();
        [DllImport("user32.dll")]
        static extern IntPtr GetThreadDesktop(IntPtr dwThread);
        [DllImport("user32.dll")]
        static extern IntPtr OpenWindowStation(string a,bool b,int c);
        [DllImport("user32.dll")]
        static extern IntPtr OpenDesktop(string lpszDesktop, uint dwFlags,
        bool fInherit, uint dwDesiredAccess);
        [DllImport("user32.dll")]
        static extern IntPtr CloseDesktop(IntPtr p);
        [DllImport("rpcrt4.dll", SetLastError=true)]
        static extern IntPtr RpcImpersonateClient(int i);
        [DllImport("rpcrt4.dll", SetLastError=true)]
        static extern IntPtr RpcRevertToSelf();
        [DllImport("user32.dll")]
        static extern IntPtr SetThreadDesktop(IntPtr a);
        [DllImport("user32.dll")]
        static extern IntPtr SetProcessWindowStation(IntPtr a);
        [DllImport("user32.dll")]
        static extern IntPtr CloseWindowStation(IntPtr a);
        #endregion
```
其中
Form1 f = new Form1();
就是要显示的窗体。
同时，服务这里要勾上 “允许服务与桌面交互”。方法是鼠标右键这个安装好的服务，属性，登录。好像代码里也可以设置。
但是这种方式，XP下还可以，WIN7下就比较尴尬，系统不是直接出来这个窗体，而是出来一个警告消息，交互式服务检测，问你要不要看一下这个消息。要看的话，才有这个窗体出来。
