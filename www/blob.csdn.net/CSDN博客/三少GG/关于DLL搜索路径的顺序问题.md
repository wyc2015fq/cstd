# 关于DLL搜索路径的顺序问题 - 三少GG - CSDN博客
2012年04月06日 14:24:53[三少GG](https://me.csdn.net/scut1135)阅读数：778标签：[dll																[windows																[path																[system																[search																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=search&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
作者：朱金灿
来源：[http://www.cnblogs.com/clever101](http://www.cnblogs.com/clever101)**还不错的博客，可多关注！有面试相关~**
    DLL的动态链接有两种方法。一种是加载时动态链接(Load_time dynamic linking)。Windows搜索要装入的DLL时，按以下顺序：应用程序所在目录→当前目录→Windows SYSTEM目录→Windows目录→PATH环境变量指定的路径。
前天看到这几句，突然设计出一道自认绝妙的笔试题：
“如果采用加载时动态链接的方式，Windows搜索要装入的DLL采用怎样的顺序？”
这个是基础题，估计你很容易答出（答案就是上面的）。呵呵，我还有后着呢：
“你是如何证明Windows搜索要装入的DLL遵循这样的顺序呢，说出你的证明步骤”
你可以思考一下。下面是我设想的一个方案，但是自己测试了一下。结果却令人吃惊。
我的证明步骤是这样的：
1.首先新建一个MFC 常规DLL工程OutputModulePath，在里面添加一个API函数，功能就是打印DLL所在路径，然后将其导出，代码如下：
```
void PaintModulePath()
{
    extern COutputModulePathApp theApp;
    TCHAR szModulePath[MAX_PATH];
    :: GetModuleFileName(theApp.m_hInstance,szModulePath, MAX_PATH);
    AfxMessageBox(szModulePath);
}
[复制代码](http://www.cnblogs.com/clever101/archive/2010/05/05/1728435.html)
```
2.编译这个DLL工程，然后将生成的DLL文件OutputModulePath.dll复制到当前目录、Windows SYSTEM目录→Windows目录→PATH环境变量指定的路径。这里要说明一下：如果调试运行的话，当前目录应该是当前目录的工程文件所在的路径，如果单击直接运行的话，当前目录应该是exe程序所在的路径，具体可以通过一下代码获取当前目录：
```
TCHAR szBuf[MAX_PATH];
    ::ZeroMemory(szBuf,MAX_PATH);
    if (::GetCurrentDirectory(MAX_PATH,szBuf) > 0)
    {
     //获取进程目录成功。
       AfxMessageBox(szBuf);
     }
[复制代码](http://www.cnblogs.com/clever101/archive/2010/05/05/1728435.html)
```
    PATH环境变量指定的路径这里可能有多个路径，这里只须将OutputModulePath.dll拷贝到其中之一就行，例如我拷贝到的是：D:\Program Files\Lua\5.1。
3.建一个测试工程（对话框程序或单文档程序都可以），在里面调用PaintModulePath函数。
我的测试步骤及结果是这样的，按F5调试运行测试程序，首先程序输出的是exe程序所在的路径，然后我将exe程序路径下的dll文件删除，但接着输出的是C:\WINDOWS\system\OutputModulePath.dll，我将C:\WINDOWS\system\OutputModulePath.dll删除，接着输出的是C:\WINDOWS\
 OutputModulePath.dll，将C:\WINDOWS\ OutputModulePath.dll删除，输出的才是当前目录下的文件路径，最后输出的是PATH环境变量指定的路径。
这样的结果和书上的理论不符。难道我的测试方案有什么不对吗？
到论坛一问，得到一个答案是微软上的一篇文章：
[Dynamic-Link Library Search Order](http://msdn.microsoft.com/en-us/library/ms682586%28VS.85%29.aspx)
现在把它翻译一下：
动态链接库的搜索顺序
一个系统可以包含多个版本的同一个动态链接库(dll)。应用程序能够通过使用[动态链接库重定向](http://msdn.microsoft.com/en-us/library/ms682600%28v=VS.85%29.aspx)或[清单文件](http://msdn.microsoft.com/en-us/library/aa375365%28v=VS.85%29.aspx)指定要加载的DLL的全路径。如果没有使用这些方法，这篇文章将讲述在装入DLL时DLL的搜索顺序。
**标准的搜索顺序**
    DLL的搜索顺序取决于是否安全DLL搜索模式是启用或禁用。
安全DLL搜索模式在默认状态下是启用的。通过创HKLM\System\CurrentControlSet\Control\Session Manager\SafeDllSearchMode注册表项并将它的值设为0可以关闭这个属性。通过调用SetDllDirectory函数可以有效禁用安全DLL搜索模式而在这篇文章中这个函数指定的路径将加入搜索范围并改变搜索顺序。
**Windows XP and Windows 2000 with SP4:  **安全DLL搜索模式在默认状态下是禁用的。通过创HKLM\System\CurrentControlSet\Control\Session
 Manager\SafeDllSearchMode注册表项并将它的值设为1可以启用这个属性。安全DLL搜索模式在默认状态下得到启用始于带sp2的Windows XP。
**Windows 2000:  并不支持**安全DLL搜索模式。在这种情况下DLL的搜索顺序和安全DLL搜索模式被禁用的情况下的顺序是一样的。安全DLL搜索模式得到支持始于带sp4的Windows
 2000。
假如安全DLL搜索模式启用，搜索顺序如下：
1. 应用程序所在的路径
2. Windows SYSTEM目录。通过调用GetSystemDirectory函数可以获取这个目录的路径。
3. 16位系统的目录。并没有函数可以获取这个目录的路径，但是它会被查找。
4. Windows目录。通过调用GetWindowsDirectory函数可以获取这个目录的路径。
5. 当前目录
6. PATH环境变量指定的路径。请注意，这并不包括每个应用程序的应用程序路径注册表项中指定。在应用程序路径注册表项的键值并不作为DLL的搜索路径。
假如安全DLL搜索模式禁用，搜索顺序如下：
1. 应用程序所在的路径
2. 当前目录
3. Windows SYSTEM目录。通过调用GetSystemDirectory函数可以获取这个目录的路径。
4. 16位系统的目录。并没有函数可以获取这个目录的路径，但是它会被查找。
5. Windows目录。通过调用GetWindowsDirectory函数可以获取这个目录的路径。
6. PATH环境变量指定的路径。请注意，这并不包括每个应用程序的应用程序路径注册表项中指定。在应用程序路径注册表项的键值并不作为DLL的搜索路径。
**预备的搜索顺序**
由系统指定的标准搜索顺序可以通过调用LoadLibraryEx函数加上LOAD_WITH_ALTERED_SEARCH_PATH参数值得到改变。标准搜索顺序也可以通过调用SetDllDirectory函数得到改变。
**Windows XP:  通过**调用SetDllDirectory函数来改变标准搜索顺序并不支持直到Windows
 XP sp1出现。
**Windows 2000: **不支持**通过**调用SetDllDirectory函数来改变标准搜索顺序。
如果您指定一个备用的搜索顺序，程序将按备用的搜索顺序进行搜索，直到所有相关的可执行模块被找到。系统启动后，DLL初始化例程处理，该系统将恢复为标准的搜索顺序。
LoadLibraryEx函数通过指定LOAD_WITH_ALTERED_SEARCH_PATH属性和lpFileName参数指定一个绝对路径支持一个预备的搜索顺序。
请注意：标准搜索顺序和通过调用指定LOAD_WITH_ALTERED_SEARCH_PATH属性的LoadLibraryEx函数来设置的预备搜索顺序只是有一点不同：标准搜索顺序开始于搜索1. 应用程序所在的路径而预备搜索顺序开始于LoadLibraryEx函数所要加载的可执行模块的所在目录。
假如安全DLL搜索模式启用，搜索顺序如下：
1. lpFileName参数值所指定的目录
2. Windows SYSTEM目录。通过调用GetSystemDirectory函数可以获取这个目录的路径。
3. 16位系统的目录。并没有函数可以获取这个目录的路径，但是它会被查找。
4. Windows目录。通过调用GetWindowsDirectory函数可以获取这个目录的路径。
5. 当前目录
6. PATH环境变量指定的路径。请注意，这并不包括每个应用程序的应用程序路径注册表项中指定。在应用程序路径注册表项的键值并不作为DLL的搜索路径。
假如安全DLL搜索模式禁用，搜索顺序如下：
1. lpFileName参数值所指定的目录
2. 当前目录
3. Windows SYSTEM目录。通过调用GetSystemDirectory函数可以获取这个目录的路径。
4. 16位系统的目录。并没有函数可以获取这个目录的路径，但是它会被查找。
5. Windows目录。通过调用GetWindowsDirectory函数可以获取这个目录的路径。
6. PATH环境变量指定的路径。请注意，这并不包括每个应用程序的应用程序路径注册表项中指定。在应用程序路径注册表项的键值并不作为DLL的搜索路径。
假如lpPathName参数指定了一个路径，SetDllDirectory函数支持一个预备的搜索顺序。这个预备的搜索顺序如下：
1. 应用程序所在的路径
2. lpPathName参数指定的目录
3. Windows SYSTEM目录。通过调用GetSystemDirectory函数可以获取这个目录的路径。
4. 16位系统的目录。并没有函数可以获取这个目录的路径，但是它会被查找。
5. Windows目录。通过调用GetWindowsDirectory函数可以获取这个目录的路径。
6. PATH环境变量指定的路径。请注意，这并不包括每个应用程序的应用程序路径注册表项中指定。在应用程序路径注册表项的键值并不作为DLL的搜索路径。
如果lpPathName参数为一个空字符串，当前目录将会从搜索顺序中删除。
SetDllDirectory 有效地禁用安全DLL搜索模式，而在搜索指定的目录路径。要恢复安全 DLL搜索模式的SafeDllSearchMode注册表值的基础和恢复当前目录到搜索顺序，调用 lpPathName的参数值为NULL的SetDllDirectory函数。
看完这篇文章，我大致知道了我的测试为何会出现那个结果，因为我的操作系统环境是Win XP + sp3。
参考文献：
1. [MFC深入浅出](http://www.vczx.com/tutorial/mfc/mfc.php)
