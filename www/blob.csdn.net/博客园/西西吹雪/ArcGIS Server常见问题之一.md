
# ArcGIS Server常见问题之一 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[ArcGIS Server常见问题之一](https://www.cnblogs.com/watsonyin/archive/2006/03/29/361637.html)
摘自ESRI网站，有些内容是有限制的，要会员才能看到。目前关于ArcGIS Server开发的资料非常少，就这么一点文档，还要限制，不能让大家共享，实在是罪过。现在我贴出来，希望这些资料对一起学习AGS的朋友有所帮助。
错误描述|
在ArcCatalog中，当成功的创建了server object之后，server object不能被预览。ArcCatalog显示下面的错误信息：
“你的选择不能在当前视图中显示.”|
导致原因|
server属性中被指定的输出值和http位置可能不正确。|
解决办法|
所有的创建的ArcGIS Server Objects需要被停止。
1 在ArcCatalog中，右击已经添加的ArcGIS Server，并选择Server属性。
2 在目录页中，选中已经添加的输出目录并点击编辑。
3 输出目录字段应该指向一个有效的目录，比如 C:\ArcGIS\Output。
如果一个虚拟目录已经被指定，确认它是有效的，并且看上去和下面的格式一样，
http://<servername>/output
4 如果有错请更改，然后重新创建server object来确认更改是否有效。|
错误描述|
当在Visual Studio.NET 2003上使用ArcGIS Server .NET 的ADF模板创建一个项目时，返回如下错误信息：
“在服务器<主机名>上的Web站点不可用（Web Site on server <host> is not available）”|
导致原因|
该模板包含了调用'http://localhost'这一URL的属性。如果在IIS中禁用了本地主机的服务，则显示该错误信息。禁用本地服务可能是因为你的Web站点已经为其指定了一个IP地址或者是因为安全方面的考虑。|
解决办法|
启用对 http://localhost的访问决定于您安装的是哪种类型的操作系统，是服务器还是非服务器。
对于非服务器的操作系统：
1.点击Internet信息服务控制台，右键选择默认站点的属性，打开属性对话框，并选择网站标签页。
2.修改IP地址栏为"(全部未分配)"。
3.重启IIS。
在IE浏览器中浏览刚刚启用的 http://localhost 服务，在Visual Studio .NET 2003 中用模板创建项目，现在应该是成功的了。
对于Windows Server操作系统：
1. 在Internet信息服务控制台上创建一个新的Web站点。
2. 为其分配IP地址：127.0.0.1。意味着只能在本地计算机上启用该站点。
3. 浏览Web站点的根目录，比如C:\Inetput\wwwroot 。
此时在Visual Studio .NET 2003 中用模板创建项目应该没有问题。|
错误描述|
当启动一个服务对象时，出现下述错误信息：“在机器XXX上创建Server 环境（Context）失败。拒绝访问output文件夹。”|
导致原因|
ArcGIS Server Object Container的帐号必须拥有虚拟文件夹的写权限。例如虚拟文件夹为: c:\inetpub\wwwroot\temp|
解决办法|
执行如下步骤：
右键点击虚拟文件夹如: c:\inetpub\wwwroot\temp
1.选择属性->安全标签页.
2.添加帐号并赋予写权限，使其可以读写图片.
3.点击OK，关闭属性对话框.|

错误描述|
使用下面的代码，来进行使用字体中的符号进行点要素渲染的时候，会出现地图消失的问题。
下面的代码是用来生成需要在渲染过程中使用的点符号的，使用的是宋体中index为21的字作为符号。
private static ICharacterMarkerSymbol
GetMarkerSymbol(IServerContext pServerContext,PowerNet.GISCommon.SymbolClass pSymbolClass)
{
System.Drawing.Font drawFont = new
System.Drawing.Font("宋体", 21);
ICharacterMarkerSymbol charMarker=
pServerContext.CreateObject("esriDisplay.CharacterMarkerSymbol") as
ICharacterMarkerSymbol;
charMarker.Font = (stdole.IFontDisp)
OLE.GetIFontDispFromFont(drawFont);
charMarker.CharacterIndex = pSymbolClass.CharacterIndex;
IRgbColor pRGB = pServerContext.CreateObject("esriDisplay.RgbColor") as IRgbColor;
pRGB.Red=pSymbolClass.SymbolColorR;
pRGB.Green=pSymbolClass.SymbolColorG;
pRGB.Blue=pSymbolClass.SymbolColorB;
charMarker.Color = pRGB as IColor;
charMarker.Size = pSymbolClass.SymbolSize;
return charMarker;
}|
导致原因|
使用ArcGIS Engine进行开发时，对于点要素类，是可以使用指定字体中的符号作为渲染的符号的，但是在arcgis server的应用中来实现这个过程，就稍微有点不同，导致的原因还是本地对象和远程对象进行了混用？对于ao对象，可能大家都已经有这个概念，就是一般不在本地创建对象，但是对于其它的对象，比如.net中的对象，可能就没有太注意了，所以才导致了这个问题的出现。|
解决办法|
这个问题的解决办法是这样的，为了避免在程序中使用下面这句代码：
System.Drawing.Font drawFont = new
System.Drawing.Font(pSymbolClass.FontSymbolName, 21);
可以采用先在*.Style文件中把符号都配好，然后转成*.ServerStyle文件，然后使用下面的代码得到相应的符号：
private IMarkerSymbol createSymbol(IServerContext pServerContext,string index)
{
IStyleGallery pSGallery = pServerContext.CreateObject("esriDisplay.ServerStyleGallery") as IStyleGallery ;
IStyleGalleryStorage pSGStorage = pSGallery as IStyleGalleryStorage ;
pSGStorage.TargetFile = @"D:\xjw.ServerStyle";
pSGStorage.AddFile (@"D:\xjw.ServerStyle");
IEnumStyleGalleryItem pEnumGItem = pSGallery.get_Items("MarkerSymbols",@"D:\xjw.ServerStyle",index);
pEnumGItem.Reset ();
IStyleGalleryItem pSGItem = pEnumGItem.Next();
IMarkerSymbol pMSymbol = pSGItem.Item as IMarkerSymbol;
IRgbColor pRGB = pServerContext.CreateObject("esriDisplay.RgbColor") as IRgbColor;
pRGB.Red=255;
pRGB.Green =0;
pRGB.Blue=0;
pMSymbol.Color=pRGB;
pMSymbol.Size=18;
pEnumGItem=null;
pSGItem=null;
return pMSymbol;
}|

错误描述|
这个错误可能发生在.net中的ArcGIS Primary Interop Assemblies(PIA)。在错误信息中的CLSID可能会变化：
"COM object with CLSID {XXXX} is either not valid or not registered"|
导致原因|
在ArcGIS安装中选择安装.NET的支持，PIAs就会为所有的ArcGIS库而安装上。
当在尝试使用一个对象库的PIA时，而这个对象库根本就没有安装时，错误就会发生，因为Com对象不能被初始化。
例如，3DAnalyst扩展模块没有被安装，而尝试使用ESRI.ArcGIS.Analyst3D命名空间中的对象，使用如下代码：
ESRI.ArcGIS.Analyst3D.IAnimationTrack aTrack = new ESRI.ArcGIS.Analyst3D.AnimationTrackClass();
一个错误就会发生：
"Error number -2147221164: COM object with CLSID {4FEDC9CB-A7BE-11D5-B2A0-00508BCDDE28} is either not valid or not registered."|
解决办法|
编程只能使用安装了的对象库。为了开发使用一个特殊的库，那就需要重新安装ArcGIS产品，确保所需的扩展模块都被安装，以及所有的.NET的支持选项。额外的许可会被需要。
1 识别使用的对象，以及这些对象所属的命名空间。
2 识别命名空间所属的产品。例如，为了使用ESRI.ArcGIS.Analyst3D命名空间中的对象，在开发帮助中浏览库总体说明那一页。会有注解说明这个命名空间是有ArcGIS Engine产品的3D扩展模块，ArcGIS desktop 3D扩展模块，以及ArcGIS Server的3D扩展模块支持。
3 重新安装产品，确保所需的扩展模块得到安装，且保证.Net支持选项被选中。|

错误描述|
当使用ArcGIS开发者控件开发应用程序、控件拖放于窗体上的时候，出现以下错误信息：
"该控件需要一个ESRI Designer 许可。请从工程中移除该控件。"|
导致原因|
出现这个信息是因为ArcGIS Engine Developer Kit 这个产品未在机器上安装并授权。
ArcGIS Desktop 安装了以下控件：
-MapControl
-PageLayoutControl
-GlobeControl
-ReaderControl
-SceneControl
-TOCControl
-ToolbarControl
ArcGIS Desktop 许可仅允许使用MapControl 和 PageLayoutControl 进行开发或设计应用程序。
剩下的其它控件许可给ArcGIS Engine，并仅能在ArcGIS Engine Developer Kit 已安装并授权时用于开发。|
解决办法|
购买ArcGIS Engine Developer Kit 产品，并在机器上授权以下任意控件：
-GlobeControl
-ReaderControl
-SceneControl
-TOCControl
-ToolbarControl
有关如何使用这些控件的详细信息，以及哪些控件需要对应哪些许可，请参考ArcGIS Developer Help。例如， 'TOCControl CoClass' 帮助主题提到，该控件只在有ArcGIS Engine 产品时才可用。|

错误描述|
安装出错1935
当安装ArcGIS Server, ArcGIS Desktop, including ArcView, ArcEditor, ArcInfo; or ArcIMS, 都可能发生如下错误:
"Error 1935: An error occured during the installation of assembly component {303994BA-6487-47AE-AF1D-7AF6088EEBDB}. HRESULT: -2147024894."
之后点击'OK' 安装回滚退出.|
导致原因|
导致原因
这个错误发生在系统安装（Microsoft XML Parser 4 ）更新.这现同样的错误信息将发生在试图安装（Microsoft XML Parser 4.0）的时候； 这也可能修改，但是经常会因为其他产品的安装而影响系统文件破坏。|
解决办法|
步骤：
1. 尽可能停止一些后台程序,例如adware 或 spyware. 有可能也停止Adwatch，在机器上删除所有的 adware.；
2. 尽可能停止一些Windows服务，控制面板 >管理员工具 >服务；
3. 检查Internet Explorer 6.0的安装. 检验Microsoft service 安装包和安装更新；
4. 在安装产品前请先安装Microsoft XML 4.0 SP2，这有可能下载其相关联的一部分；
5. 接着安装ESRI软件产品在第4步安装成功；|

错误描述|
在操作系统是Windows XP SP2、CPU为 AMD Athlon 64系列或 Pentium 4 和 Celeron-D 的机器上安装ArcGIS 9.0时，出现1904错误，系统无法注册一些 dll 文件。|
导致原因|
在Windows XP SP2 中, DEP（数据执行保护）选项默认情况下是打开的， AMD和Intel新的CUP系列中包含这个选项。数据执行保护是一套硬件和软件技术用于执行对内存的额外检查，以保护电脑遭受恶意代码的破坏。|
解决办法|
方法一：移除Windows XP的SP2，然后安装ArcGIS 9.0。
方法二：关闭DEP（数据执行保护），安装ArcGIS 9.0，安装完再激活数据执行保护。具体如下：
1. 以管理员权限登陆Windows XP。
2. 鼠标邮件点击“我的电脑”，选择“属性”菜单
3. 点击“高级”页。
4. 选择“启动与恢复”下的“设置”按钮。
5. 选择默认操作系统，如下:
"Microsoft Windows XP Professional" /fastdetect /NoExecute=OptIn
6. 点击“编辑” 按钮打开boot.ini 文件，将其中的“”/NoExecute=OptIn”改为“/NoExecute=AlwaysOff”。
7. 保存文件，重新启动计算机。
8. 安装ArcGIS，此时应该就不会出现1904错误了
9. 安装完ArcGIS以后，将 boot.ini 文件设置改回来，以重新激活数据执行保护的安全选项。|




posted on2006-03-29 12:02[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=361637)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
