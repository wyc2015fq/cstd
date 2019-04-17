# 如何生成转储(dmp)文件--工具篇 - DoubleLi - 博客园






# 1. 任务管理器

任务管理器可以说是最易获取的系统工具，同时它具有生成转储文件的功能。但要注意的是在64位操作系统上面，默认启动的是64位的任务管理器。使用任务管理器生成转储文件需要遵循一个原则：用32位任务管理器给32位进程（无论该进程是运行在32位还是64位系统上面）生成转储文件，用64位任务管理器给64位进程生成转储文件。在64位系统上，32位的任务管理器位于`C:\Windows\SysWOW64\taskmgr.exe`。

生成方法：右键进程 --> 创建转储文件-->弹出对话框提示生成成功，以及dmp文件位置。
![](https://images2017.cnblogs.com/blog/214032/201709/214032-20170905135057226-1017780395.jpg)

> 
类似的工具还有：Process Explorer，PCHunter等。


# 2. 注册表

可以通过在注册表中进行配置，让操作系统在程序崩溃时自动生成dmp文件，并放到指定位置。
在注册表项`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps`下面根据进程名（含.exe）新建子项，并配置如下值：
名称：`DumpCount`，类型：`REG_DWORD`，最大保留Dump个数，默认为10.
名称：`DumpType`，类型：`REG_DWORD`，Dump类型(1-Mini dump, 2-Full dump)，默认为1.
名称：`DumpFolder`，类型：`REG_EXPAND_SZ`，Dump文件保存的位置。

![注册表](https://images2017.cnblogs.com/blog/214032/201709/214032-20170905135113679-704636339.jpg)

# 3. Windbg

     Windbg这么强大的调试工具当然是可以生成转储文件的，何止生成转储文件，它能做的更多。
生成方法：File-->Attach to Process-->输入`.dump /ma /u d:\test.dmp` 。
提示成功之后，可以在D盘看到生成dmp文件到test_0bf0_2017-08-13_23-46-37-244_11cc.dmp文件。
0bf0_2017-08-13_23-46-37-244_11cc是`/u`参数附加上去的，意思是2017年08月13日 23时46分37秒244毫秒，进程PID位11cc。
.dump命令参数比较多，常用的组合就是/ma，`/m`表示生成minidump，`/a`表示dmp包含所有信息，`/u`参数就是上面说的附加时间和PID信息到文件名。

![图例](https://images2017.cnblogs.com/blog/214032/201708/214032-20170823143847543-77774602.png)

# 4. Windbg -I

`Windbg -I` 可以将Windbg设置为及时调试器（开启了UAC的系统上面，需要以管理员权限运行），也就是我们常说的JIT调试器。设置成功之后，如遇到程序崩溃，Windbg会自动运行并附加到崩溃进程。
![](https://images2017.cnblogs.com/blog/214032/201709/214032-20170905135143976-2004291521.jpg)

设置成功之后会弹出对话框。如果不想弹出对话框，可以加上S（slient首字母）`Windbg -IS`.

也可以通过修改注册表项AeDebug来实现和`windbg -I`同样的功能。
根据windbg位数（32/64）和系统的位数（32/64）的不同，修改的注册表项的位置也不同：
32位windbg--32位系统：`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug`
32位windbg--64位系统：`HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows NT\CurrentVersion\AeDebug`
64位windbg--64位系统：`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug`

AeDebug项下面有2个值：
名称：`Auto`，类型：`REG_SZ`，0表示出现崩溃弹出对话框，让用户选择关闭程序还是调试程序；1表示自动弹出设置调试器。
名称：`Debugger`，类型：`REG_SZ`，调试器值。默认为`"C:\WINDOWS\system32\vsjitdebugger.exe" -p %ld -e %ld`，设置为windbg需更改为`"C:\Debuggers\WinDbg\x86\windbg.exe" -p %ld -e %ld -g`

看起来挺复杂，其实挺好理解的。

# 5. Adplus

adplus工具位于windbg安装目录，最早叫adplus.vbs，以VBScript脚本提供，最新版改成了adplus.exe。adplus.exe不仅可以在程序崩溃时手动运行来生成dmp文件，也可以在崩溃之前就运行它，当程序崩溃时它会自动生成dmp文件；甚至可以在程序没有运行之前就先运行adplus，当程序崩溃时它会自动生成dmp文件。
如：`adplus -pn powerpnt.exe -pn wincmd32.exe -hang -o c:\test`

## 5.1 adplus用法
`ADPlus <RunMode> -o <OutputDirectory> [Options]`
`RunMode`：-hang或-crash
`-hang` 附加到进程，生成dmp，然后解除附加(detach)。多用于程序卡死的情况下。
`-crash` 附加到进程，直到程序崩溃或者其他事件发生，生成dmp文件，然后解除附加。

## 5.2 常用参数（完整文档见[adplus.doc](https://files.cnblogs.com/files/jiangxueqiao/adplus.zip)文件）：

`-o 目录`
指定生成文件存储目录。

`-p 进程ID`
指定进程ID，可以同时使用多次-p来指定多个进程。

`-pn 进程名`
指定进程名，支持通配符，也可以同时使用多次-pn来指定多个进程，但进程名必须存在，不存在则失败。

`-po 进程名`
和-pn类似，但-po不要求进程名必须存在。可以在进程启动之前就先启动Adplus.

`-pmn 进程名`
pmn为Process Monitor缩写。顾名思义，可以监视进程列表，一旦指定进程运行，则附加上去。只适用于`-crash` 模式。





感谢您的阅读！三人行必有我师，欢迎对文章进行指正。









