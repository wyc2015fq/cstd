# 设计网络服务器ISAPI - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:08:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1248标签：[服务器																[网络																[cgi																[command																[iis																[vc++](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)](https://so.csdn.net/so/search/s.do?q=iis&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=cgi&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
个人分类：[网络编程](https://blog.csdn.net/byxdaz/article/category/166215)


设计网络服务器ISAPI

阅读本文需要具备基本的Web/CGI 、MFC 以及Visual C++ v4.1或更新版本的知识。 

　　ISAPI 与CGI

　　很长时期以来，CGI都作为交互web开发的一个标准工具。CGI脚本允许使用者用各种语言来编写简单的应用程序，它在网络服务器上运行并直接输出到用户的网络浏览器上。用户的数据通过环境变量或者标准输入设备输入，程序则会通过标准输出返回HTML文本。这么一个简单的设计，就结合了Perl及TCL语言，也使得CGIs非常简单易用。

　　但CGIs也有一个非常大的缺点：性能问题。虽然有很多方法让CGI运行得更快（例如：用户可以把CGI写成可执行的可编译的语句，而不是PERL脚本），但速度仍是问题所在。每次进入CGI都得通过网络，可执行CGI仍必须为每一次进入请求创建新的程序。对于一个访问量大的站点来说，上述方法对于服务器无疑是个巨大的负担。

　　当微软开始研究他们的网络服务器（MS IIS 或者是 IIS）时意识到CGIS对于大型的网络服务器来说是个主要的问题所在。 

　　一、进入ISAPI

　　事实上，ISAPI使用DLL。所用的DLL则会被加载进服务器中。将代码缓存进内存的作法替代了每请求一次重新加载的做法，此种技术的应用正呈上升趋势。

　　ISAPI 的优点

　　速度

　　在功能上此优点得到最大体现。

　　特征

　　ISAPI可以创建服务器过滤器。完全由MFC集成。

　　ISAPI的不足

　　标准性

　　目前只有少数几种服务器支持ISAPI

　　开发的简便性

　　文档非常少，并且调试程序的过程比较枯燥。

　二、ISAPI的基本知识

　　ISA是基本于MFC ChttpServer类别的，CHttpServer 基本上控制了所有的服务器内部交互行为，并包含了用户要求的所有功能。事实上ISA能够处理大量的相类似的请求。因此，CHttpServer为每个请求都创建了ChttpServerContext。ChttpServerContext包括了所有的专业化数据以及所有的HTML。

　　ISAPI DLLs是由用户的需要而开发的，用法与CGI类似。如下例"

　　http://www.mysite.com/myisa.dll?name=bob&id=15248

　　"名称"和"ID"域以及相关数据都进入了ISA，并且数据必须在使用以前被放置进数据库存中。为加快ISAPI则需要用到 "mapping"系统。

　　"mapping"系统同样具有其它功能：ISAPI能引导 请求 到ISA内的专用功能区。"请求"串里包含了可以令"mapping"系统用于引导"请求"至适当功能区的命令。

　　因为ISAPI使用了处理"请求"的指令，因此ISA开发系统就给人感觉有点迟缓。但是只要一旦掌握，确是一个处理"请求"的强有力的工具。

　　设置项目

　　开发ISA的第一步是建立一个项目工作区。正如其它由Visual C++ (VC++)创立的工作区一样，有工作向导引导用户完成最初步骤。选择New-Project Workspace，选择"ISAPI Extension Wizard"作为项目种类，命名为"Hello Web"，然后点击创建。

　　完成上述步骤后，会跳出对话框询问您愿意创建哪种类型的ISA。缺省设置已为ISA配置好，MFC会动态的通过缺省连接。如果您的服务器已安装MFCDLLS，则上述步骤适用，如果没有则ISA不会运行。如果项目需要静态连接。完成上述步骤后，点击"完成"。 Visual C++ 会提醒您文件正创建中，并会生成ChelloWebExtension。本文里的所有工作都将在CHelloWebExtension"里完成。

　　现在你已建立好一个项目，是时候完成一些ISAPI开发工作了。正如早先提到的，ISA在运行时则会成为IIS的一部份。IIS依次序的运行就象NT服务器一样。这样则会使得调试过程变得复杂化。因为VC++的调试系统不能够控制ISA，当服务器作为一个服务系统时。为了解决这个问题，微软将IIS分成两个部份，一个是service,，另一个是可执行的。通过可执行的部份，利用指令行可调节服务器。虽然问题得到了解决并使得开发更为简便，但是设置以上步骤的过程却有点枯燥。

　　当你进入调试部份，在用户的允许状态下，VC++ (然后是 IIS)会在用户的帐户下运行。然而有些是用户的指令得不到进入但IIS得执行的部份，因此用户得做以下步骤：

　　打开User Manager域工具组（在Administrative Tools program 组）

　　在Policies菜单中选择User Rights

　　打开Show Advanced User Rights栏

　　在右边列表中选择Act as part of the operating system

　　点击Add按钮以弹出Add Users and Groups对话框。点击Show Users按钮，选择您需要的帐户。然后点击Add.

　　重复相同步骤以生成Generate security audits rights。

　　以上步骤完成后请退出然再重新登录，以使程序生效。

　　IIS包含了FTP Publishing Service, Gopher Publishing Service, World Wide Web Publishing Service等三个Service。一旦调试程序从指令行里运行IIS，则三个services则会停止运行。

　　如果用户想让程序调试得到合理分配，最好关掉IIS service，转而使用Services Control Applet，并且禁止自动重启功能。

　　一旦当Service关闭，则项目工作区则需按以下步骤来配置

　　从Build菜单中选择Settings

　　点击Debug标签并选择"General Category". 

　　在"Executable for debug session"域中键入可执行IIS的位置

　　在"Program arguments" field中键入"-e w3svc"

　　点击Link 标签。

　　在"Output filename" field中键入路径和文件名。路径会在站点的目录树里显示，因此可以通过URL进入。例如：您的站点根目录是c:/www/，并且您将"helloweb.dll"放在根目录里，因此URL则会是：

　　http://www.mysite.com/helloweb.dll

请在更改设置后退出登录并重新登录。

　　由ISAPI Extension 向导生成的默认设置包括了编译ISA的所有细节。现在您已完成配置调试程序的环境，现在可以创建并运行项目。

　　按F5以打开ISA，当系统询问是否建立项目时，按YES。

　　在程序调试创建的几秒后，IIS会在后台运行。

　　后将DLL的URL输入你喜爱的网络浏览器，记住在尾部加入一个问号。

　　则URL会显示如下：

　　　http://www.mysite.com/helloweb.dll?

　　第一次连接到ISA会耗费上几秒。但是DLLS会在执行后进行缓存，因此速度会变得稳定。

　　在DLL登录后，将会显示以下信息：

　　This default message was produced by the Internet Server DLL Wizard.

　　Edit your CHelloWebExtension::Default() implementation to change it.

　　现在你已得到一个工作的ISA

　　Walking through the base code

当EXTENSION_CONTROL_BLOCK提出请求时，则会传送到Command Parse Map.。 Parse Map是由一系列的宏所定义的，正如下面所举的代码，是从Hello Web项目中复制过来的：

　　ISA有两个主要元素：Parse Map 以及 Command Handler 功能。

　　BEGIN_PARSE_MAP(CHelloWebExtension, CHttpServer)

　　// TODO: insert your ON_PARSE_COMMAND() and 

　　// ON_PARSE_COMMAND_PARAMS() here to hook up your commands.

　　// For example:

　　ON_PARSE_COMMAND(Default, CHelloWebExtension, ITS_EMPTY)

　　DEFAULT_PARSE_COMMAND(Default, CHelloWebExtension)

　　END_PARSE_MAP(CHelloWebExtension)

　　BEGIN_PARSE_MAP标注了parse map的起始处，ISA's ChttpServer 和基类CHttpServer作为参数。

　　ON_PARSE_COMMAND_PARAMS()表明对指令处理器是一种特殊请求格式或命令，它的参数就是指向的函数的类名和请求的格式。DEFAULT_PARSE_COMMAND说明那个函数被调用，参数是被调用的类名。

　　Command Handler functions是主函数ChttpServer类的成员函数，parse map通过get方法调用CHttpServer。下面则是Hello Web 的"缺省"指令处理程序：

void CHelloWebExtension::Default(CHttpServerContext* pCtxt)
{
StartContent(pCtxt);
WriteTitle(pCtxt);
*pCtxt << _T("This default message was produced by the Internet");
*pCtxt << _T("Server DLL Wizard. Edit your CHelloWebExtension::Default()");
*pCtxt << _T("implementation to change it./r/n");
EndContent(pCtxt);
}

　　当请求为空或包含"Default"这个函数被调用，请求通过ChttpServerContext进入程序体，第一个参数必须是一个ChttpServerContext对象。StartContent()方法在pCtxt中放置＜HTML＞＜BODY＞，WriteTitle()则是放置 ＜title＞标记。下面 三行语句则写入缺省消息，并使pCtxt指向ChtmlStream,在ISA处理完成后发送到客户端。

　　 Hello Web 

　　第一个程序会以"HELLO WEB"替换缺省信息串。

　　找到CHelloWebExtension class 中的Default() 成员函数，按照下面的方式更改。

void CHelloWebExtension::Default(CHttpServerContext* pCtxt)
{

StartContent(pCtxt);
WriteTitle(pCtxt);
*pCtxt << _T("Hello Web!");
EndContent(pCtxt); 
}

　　创建,运行DLL并从网络浏览器重新载入并替换DLL

　　缺省信息为：

　　produced by the InternetServer DLL Wizard. Edit

　　your CHelloWebExtension::Default() implementation to change it.

　　将会显示：

　　Hello Web!

　　如果出现"Server Error 500: Specified module not found."提示，您所创建的项目是动态连接，而且缺少必要的DLLs。纠正这个错误，就必须使用MFC重新静态的连接项目。


