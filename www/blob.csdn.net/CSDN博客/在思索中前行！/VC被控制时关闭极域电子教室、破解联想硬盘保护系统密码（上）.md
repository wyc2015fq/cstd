# VC被控制时关闭极域电子教室、破解联想硬盘保护系统密码（上） - 在思索中前行！ - CSDN博客





2014年09月26日 13:21:15[_Tham](https://me.csdn.net/txl16211)阅读数：3205








## [《[原]关于VC运行时关闭极域电子教室的改进方法》](http://sunnysab.blog.163.com/blog/static/180375009201323191111647/)



本文将讲资料和方法，具体实现和破解联想硬盘保护系统密码在（下）中，有关破解联想硬盘保护系统（删除它）的方法很简单，用硬盘保护卡克星就可以了，但是恢复很麻烦，我不知道恢复MBR引导记录这个办法是否有效（在还原卡正常的电脑里用360系统急救箱备份MBR，再在已破解的电脑上恢复这个备份的MBR，我没试过，这需要知道硬盘保护卡克星的工作原理，如果它不只是清除MBR的话那就找维修店吧，这办法有点险，当然前面也提到在（下）中将说明如何破解联想硬盘保护系统密码），另外，本实验仅在windows
 xp环境下进行。

        学校有综合实践活动课，每个人可以自己按爱好报活动，我报了个电脑创作，但真正上课时，又十分郁闷。学校电脑监控和还原配置：（监控软件）极域电子教室V6 2007豪华版，有的机房还有联想网络控制工具，但我们的机房没有。（还原软件）联想硬盘保护系统。

        步入正题，每天看着屏幕发呆也不是个事，毕竟有些内容还是蛮简单的，于是我想到了破解极域电子教室密码。网上有资料，说极域电子教室的密码在[HKEY_LOCAL_MACHINE\SOFTWARE\TopDomain\e-learning Class Standard\1.00]下的UninstallPasswd键值内容就是明文密码了。我试了一试，也成功了，于是破了密码就进入设置，然后好好玩一玩。我把“防止学生关闭本进程”的选项关了，点击确定，现在就可以在windows任务管理器里面关闭它了。（任务管理器被禁用时的解决办法，来源，百度经验：[http://jingyan.baidu.com/article/00a07f3889107082d028dc0f.html](http://jingyan.baidu.com/article/00a07f3889107082d028dc0f.html)）

        但是现在又出现了一个问题：这样岂不是太麻烦吗？要是有一种能随时在被老师所谓“屏幕广播”的状态下退出极域电子教室该多好啊，所以我们现在就得向这个办法进军。我在网上找到了一些资料，说极域电子教室无法阻拦Ctrl、Shift等键，这就好办了，因为5下shift键可以启动windows的辅助功能——粘滞键。再加上平时信息技术课时同学们总是害人，在老师控制电脑时按五下shift，结果弹出了一个关不掉的窗口（因为是在被控制嘛）。我们可以利用粘滞键功能来关闭老师的极域电子教室和那讨厌的“屏幕广播窗口”。（关于启用粘滞键功能，[http://wenwen.soso.com/z/q345572886.htm](http://wenwen.soso.com/z/q345572886.htm)有。按5下shift是启动粘滞键，先要启用才能启动这个道理自然不必多说）

        说到这，你应该知道怎么办了。我们实际上可以将粘滞键程序替换成自己的程序。粘滞键的程序名是：sethc.exe，位于C:\WINDOWS\System32\下，在C:\WINDOWS\System32\dllcache\下也有备份，这个备份就是windows文件保护的事情了，win7的“dllcache”是C:\WINDOWS\Winsxs\文件夹，但是存储方式明显不同，它多了许多文件夹。

        下一节，我们就开始用C++语言写关闭极域电子教室的程序（一句话即可：system("taskkill -f -im StudentMain.exe"); ），并且我们还讨论破解联想硬盘保护系统的密码，预告下，联想硬盘保护系统的密码就在MBR里。

## [《[原]VC被控制时关闭极域电子教室、破解联想硬盘保护系统密码（下）》](http://sunnysab.blog.163.com/blog/static/1803750092013266371592/)
[《[原]关于VC运行时关闭极域电子教室的改进方法》](http://sunnysab.blog.163.com/blog/static/180375009201323191111647/)


  首先说说怎么查看密码，用工具[http://l5.yunpan.cn/lk/Q8FBhqALyPZ4W](http://l5.yunpan.cn/lk/Q8FBhqALyPZ4W)就可以了，网上也有下载的，但是我在学校机房测试的时候发现：“从LenRCClient.exe读取”这个选项不管用，虽然提示“找到进程，其PID为XXXX”。无奈，我试了试从硬盘读取，结果也不成，我只好把最后的希望寄托在“从MBR备份文件中读取”这一项了。但是MBR文件怎么弄呢？身边没有其他工具，只有一个360系统急救箱，它可帮了我大忙，我用它备份了MBR引导文件，然后点击“联想硬盘保护系统密码破解”软件的“从MBR备份文件中读取”的这一项，选择好我们刚才备份的MBR引导文件，试试，成功了！这标志着我们可以不使用清除MBR的方法来进行破解。（注：联想硬盘保护系统的默认密码应该是lenovo）

        回到如何实现被控制时关闭极域电子教室。前面已经说到了粘滞键，我们就用粘滞键来进行摆脱控制。处理程序：（完整源码下载地址：1.[360云盘](http://l5.yunpan.cn/lk/Q8WM3Pa5GjvG8) 2.[新浪微盘](http://vdisk.weibo.com/s/tE-a_)）

**// #include "include.h"**





```cpp
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
void OnStart()
{
 char szAppName[256]={0};
 GetModuleFileName(NULL,szAppName,128);
 if( strcmp(szAppName,"C:\\WINDOWS\\system32\\sethc.exe") )
 {
  CopyFile(szAppName,"C:\\WINDOWS\\system32\\sethc.exe",FALSE);
  //CopyFile(szAppName,"C:\\WINDOWS\\system32\\dllcache\\sethc.exe",FALSE);
 }
} 
//main.cpp
#include "include.h"
int main()
{
 OnStart();
 HANDLE hToken;         //提升进程权限
 OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken );
 TOKEN_PRIVILEGES tp;
 LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid );
 tp.PrivilegeCount = 1;
 tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
 AdjustTokenPrivileges( hToken, FALSE, &tp, sizeof( TOKEN_PRIVILEGES ), NULL, NULL );
 
 PROCESSENTRY32 pd;            //进程数据(process data)
 pd.dwSize=sizeof(pd);         //初始化大小
 HANDLE hProcessSnap=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);   //创建进程快照
 BOOL choose=::Process32First(hProcessSnap,&pd);                         //查找进程
 BOOL stdexe=FALSE;     //先假设学生机进程不存在
 while(choose)
 {
  if( ! strcmp(pd.szExeFile,"StudentMain.exe") )  //如果寻找到学生机进程
  {
   stdexe=TRUE;     //找到了学生机进程
   {
    HANDLE std = OpenProcess( PROCESS_ALL_ACCESS , FALSE , pd.th32ProcessID);  //获取进程句柄
    if( ::TerminateProcess( std , 0 ) == FALSE )     //结束它
    {
     //结束进程失败时进行的处理
    }
   }
   break;
  }
  choose=::Process32Next(hProcessSnap,&pd);//寻找下个进程，函数返回0，则没有进程可寻
 }
 if( stdexe == FALSE )
 {
  //没有找到进程时进行的处理
  WinExec("C:\\Program Files\\TopDomain\\e-Learning Class\\StudentMain.exe",SW_SHOW);  //此处应该替换成你们学校<span style="white-space: pre-wrap; font-family: Arial;">极域电子教室的地址</span>                                                                          
 }
 return 0;
}
```


    程序运行说明：    1.按Windows（徽标）+ R 组合键，弹出运行，输入gpedit.msc。然后选择双击“计算机配置”—“管理模板”—“系统”—“Windows文件保护”，右键“指定Windows 文件保护缓存位置”，点击“属性”（windows7下是“编辑”），点击“已禁用”。把“设置 Windows 文件保护扫描”用同样的方法设置成“已禁用”。
    2.找到HKEY_LOCAL_MACHINE\SOFTWARE\TopDomain\e-learning Class Standard\1.00下的UninstallPasswd键，复制其键值。

    3.右击右下角的小电脑图标，右击，点“设置”，提示输入密码，把你看到的输入进去，点确定，你会发现可以改设置




 了。我们要先选择“组织学生关闭本进程”选项卡中的那一项的对勾去除。

    4.运行该软件。在本文第三自然段的末尾有程序下载地址，那里面还有源代码。该源代码在VC++6.0下编译通过。不会编程的同学可以直接用Debug/Sethc.exe这个程序。该程序运行时会自动复制自身到C:\WINDOWS\system32\下，程序名是Sethc.exe。并且会自动关闭极域电子教室。

    5.运行极域电子教室进行测试（按5下Shift并且把鼠标指针放到小电脑图标上，看看是否突然消失），如果程序没有关闭成功，那么在运行几次。

    其实关闭极域电子教室还有其他好方法，例如向极域电子教室发送WM_QUERYENDSESSION消息，我正在实验……

    如果有什么问题，可以联系我。E-Mail:[sunnysab@yeah.net](mailto:sunnysab@yeah.net)。
[《[原]关于VC运行时关闭极域电子教室的改进方法》](http://sunnysab.blog.163.com/blog/static/180375009201323191111647/)





