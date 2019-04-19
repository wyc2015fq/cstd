# DirectX(June 2010)时遇到Error Code s1023 - 三少GG - CSDN博客
2013年01月31日 18:53:19[三少GG](https://me.csdn.net/scut1135)阅读数：1639
## 安装DirectX时遇到Error Code s1023
向开发DirectX 3D游戏！就安装DirectX最新版！！安装时遇到Error Code s1023![](http://b.hiphotos.baidu.com/album/pic/item/314e251f95cad1c8564d01cc7f3e6709c83d5163.jpg)
试了好多办法都不行！！关掉所有程序还是不行！！感谢伟大的Google在一个英文网站上提到如果安装了Microsoft  Visual C++ 2010 x86/x64 Redistributable 的版本大于10.0.30319 
 就会提示失败！将这个卸载掉就可以安装成功！！
总是在最后配置Visual C++ Runtime的时候出现S1023这个错误
上网找了一大圈，最后还是在stack overflow上找到：
地址是：[http://stackoverflow.com/questions/4102259/directx-sdk-june-2010-installation-problems-error-code-](http://stackoverflow.com/questions/4102259/directx-sdk-june-2010-installation-problems-error-code-s1023)[s1023](http://stackoverflow.com/questions/4102259/directx-sdk-june-2010-installation-problems-error-code-s1023)
里面大致是说（这里直说Win7的，XP的话应该换了）
已在此计算机上检测到 Microsoft Visual C++ 2010 Redistributable 的更新版本。
这就证明你的机器之前就已经有装过一个工具了
4、将上面多安装的工具，在控制面板里面删除就可以了，这个工具的名字大概是：
Microsoft Visual C++ 2010 x86/x64redistribuable - 10.0.(number over 30319)
删除再安装一次应该就可以了！
[DirectX( June 2010 ) Error Code S1023](http://blog.csdn.net/coolman2007/article/details/7540116)
在Win7上面安装DirectX（June 2010）版本的时候，到了最后，遇到了一个情况，就是出现了Setup错误，错误代码是S1023，搜索了一下，得到如下的结果：
按照以下步骤来解决，就可以确保安装不会出现该问题
(1) Remove the *Visual C++ 2010 Redistributable Package* version 10.0.40219 (Service Pack 1) from the system (both x86 and x64 if applicable). This can be easily done via a command-line with administrator rights:
MsiExec.exe /passive /X{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}
MsiExec.exe /passive /X{1D8E6291-B0D5-35EC-8441-6616F567A0F7}
(2) Install the DirectX SDK (June 2010)
(3) Reinstall the *Visual C++ 2010 Redistributable Package* version 10.0.40219 (Service Pack 1). On an x64 system, you should install both the x86 and x64 versions of the C++ REDIST. Be sure to install the most[current
 version available](http://www.microsoft.com/download/en/details.aspx?displaylang=en&id=26999), which at this point is the [KB 2565063](http://support.microsoft.com/kb/2565063) with a security fix.
具体可参考[http://blogs.msdn.com/b/chuckw/archive/2011/12/09/known-issue-directx-sdk-june-2010-setup-and-the-s1023-error.aspx](http://blogs.msdn.com/b/chuckw/archive/2011/12/09/known-issue-directx-sdk-june-2010-setup-and-the-s1023-error.aspx)
