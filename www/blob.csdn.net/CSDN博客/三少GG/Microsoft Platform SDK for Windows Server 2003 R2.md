# Microsoft Platform SDK for Windows Server 2003 R2 - 三少GG - CSDN博客
2012年12月13日 16:57:50[三少GG](https://me.csdn.net/scut1135)阅读数：3162
### [关于Platform SDK和Windows SDK](http://blog.csdn.net/colinchan/article/details/5984629)
分类： [所有技术文章](http://blog.csdn.net/colinchan/article/category/221924)2010-11-03 13:57
792人阅读[评论](http://blog.csdn.net/colinchan/article/details/5984629#comments)(0)收藏[举报](http://blog.csdn.net/colinchan/article/details/5984629#report)
**问题1：什么是Windows SDK？**Windows SDK是用来支持32位和64位Windows平台以及.NET框架编程模型的开发包。Windows SDK这个名字从Windows Vista开始使用，它包含以前Platform SDK中的所有内容，同时包含.NET框架的内容。
**问题2：什么是Platform SDK？**
Platform SDK（也简写作PSDK）是在Windows Vista之前的Windows操作系统开发包。PSDK不包含任何.NET框架编程模型的内容。现在，Windows SDK已经取代了PSDK。PSDK不会再有更新版本发布，尽管一些PSDK仍然可以在微软的网站上下载到，或者通过CD/DVD的形式从[http://www.qmedia.ca/launch/psdk.htm](http://www.qmedia.ca/launch/psdk.htm)订购到。
**问题3：在什么情况下我还需要使用老的Platform SDK？**
你可能因为某些原因还是要去使用老的Platform SDK。比如，你还在使用VC 6进行开发，那你就要去用2003年2月份发布的Windows Server 2003 PSDK，这是最后一个对VC 6开发提供全面支持的SDK。
**问题4：Windows SDK只支持Windows Vista开发吗？**
（2006年发布了第一个版本的Windows SDK。）2007年3月份发布的Windows SDK Update for Windows Vista支持了除Vista以外更多的操作系统，如Windows Server 2003、Windows Server 2003 R2 Standard Edition (32-bit x86)、Windows Server 2003 R2 x64 editions、Windows Server 2003 Service Pack 1、Windows Server 2003
 x64 editions、所有版本的Windows Vista、Windows XP Service Pack 2。在SDK下载页面的“Supported Operating Systems”声明中，我们可以看到该SDK最新包含的内容，以及支持的所有操作系统（可以在它上面开发的以及为它开发应用的所有系统）。
参考资料：[http://forums.microsoft.com/MSDN/ShowPost.aspx?PostID=1986681&SiteID=1](http://forums.microsoft.com/MSDN/ShowPost.aspx?PostID=1986681&SiteID=1)
传说中最后支持Visual C++ 6.0的PSDK版本，微软在[MSDN博客](http://blogs.msdn.com/b/windowssdk/archive/2006/06/20/640172.aspx)上说
> 
Perhaps you want to install a version of the PSDK that will work with VS6, the last version of the PSDK to work with VS6 was the following version:
Microsoft Platform SDK, February 2003 Edition 
The only way you can get this SDK is by ordering it (for about $10 — you only pay shipping costs) from…
[http://www.qmedia.ca/launch/psdk.htm](http://www.qmedia.ca/launch/psdk.htm)
也许你想安装一个依然支持VS6的PSDK版本，最后一个能在VS6下工作的PSDK版本是下面的版本：Microsoft Platform SDK, February 2003 版。你得到这个SDK的唯一方式是向[http://www.qmedia.ca/launch/psdk.htm](http://www.qmedia.ca/launch/psdk.htm)订购（大概$10，你仅需支付邮递的钱）。
难道微软真的就不提供下载地址了吗？我不死心，Google啊Google，终于[搜索到了](http://hi.baidu.com/yfsq/blog/item/240aefd92ae4ca2810df9b7c.html/cmtid/9d05118284f76eb36d8119f3)，那个人太牛了，怎么找到的。下载下来安装了一下，果然是Windows Server 2003 PSDK。
下面是链接
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.1.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.1.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.2.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.2.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.3.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.3.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.4.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.4.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.5.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.5.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.6.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.6.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.7.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.7.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.8.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.8.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.9.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.9.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.10.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.10.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.11.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.11.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.12.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.12.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.13.cab](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.13.cab)
[http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.bat](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/PSDK-FULL.bat)
[Extract.exe](http://download.microsoft.com/download/platformsdk/sdk/update/win98mexp/en-us/3790.0/FULL/Extract.exe)
介绍一下安装方法:
1.下载后全部放在一个文件夹中.如d:/temp
2.打开命令行,移动到该文件夹.如 cd /d d:/temp
3.执行PSDK-FULL.注意给出要解压到的文件夹,如 PSDK-FULL d:/sdk
4.退出命令行,到d:/sdk中运行Setup.Exe.会弹出一个安装WEB页.
5.在页面左上角选 Download->Install,按照安装提示操作.
用IE打开，运行脚本执行，安装到C:\Program Files\Microsoft Platform SDK for Windows Server 2003 R2下。留意安装x86版本。
![](https://img-my.csdn.net/uploads/201212/13/1355390311_2892.jpg)
///////////////////////////
直接下载：
Microsoft Platform SDK for Windows Server 2003 R2** Include c++ 头文件**
[http://download.csdn.net/download/geedragon/2577598](http://download.csdn.net/download/geedragon/2577598)
C:\Program Files\**Microsoft Platform SDK for Windows Server 2003 R2 LIB文件**
[http://download.csdn.net/detail/geedragon/2577609](http://download.csdn.net/detail/geedragon/2577609)
