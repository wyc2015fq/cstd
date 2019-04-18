# 批处理隐藏打开IE - ljx0305的专栏 - CSDN博客
2010年06月25日 15:07:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3593
bat 
复制内容到剪贴板 
代码:
mshta vbscript:CreateObject("WScript.Shell").Run("iexplore http://bbs.verybat.org",0)(window.close)
vbs 
复制内容到剪贴板 
代码:
CreateObject("WScript.Shell").Run "iexplore http://bbs.verybat.org", 0
复制内容到剪贴板 
代码:
Set objIE=CreateObject("InternetExplorer.Application")
objIE.Navigate "http://bbs.verybat.org"
objIE.Visible = false
//把如下红色字保存到 "文件名.bat"文件中就可以实现隐藏IE访问地址
@echo off
mshta vbscript:CreateObject("WScript.Shell").Run("iexplore http://172.20.2.175:8080/cps/install.action?srcNo=CNFOL&srcNo2=A",0)(window.close)
exit
VBS的RUN函数本身带三个参数-隐藏后台运行模式 
1.代码事例：
DIM objShell 
set objShell=wscript.createObject("wscript.shell") 
iReturn=objShell.Run("cmd.exe /C C:/addolcli/AddolC.exe", 0, TRUE) 
VBS的RUN函数本身带三个参数，第一个参数就是你要运行的程序 
第二个参数就是用来控制运行窗口模式 有0-9模式 
其中0是隐藏后台运行，6是最小化运行 
具体可以参数MSDN帮助文档 
SW_HIDE 0 Hides the window and activates another window. 
SW_MINIMIZE 6 Minimizes the specified window and activates the next top-level window in the Z order. 
SW_RESTORE 9 Activates and displays the window. If the window is minimized or maximized, Windows restores it to its original size and position. An application should specify this flag when restoring a minimized window. 
Run的第三个参数将允许我们防止相关程序以并发方式运行。您可将第三个参数赋值为True，以便指示脚本停留在使用Run方法的代码行并保持等待状态，直到已被激活的程序运行完毕。我们已经知道了因未设定该参数所产生的后果，该参数在缺省状态下被设定为False，而这种设置必将导致两个程序同时运行。也就是说TRUE值时必须等待这个脚本完成才运行其他程序，而false时可能还未运行完又执行另外一个脚本 
还有一种方式的隐藏运行 
set ws=wscript.createobject("wscript.shell") 
ws.run "bat.bat /start",0 
将上面代码写在记事本里保存扩展名为**.vbs 。 
以下是调用的批处理。即是上面中的bat.bat文件。 
@ECHO OFF 
C:/addolcli/AddolC.exe 
@ECHO OFF
object.Run(strCommand, [intWindowStyle], [bWaitOnReturn]) 
参数
object 
WshShell 对象。 
strCommand 
表示要运行的命令行的字符串值。必须包括要传递到可执行文件的所有参数。 
intWindowStyle 
可选。表示程序窗口外观的整数值。请注意，并非所有程序都使用此信息。 
bWaitOnReturn 
可选。布尔值，表示在继续执行脚本中的下一条语句之前，脚本是否等待执行完程序。如果设为 true，则在执行完程序后才执行脚本，Run 方法返回由程序返回的任何错误代码。如果设为 false（默认值），则 Run 方法将自动在启动程序后立即返回 0（不是错误代码）。 
说明
Run 方法返回一个整数。Run 方法启动在新 Windows 进程中运行的程序。可以让脚本等到程序执行完后再继续执行。这允许您同步运行脚本和程序。strCommand 参数内的环境变量自动扩展。如果某个文件类型已正确注册到某个程序中，则对该类型的文件调用 Run 方法时将执行该程序。例如，如果您的计算机系统中装有 Word，则对 *.doc 文件调用 Run 方法时将启动 Word 并加载该文档。下表列出了 intWindowStyle 的可用设置。
intWindowStyle 说明 
0 隐藏一个窗口并激活另一个窗口。 
1 激活并显示窗口。如果窗口处于最小化或最大化状态，则系统将其还原到原始大小和位置。第一次显示该窗口时，应用程序应指定此标志。 
2 激活窗口并将其显示为最小化窗口。 
3 激活窗口并将其显示为最大化窗口。 
4 按最近的窗口大小和位置显示窗口。活动窗口保持活动状态。 
5 激活窗口并按当前的大小和位置显示它。 
6 最小化指定的窗口，并按照 Z 顺序激活下一个顶部窗口。 
7 将窗口显示为最小化窗口。活动窗口保持活动状态。 
8 将窗口显示为当前状态。活动窗口保持活动状态。 
9 激活并显示窗口。如果窗口处于最小化或最大化状态，则系统将其还原到原始大小和位置。还原最小化窗口时，应用程序应指定此标志。 
10 根据启动应用程序的程序状态来设置显示状态。 
引用：[http://hi.baidu.com/fhqswcan/blog/item/7064a31e76421a1e403417e7.html](http://hi.baidu.com/fhqswcan/blog/item/7064a31e76421a1e403417e7.html)
WScript.Shell对象提供了run和exec两个方法，最近项目中需要在vbscript脚本中用pscp从linux服务器上copy文件。本来使用了exec方法，以下代码
Set exeRs = objws.Exec("pscp -r -scp -pw " & password & " " & loginid & "@" & host & ":" & data_path & fileName & " " & windows_path)
<!--[if !supportLineBreakNewLine]-->
<!--[endif]-->
但在测试的时候，当拷贝的文件比较大，拷贝时间较长的时候就出现cmd窗口死在那里不动了的情况，直接在cmd窗口中运行pscp拷贝相同的文件就可以正常 结束。也找不到是什么原因，于是改成run方法试一下，结果run方法是可以正常结束的，但是原来的代码中有需要取得cmd的控制台输出信息，而run却 不方便取得控制台信息，最后只能先把控制台信息重定向到文件中，然后程序在读取。最后说一下对run和exec两个方法的区别的一些理解：
这两个方法的声明如下
<!--[if !supportLineBreakNewLine]-->
<!--[endif]-->
Function Exec(ByVal Command As String) As WshExec
Function Run(ByVal Command As String, [ByVal WindowStyle], [ByVal WaitOnReturn]) As Integer
可以看出几点区别：
1，run的返回值是一个整数，就是0或1成功和失败两个状态，而exec方法的返回值是一个对象，从返回对象中可以获得控制台输出信息和控制台错误信息，即StdOut和StdErr属性等。例如：
Set exeRs = objws.Exec("pscp -r -scp -pw 。。。。"
errMsg = exeRs.StdErr.ReadAll()
stdMsg = oExec.StdOut.ReadAll（）
<!--[if !supportLineBreakNewLine]-->
<!--[endif]-->
可以取道控制台错误和控制台信息。
<!--[if !supportLineBreakNewLine]-->
<!--[endif]-->
2，Run 的后两个参数，一个是cmd窗口的风格，一个是是否等待执行完成。最后一个参数很有用，如果你希望等待本次cmd执行的程序结束后，在执行 objws.Exec后面的语句的话，只要设置这个参数为true就可以了，否则后面的语句将不等待cmd窗口完成，直接运行（我们项目就有这个需求，从服务器上拷贝一个文件到本地后，马上要打开这个文件操作，如果文件没有拷贝完就去操作就会出错的。）。另外，如果你使用exec 方法的时候，如果希望等待cmd中程序执行完后，在执行后面的语句，也可以通过下面的方法：
oExec.StdErr.ReadAll()或者oExec.StdOut.ReadAll（），道理上也应该好理解，要得到输出的信息，肯定要cmd执行完后才会有输出的。
参数的详细信息可以参考msdn
[http://msdn.microsoft.com/zh-cn/library/ateytk4a(en-us,VS.85).aspx](http://msdn.microsoft.com/zh-cn/library/ateytk4a(en-us,VS.85).aspx)
[http://msdn.microsoft.com/zh-cn/library/d5fk67ky(en-us,VS.85).aspx](http://msdn.microsoft.com/zh-cn/library/d5fk67ky(en-us,VS.85).aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/zmxj/archive/2009/02/25/3937372.aspx](http://blog.csdn.net/zmxj/archive/2009/02/25/3937372.aspx)
