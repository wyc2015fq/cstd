# 如何诊断Windows CE的应用程序崩溃(续) (转载) - xqhrs232的专栏 - CSDN博客
2009年11月12日 18:42:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：780
 在《如何诊断Windows CE的应用程序崩溃》一文中我介绍了如何利用Remote Process Explorer的Crack Address功能定位程序的崩溃地址。但是看起来还是有很多人不知道怎么使用这个工具，这篇文章详细介绍一下。
首先要确保你的PC上有dbghelp.dll。微软没有公开PDB文件格式，不过有解析PDB文件的API，在dbghelp.dll中提供，最新版本可在微软网站上下载。在微软推出.NET Framework之前，基于COM组件编程很流行，因此微软也以COM组件的形式提供了另外一组接口，叫DIA（Debug Interface Access）。Remote Proecess Explorer用dbghelp.dll解析WinCE调试符号，由于WinCE的可执行程序模块使用PE格式，调试符号文件使用PDB格式，和
 Windows桌面平台一样。因此它也可以分析Windows桌面应用程序地址。dbghelp.dll可以放到系统目录（c:/windows /system32），或者放到remote process explorer所在目录。
其次应该把EXE/DLL和PDB放在一个目录。如果放在不同目录，dbghelp可能会找不到PDB。
具体用这个工具有几步。举个例子来说明，比如要定位下面的崩溃地址PC=03f7e034：
RaiseException: Thread=8d5108b0 Proc=81a37040 'device.exe'
AKY=00000205 PC=03f7e034(coredll.dll+0x0001e034) RA=80120ef4(NK.EXE+0x00020ef4) BVA=00000001 FSR=00000001
从PC=03f7e034(coredll.dll+0x0001e034) 这句我们知道三个重要信息：崩溃模块coredll.dll，崩溃地址0x03f7e034，模块加载基地址0x03f7e034-0x0001e034=0x03f60000。CE5以前的版本不会显示(coredll.dll+0x0001e034)，基地址不能直接算出来。不过一般来说这些DLL如果你的系统没有做过很大改变，每次加载的基地址是一样的，所以你可以把系统运行起来查看它的加载基地址。
根据这三条信息Remote Process Explorer就可以工作了：
1，Cracking Address填崩溃地址：0x03f7e034
2，base address填基地址：0x03f60000
3，模块路径栏填coredll.dll的全路径，可以用右面的按钮定位。 
![](http://images.cnblogs.com/cnblogs_com/singlerace/screenshot.73.png)
现在可以试着按第三行的寻找按钮（有一个放大镜的图标）。顺利的话应该就能提示源文件路径、函数名和代码行。在设当设置了调试符号搜索路径和源文件搜索路径后，下面的文本框还能直接显示源代码，崩溃行用红色标出。搜索路径在"File --> File Path..."对话框里设置（多个路径用分号;分隔）：
![](http://images.cnblogs.com/cnblogs_com/singlerace/screenshot.74.png)
由于生成PDB时的源文件路径和我们安装Platform Builder的WINCE路径肯定不一样，因此最下面的"Source Path Mapping"还要做一些路径映射，方便Remote Process Explorer搜索源代码。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/singlerace/archive/2009/02/26/3940619.aspx](http://blog.csdn.net/singlerace/archive/2009/02/26/3940619.aspx)
