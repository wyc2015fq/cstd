# VC++ 控制台程序不弹出窗口的方法 - 战斗蜗牛的专栏 - CSDN博客





2009年12月21日 18:35:00[vbskj](https://me.csdn.net/vbskj)阅读数：6703








调用FreeConsole();需要头文件windows.h,在程序开始时调用就行了。但是VC开发环境的控制台还是会有的，因为那是VC调的，不过你的程序不会输出到那个控制台罢了
[http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dllproc/base/freeconsole.asp](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dllproc/base/freeconsole.asp)

project->settings->link,category选output,在project options里把/subsystem:console改为/subsystem:windows.然后Entry-point symbol里填mainCRTStartup就行了 







转载：

大家都知道,当编写一个win32 console application时,当运行此类程序的时候默认情况下会有一个类似dos窗口的console窗口,但是有的时候我们只想在程序中运行一段功能代码,不希望显示这个console窗口,让代码执行完毕之后程序自动退出.下面就介绍一下,如何隐藏win32 console application的console窗口 
因为此种方法是通过设置编译器的链接开关来实现,所以让我们来看一下编译
器的链接开关选项(也就是linker选项).
首先我们来看一下linker的 /subsystem 选项
该选项的语法形式如下:
/subsystem:{console|efi_application|efi_boot_service_driver|
efi_rom|efi_runtime_driver|native|posix|windows|windowsce}
[,major[.minor]]
这个链接选项告诉操作系统如何运行可执行文件
console:
win32 字符模式应用程序,此种类型的应用程序在运行的时候会产生一个类似dos
窗口的控制台窗口,如果在应用程序的主函数为main()或者wmain(),在默认情况下
该应用程序就是一个控制台应用程序
extensible firmware interface
和cpu具体架构相关的一个参数选项,并不常用,在这里暂不详细介绍.
如果对此有兴趣的可以访问intel主页来查看相关内容
native;
设备驱动器选项,如果/driver:wdm选项被设定的话,该链接选项(native)就为默认选项
posix:
在windows nt 种运行在posix子系统上的应用程序
windows:
该类型的应用程序不产生console窗口,该类型的应用程序的窗口由用户自己创建,简而言之
就是一个标准的win32 application,其入口地址为winmain()函数或者wwinmain()函数的地址
如果你在应用程序种定义的主函数为winmain或者wwinmain,在默认情况下该应用程序就是一个
win32 application !
windowsce:
运行在windows ce上的应用程序
major and minor (optional):
主版本号和次版本号,该选项为可选,该选项为0~65535之间的十进制整数 
从上面可以看出如果我们建立一个win32 console application的话,linker的/subsystem选项应该为
console,可以在vc开发环境的project->setting->link->project option中看到!
接下来我们再看看应用程序是如何运行的!
我们知道用vc编写的程序,运行的时候是需要 cc 运行库支持的.当我们运行一个c/c 程序的时候
链接器会首先寻找应用程序的启动函数,例如:
如果你建立了一个console程序的话,编译器得链接开关会是以下这种形式
/subsystem:console /entry:maincrtstartup (ansi)
/subsystem:console /entry:wmaincrtstartuup (unicode)
如果你建立了一个win32 application,编译器得链接开关则会是一下形式
/subsystem:windows /entry:winmain (ansi)
/sbusystem:windows /entry:wwinmain (uincode)
上面的两种形式可以再project->setting->link->project option中看到
上面的subsystem和entry并不需要都设置,如果你只设置了/subsystem:console
的话,那么默认的entry开关在默认情况下应为/entry:maincrtstartup
反之,如果你在应用程序中定义了main函数的话,默认情况下,你的/subsystem开关
应该为/system:console
在默认情况下/subsystem 和/entry开关是匹配的,也就是
console对应maincrtstartup或者wmaincrtstartup
windows对应winmain或者wwinmain
但是我们也可以通过手动改动的方式使他们不匹配
例如我们可以这样改动
#pragma comment( linker, /subsystem:windows /entry:maincrtstartup ) // 设置入口地址 
int main(int argc, char* argv[])
{
messagebox(null, hello, notice, mb_ok);
return 0;
}
在默认情况下链接器看到/subsystem下是windows选项的时候,它会自动寻找winmain或者wwinmain
但我们强制指定入口地址,这样运行程序的时候默认的console窗口就会隐藏!
上面是在代码中使用#pragma指令来设置,还有一种就是直接在开发环境的
project->setting->link->project option中手工改动!
写了这么多,自己都有点感觉乱,没有办法,以前没写过什么文章,所以措辞可能不太好,希望大家见谅。
1:如果console程序已经写好了，不能改了，也可以。写一个API程序，不要画窗口，然后用CreateProcess调用写好的console程序，把属性设成SW_HIDE即可。
2:不能用控制台来写(CONSLOE),要用WINMAIN做入口就可以了,不画窗口,别人就都看不见了. 你只用想想办法把你的进程在任务栏里面隐藏住就可以.
3:如果是console程序，用API函数GetStdHandle()获得控制台程序的窗口句柄，然后在隐藏窗口
4://这一句隐藏控制台 
#pragma   comment(linker,   "/subsystem:/"windows/"   /entry:/"mainCRTStartup/""   ) 
你要写控制台程序就用这个
在控制台程序中隐藏控制台窗口!



