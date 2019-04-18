# IIS的安装与配置 - 深之JohnChen的专栏 - CSDN博客

2008年11月09日 17:43:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2442标签：[iis																[internet																[windows																[文档																[web																[浏览器](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=web&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=internet&t=blog)](https://so.csdn.net/so/search/s.do?q=iis&t=blog)
个人分类：[WEB编程](https://blog.csdn.net/byxdaz/article/category/479812)


**IIS的安装与配置**

**1、运行环境与软件要求**

Windows 2000 以上 
IIS 4.0 以上

**2、IIS的安装与配置**

**A.安装IIS**
　　若操作系统中还未安装IIS服务器，可打开“控制面板”，然后单击启动 “添加/删除程序”， 在弹出的对话框中选择 “添加/删除Windows组件”，在Windows组件向导对话框中选中“Internet信息服务（IIS）”，然后单击“下一步”，按向导指示，完成对IIS的安装。

（图例如下图1、图2） 由于本人使用的是Windows Server 2003操作系统所以和大家的安装界面有所出入，但大体相同）：

![2005103203114359.jpg](http://v2.djasp.net/upFiles/edtUp/2005103203114359.jpg)
图1

![2005103203115525.jpg](http://v2.djasp.net/upFiles/edtUp/2005103203115525.jpg)
图2 

**B.启动Internet信息服务（IIS）**

　　Internet信息服务简称为IIS，如果您是在windowsXp或windows2000下安装的IIS,当您的IIS在安后后，IIS服务就会自动动行，您只需要设置相应的虚拟目录即可调试网页，如果您的系统是Windows Server 2003或更高版本的，IIS服务默认情况下是关闭的，此时您需要用如下方法来打开IIS服务：
　　单击Windows开始菜单>>所有程序>>管理工具>>Internet信息服务（IIS）管理器，即可启动“Internet信息服务”管理工具（如图3），或者点击开始菜单>>控制面版>>管理工具>>Internet信息服务(iis)管理器，也可以启动"Internet信息服务"。然后在如下图所示的左边下面的“默认网站”上点击鼠标右键，选择＞＞“启动”即可启动默认网站的IIS服务了。

![2005103203115396.jpg](http://v2.djasp.net/upFiles/edtUp/2005103203115396.jpg)
图3 

**Ｃ.配置IIS**
IIS安装后，系统自动创建了一个默认的Web站点，该站点的主目录默认为C://Inetpub/wwwroot。
用鼠标右键单击“默认Web站点”，在弹出的快捷菜单中选择“属性”，此时就可以打开站点属性设置对话框，（如图4）在该对话框中，可完成对站点的全部配置。

![2005103203115449.jpg](http://v2.djasp.net/upFiles/edtUp/2005103203115449.jpg)
图4—默认Web站点属性

**主目录与启用父路径**
单击“主目录”标签，切换到主目录设置页面，（如图5）该页面可实现对主目录的更改或设置。注意检查启用父路径选项是否勾选，如未勾选将对以后的程序运行有部分影响。（如图6），主目录—配置---选项。

![2005103203115845.jpg](http://v2.djasp.net/upFiles/edtUp/2005103203115845.jpg)
图5—默认Web站点主目录设置

![2005103203117822.jpg](http://v2.djasp.net/upFiles/edtUp/2005103203117822.jpg)
图6—默认Web站点启用父路径设置

**设置主页文档**
　　单击“文档”标签，可切换到对主页文档的设置页面，（如图7）主页文档是在浏览器中键入网站域名，而未制定所要访问的网页文件时，系统默认访问的页面文件。常见的主页文件名有index.htm、index.html、index.asp、index.php、index.jap、default.htm、default.html、default.asp等
　　IIS默认的主页文档只有default.htm和default.asp，根据需要，利用“添加”和“删除”按钮，可为站点设置所能解析的主页文档。

提示：如果您想用IIS建立多个站点服务，您也可以在“默认网站”上点右键＞“所有任务”＞“新建虚拟目录”来新建一个或多个子站点。

**D、网页的调试**

　　如果您向上图3所示的，设置了您的“IIS主目录”为“E:/web”后，并且在IIS的主页文档中有default.asp这个页面，那么E盘下的web目录将会作为您的IIS主目录，此时您可以在E:/web目录下建立一个名为default.asp的asp页，此页的源码如下：
<%
      Response.Write"ＡＳＰ程序调试！"
%>
　　然后您就可以通过在浏览器中输入如下四种虑拟路径中的一种方式来调试运行的这个E:/web/default.asp：
方法一：http://localhost/
方法二：http://127.0.0.1/
方法三：http://您的电脑IP地址/
方法四：http://您的计算机名/
对于第三种方法，如果您的电脑在局域网中，则您的IP地址为您的电脑在您局域网中的IP，
如果您不知道您电脑的IP地址，
请在您的电脑的“开始”>“运行”里面输入：
ipconfig
然后回车，即可看见诸如：
IP Address:.............192.168.1.X
这样的一串字符(X为一个数字)，其中的：“192.168.1.X”就是您的ＩＰ地址。
对于第一种方法，如果您不知道您的计算机名是什么，请在“我的电脑”图标上点右键，选择“属性”，然后在“计算机名”中，就可以找到您的计算机名。

例：本人的电脑设置了E:/web为IIS主目录，并在该目录下建立了一个名为demo.asp的网页，我的计算机名为：djasp，我的计算机IP地址为：192.168.0.2，我可以通过以下几种方式来访问这个asp页：
http://localhost/demo.asp
http://127.0.0.1/demo.asp
http://192.168.0.2/demo.asp
http://djasp/demo.asp


