# Wince下面添加应用程序，并且自动运行----几种方式介绍 - xqhrs232的专栏 - CSDN博客
2013年08月24日 23:40:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：953
原文地址::[http://www.cnblogs.com/xilentz/archive/2010/05/31/1747930.html](http://www.cnblogs.com/xilentz/archive/2010/05/31/1747930.html)
Wince下面添加应用程序，并且自动运行
添加快捷方式：
1、复制可执行文件到BSP（$(_FLATRELEASEDIR)\PLATFORM\SMDK2410）的file目录中，在目录中创建连接文件.lnk  
创建.lnk的方法：新建.txt文件，在文件中添加21#/windows/*.exe，然后保存，重新命名为.lnk就可以了，”#”前面的21，我看了一些资料，都说是表示“#”后面的字符数，但是这个字符数与后面字符个数不匹配的时候也能正常运行。   (示例：使用记事本打开FILE文件夹下面的一个快捷方式查看)
  2、在Platform.bib中添加 （在PB工作窗口ParameterView中Hardware Specific Files 下）
    .exe        $(_FLATRELEASEDIR)\.exe   NK   U  
    .lnk         $(_FLATRELEASEDIR)\.lnk   NK   U  
  3、在Platform.dat中添加：
    Directory(LOC_DIRWINDOWSSTARTUP):-File(".lnk",   "\windows\.lnk")  
1) 假定Windows CE.NET目标工程目录为D:\WINCE420\PUBLIC\SJPG2440，并且工程已经Build成功，假定Windows CE.NET的应用为MyApp.exe；
2) 将应用程序MyApp.exe复制到SJPG2440工程目录
D:\WINCE420\PUBLIC\SJPG2440\RelDir\SAMSUNG_SMDK2440_ARMV4IRelease 下
3) 修改SJPG2440工程的project.bib文件，在FILES Section添加如下内容：
MyApp.exe $(_FLATRELEASEDIR)\MyApp.exe NK H
4) 创建快捷方式文件MyApp.lnk，文件内容如下：
10#\Windows\MyApp.exe
将MyApp.lnk文件也放入到SJPG2440工程目录
D:\WINCE420\PUBLIC\SJPG2440\RelDir\SAMSUNG_SMDK2440_ARMV4IRelease下
5) 修改SJPG2440工程的project.bib文件，在FILES Section添加如下内容：
MyApp.lnk $(_FLATRELEASEDIR)\MyApp.lnk NK H
6) 修改MyWinCE工程的project.dat文件，添加如下内容：
Directory("\Windows\Startup"):-File("MyApp.lnk","\Windows\MyApp.lnk")
7) 修改MyWinCE工程的platform.bib文件，在FILES Section添加如下内容：
MyApp.exe $(_FLATRELEASEDIR)\MyApp.exe NK H
MyApp.lnk $(_FLATRELEASEDIR)\MyApp.lnk NK H
8) Platform Builder IDE：【Build】->【Make Image】（记得千万不要Build或者Rebuild）
9) 成功后，得到的NK.bin（或NK.nb0）就包含了应用程序MyApp.exe和MyApp.lnk，当把相应的内核烧入开发板MyApp程序就会在系统启动时自动运行。
修改注册表方法：
  Windows   CE启动时分别执行[HKEY_LOCAL_MACHINE\init]键下所有子键列出的程序。例如：
[HKEY_LOCAL_MACHINE\init]           
"Launch20"＝"device.exe"            
"Launch30"＝"gwes.exe"           
"Depend30"＝hex：14,00  
一般的按上面第二种方法将注册表中的   "Launch50"＝"explore.exe"中的explore.exe改成你自己的应用程序就可以了,这样就能够不进入CE的那个桌面,而直接进入你的应用~注意一下你的应用程序的路径就可以了!  
    “Launch50"＝"explore.exe”是在shell.reg中
1) 首先新建工程，把你的应用程序放到内核里面去（前面介绍了如何做）；
2) 编译平台；
3)修改
D:\WINCE420\PUBLIC\SJPG2440\RelDir\SAMSUNG_SMDK2440_ARMV4IRelease下的shell.reg的文件：
在文件中有这样一行：
[HKEY_LOCAL_MACHINE\init]
"Launch50"="explorer.exe"
"Depend50"=hex:14,00, 1e,00
修改如下：
[HKEY_LOCAL_MACHINE\init]
"Launch40"=" MyApp.exe"
"Depend40"=hex:14,00, 1e,00
"Launch50"="explorer.exe"
"Depend50"=hex:14,00, 1e,00
4) Platform Builder IDE：【Build】->【Make Image】（记得千万不要Build或者Rebuild）
5) 成功后，得到的NK.bin（或NK.nb0）就包含了应用程序MyApp.exe和MyApp.lnk，当把相应的内核烧入开发板MyApp程序就会在系统启动时自动运行。
编写shell应用程序
以上方法都可行，但是都存在一个问题，就是应用程序是集成到NK里面的，也就是说每次升级应用程序都要重新编译下载内核，很麻烦，尤其在程序调试阶段，大家都希望把应用程序放在SD卡上，这样更新起来比较容易；据说通过第三种方法可以实现，即修改"Launch80"="MyApp.exe"为"Launch80"="\STDCard\MyApp.exe"( STDCard为SD卡目录)，但是我试了一下没有成功，因为Launch80运行时SD卡的文件驱动还没有加载，找不到MyApp.exe文件。同样，采用快捷方式加载SD卡里的应用程序也不可行。这里又一个新的办法，比如叫ShellExe.exe，将此程序加入到映像里，通过StartUp快捷方式调用ShellExe，ShellExe再去调用SD卡里的应用程序，具体实现步骤如下： 
1、 在eVC下编译如下代码：
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
         WIN32_FIND_DATA fd;
         HANDLE hd=INVALID_HANDLE_VALUE;
         int iCount = 20;
         while(iCount--)
         {
                   hd=::FindFirstFile(lpCmdLine,&fd);
                   Sleep(500);
                   if(INVALID_HANDLE_VALUE!=hd) break;
         }
         if(0==iCount) return 0;
         FindClose(hd);
         SHELLEXECUTEINFO ShExeInfo={0};
         ShExeInfo.cbSize=sizeof(SHELLEXECUTEINFO);
         ShExeInfo.fMask=SEE_MASK_NOCLOSEPROCESS;
         ShExeInfo.hwnd=NULL;
         ShExeInfo.lpVerb=NULL;
         ShExeInfo.lpFile=lpCmdLine;
         ShExeInfo.lpParameters=L"";
         ShExeInfo.lpDirectory=NULL;
         ShExeInfo.nShow=SW_SHOW;
         ShExeInfo.hInstApp=NULL;
         ShellExecuteEx(&ShExeInfo);
         return 0;
}
生成ShellExe.exe的可执行文件，此段代码主要功能是查找指定的应用程序，然后执行；下面这段代码可以保证在SD卡文件系统正确加载后才去执行应用程序；
while(iCount--)
{
         hd=::FindFirstFile(lpCmdLine,&fd);
         Sleep(500);
         if(INVALID_HANDLE_VALUE!=hd) break;
}
文件的名称和路径由命令行参数指定：
ShExeInfo.lpFile=lpCmdLine;
2、 新建一个快捷方式，如Autorun.lnk，按如下方式编辑其内容：
21#\windows\shellexe.exe \stdcard\MyApp.exe
其中\stdcard\MyApp.exe应用程序的绝对路径；
3、 将MyApp.exe和Autorun.lnk添加到NK里，方法是在project.bib文件内加入如下内容：
ShellExe.exe f:\WINCE420\PBWORKSPACES\LioetEnTer\RelDir\ShellExe.exe NK S
Autorun.lnk f:\WINCE420\PBWORKSPACES\LioetEnTer\RelDir\Autorun.lnk NK S
注意：ShellExe.exe的属性不能带H（隐藏）.
4、 在project.dat里加入如下内容：
Directory("\Windows\Startup"):-File("Autorun.lnk","\Windows\Autorun.lnk")
5、 选择Make Image生成映像（当然Build也可以，就是慢点儿），烧到FLASH里，开机运行，可以看到SD卡里的MyApp.exe被正确执行。
总结
这种方法用起来比较方便，ShellExe.exe不用每次都重新编译，只要根据应用程序路径修改Autorun.lnk即可，可以加载Flash、U盘、SD卡里的应用程序。调试及升级应用程序就不用重新烧写内核了。
开机后直接运行您的程序而不显示Windows CE桌面
首先编译你的平台，在RELEASEDIR里面修改shell.reg的文件 ： 
在文件中有这样一行： 
[HKEY_LOCAL_MACHINE\init] 
"Launch50"="explorer.exe" 
"Depend50"=hex:14,00, 1e,00 
把这个explorer.exe改成你的应用程序（比如：MyApp.exe）就可以了。
这里解释一下Launch后面的数字越小，该程序执行的越早（优先级越高）
Depend后面的字符串hex:14,00, 1e,00，表示该应用程序需要依赖其他的应用程序，也就是说必须要如字符串所对应的其他应用程序执行完以后这个程序才能执行。
