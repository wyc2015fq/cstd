# C＃下实现动态系统托盘图标 - weixin_33985507的博客 - CSDN博客
2006年06月30日 17:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
作者：王凯明
来源：网络  
Visual C#是微软公司推出的下一代程序开发语言，是微软.Net框架中的的一个重要组成部分。微软的.Net平台堪称为新一代的互联网平台。对于技术开发人员来说，.Net平台的两大特性是非常卓越的，首先是其所提供的公共语言运行时,也就是公用语言运行平台,另一方面就是其庞大而全面的统一编程类。Visual C#就是通过调用这些类、对象来实现许多比较强大的功能。 
本文通过向大家介绍一个.net平台上用c#写的可以停放在任务栏上的图标程序（类似于flashget、oicq那种系统托盘图标）来和大家一起体验一下用c＃编程的乐趣。 
一、概述： 
我这里所指的图标程序是类似于在Windows中经常接触的停放在任务栏上，在后台运行的病毒监视、媒体播放等程序。它们一般都具有单击显示或隐藏主界面，击右键弹出菜单，当鼠标停在图标上时显示提示信息的功能。 
程序主要用到了一下几个类： 
system.windows.fomrs.notifyicon类 
System.Windows.Forms.ContextMenu类 
System.Windows.Forms.MenuItem类 
System.Windows.Forms.Label类 
System.Windows.Forms.Timer类  
二、程序设计和运行的环境： 
（1）微软公司视窗2000服务器版或视窗XP版 
（2）.net FrameWrok SDK Beta 2版 
三、具体实现步骤： 
（1）．在Visual Studio下新建一个C＃的工程，不妨命名为AnimateSystemTray。图示如下：   
![20011225-01.jpg](http://www.ddvip.net/program/c-/index5/img_c/20011225-01.jpg)
（2）.现在我们来设计程序的主界面： 
先将窗体的Text属性设置为"动画系统图标示例"，MaximiseBox、MinimiseBox、ShowInTaskbar属性均设置为False。 
再往窗体上添加Label控件、NotifyIcon控件、ContextMenu控件、Timer控件各一个。 
将Label控件的Text属性设置为"请右击系统托盘图标"；将NotifyIcon控件的Icon属性设置为一个图标文件： ![20011225-02.jpg](http://www.ddvip.net/program/c-/index5/img_c/20011225-02.jpg) ，Text属性设置为"这是个示例程序！",ContextMenu属性设置为"contextMenu1"；接下来开始编辑ContextMenu控件，图示如下： 
![20011225-03.jpg](http://www.ddvip.net/program/c-/index5/img_c/20011225-03.jpg)
最后还要在导入两个图标 ![20011225-02.jpg](http://www.ddvip.net/program/c-/index5/img_c/20011225-02.jpg)(Icon1.ico)， ![20011225-04.jpg](http://www.ddvip.net/program/c-/index5/img_c/20011225-04.jpg) (Icon2.ico)文件到该项目。 
（3）.现在我们开始编写代码部分： 
首先在我们的类中添加以下数据成员： 
private Icon m_Icon1; 
private Icon m_Icon2; 
private bool m_bFlag; 
private bool m_bShowWnd; 
修改Form1()函数为： 
public Form1() 
{ 
// 
// Required for Windows Form Designer support 
// 
InitializeComponent(); 
// 
// TODO: Add any constructor code after InitializeComponent call 
// 
m_bFlag=true; 
m_bShowWnd=true; 
try 
{ 
m_Icon1 = new Icon("Icon1.ico");//导入图标文件 
m_Icon2 = new Icon("Icon2.ico"); 
} 
catch ( Exception e ) 
{ 
MessageBox.Show("Error " + e.Message,"Animate Tray - Error"); 
menuItem2.Enabled = false; 
menuItem3.Enabled = false; 
} 
} 
添加menuItem1、menuItem2、menuItem3、m_trayIcon的Click事件，消息处理函数如下： 
private void menuItem1_Click(object sender, System.EventArgs e) 
{ 
timer1.Start();//打开计时器 
} 
private void menuItem2_Click(object sender, System.EventArgs e) 
{ 
timer1.Stop();//停止计时器 
} 
private void menuItem3_Click(object sender, System.EventArgs e) 
{ 
Application.Exit();//退出应用程序 
} 
private void m_trayIcon_Click(object sender, System.EventArgs e) 
{ 
if(m_bShowWnd == true)//隐藏主界面 
{ 
this.Visible = false; 
m_bShowWnd = false; 
} 
else//显示主界面 
{ 
this.Visible = true; 
m_bShowWnd = true; 
} 
} 
最后还要添加timer1的Tick（）函数： 
private void timer1_Tick(object sender, System.EventArgs e) 
{ 
if ( m_Icon1 != null && m_Icon2 != null ) //如果两个图标文件都被正确载入 
{ 
//只要timer1被启动，则在两个图标之间不断进行选择变换，实现动画效果 
if ( m_bFlag == true ) 
{ 
m_trayIcon.Icon = m_Icon2; 
m_bFlag = false; 
} 
else 
{ 
m_trayIcon.Icon = m_Icon1; 
m_bFlag = true; 
} 
} 
} 
好了，按Ctrl＋F5试试效果吧！ 
运行程序，将显示一个主窗口，此时还可以看到在任务栏上面多了一个图标。当鼠标移到其上停留一下时，将显示一个简单的提示信息，点击该图标则将隐藏主窗口，单击右键，出来一个菜单，选"动画图标开始"和"动画图标结束"可以控制图标状态。 
**四．总结： **
做完这个程序，我们不难发现用C＃做一些运用程序效果是非常方便的。以前我在SDK、MFC下，用到了Shell_NotifyIcon（）、TrackPopMenu()以及DestroyMenu()，还要自己添加消息处理函数，工作量是非常大的。现在微软为我们做好了这些工作，是不是很方便？快快加入C#的行列吧。

