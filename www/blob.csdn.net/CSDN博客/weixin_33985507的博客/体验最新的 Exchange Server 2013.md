# 体验最新的 Exchange Server 2013 - weixin_33985507的博客 - CSDN博客
2012年11月18日 09:10:22[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
微软最近发布了不少新产品，就在Office 2013 客户预览版发布之际，其相关的系列产品也全面发布，如：SharePoint、Exchange、Lync、Office Web Apps Server、Project、Visio、Office Professional Plus 2013，试用网站可访问：http://www.microsoft.com/china/office/preview/try-more-products。在简单的体验了Office 2013 之后便着手测试Exchange 2013，搭建了一个全新的虚拟测试环境，该虚拟化境基于Windows Server 2012 的Hyper-V，并在上面虚拟了四台VMs：一台基于Windows Server 2012 的AD（other roles：DC, DNS, DHCP, CA）服务器，一台基于Windows Server 2012 的Exchange 2013（roles：Mailbox, Client Access）服务器，以及一台基于Windows 8 的客户端计算机并安装有Office 2013，此外为了模拟内、外网，还有一台基于Windows Server 2008 R2 的TMG2010 服务器。具体的结构可参考下图：
![](http://192.168.88.252/attachment/photo/Mon_1210/575_020b1349514173ef3c3001b789042.jpg)
养成了一个好习惯，在开始前都要看文档或者去TechNet上看Librarys，这样在之后的实践阶段才能避免不该发生的错误。
硬件需求方面，到没什么特别，不过内存可是要求蛮高！实际测试中发现分配了4GB内存的虚拟机在安装完毕Exchange 2013，重启后连系统都跑不起来，那叫一个慢！后来分配到5GB，强多了！！！活动目录林要求最低也是Windows Server 2003 林模式，而DC 和GC 最低也要是Windows Server 2008 Standard。
此外，Exchange 2013 Preview 在角色上发生了重大的变化，只有Mailbox 和Client Access 服务器角色，而它们只支持Windows Server 2008 R2 和Windows Server 2012 系统。
        Exchange 2013 对于Outlook 客户端的支持也有一些变化，Outlook 2003 将不再受支持，目前将只支持如下的Outlook 版本：
Outlook  2013 PreviewOutlook 2010 SP1 with April 2012  Cumulative UpdateOutlook 2007 SP3 with July 2012  Cumulative UpdateEntourage 2008 for Mac, Web Services  EditionOutlook for Mac 2011
对Exhcange 2013 有了初步的了解后，下面就开始我们的安装过程。与以往一样，安装Exchange 之前都要针对不同角色服务器进行软件需求方面的准备，对于Exchange 2013 也不例外。计划将Mailbox 和Client Access 服务器角色安装到一台服务器上，所以参考了Exchange 2013 Preview 的Librarys –Exchange 2013  Prerequisites。
1.         安装所需的Windows 功能和组件，为此在PowerShell 执行下面的命令行。
Install-WindowsFeature AS-HTTP-Activation, Desktop-Experience,  NET-Framework-45-Features, RPC-over-HTTP-proxy, RSAT-Clustering,  Web-Mgmt-Console, WAS-Process-Model, Web-Asp-Net45, Web-Basic-Auth,  Web-Client-Auth, Web-Digest-Auth, Web-Dir-Browsing, Web-Dyn-Compression,  Web-Http-Errors, Web-Http-Logging, Web-Http-Redirect, Web-Http-Tracing,  Web-ISAPI-Ext, Web-ISAPI-Filter, Web-Lgcy-Mgmt-Console, Web-Metabase,  Web-Mgmt-Console, Web-Mgmt-Service, Web-Net-Ext45, Web-Request-Monitor,  Web-Server, Web-Stat-Compression, Web-Static-Content, Web-Windows-Auth, Web-WMI,  Windows-Identity-Foundation -Restart
![](http://blog.51cto.com/attachment/201211/162243904.png)
![](http://blog.51cto.com/attachment/201211/162307567.png)
2.         下载并安装如下软件：
a)Microsoft  Unified Communications Managed API 4.0, Core Runtime 64-bit
    b)Microsoft Office 2010 Filter  Pack 64 bit
    c)Microsoft Office 2010 Filter  Pack SP1 64bit
![](http://blog.51cto.com/attachment/201211/162701918.png)
![](http://blog.51cto.com/attachment/201211/162720589.png)
![](http://blog.51cto.com/attachment/201211/162734370.png)
![](http://blog.51cto.com/attachment/201211/162836114.png)
![](http://blog.51cto.com/attachment/201211/162849929.png)
![](http://blog.51cto.com/attachment/201211/162904486.png)
![](http://blog.51cto.com/attachment/201211/162923522.png)
![](http://blog.51cto.com/attachment/201211/162935988.png)
![](http://blog.51cto.com/attachment/201211/162944564.png)
3.         卸载Visual C++ 11 Beta Redistributable (x64)，为此请进入控制面板下的程序和功能中将其卸载。
![](http://blog.51cto.com/attachment/201211/164134108.jpg)
4.扩展架构
架构准备：
登录到AD服务器，打开命令提示符进入Exchange安装光盘目录，输入：
         Setup /PrepareSchema /IAcceptExchangeServerLicenseTerms
![](http://blog.51cto.com/attachment/201211/165120557.png)
林准备：
打开命令提示符进入Exchange安装光盘目录，输入：
         Setup /PrepareAD /OrganizationName:zhaopeng /IAcceptExchangeServerLicenseTerms
![](http://blog.51cto.com/attachment/201211/165138704.png)
域准备：
打开命令提示符进入Exchange安装光盘目录，输入：
         Setup /PrepareDomain:zhaopeng.com /IAcceptExchangeServerLicenseTerms
![](http://blog.51cto.com/attachment/201211/165158369.png)
一切准备就绪后就可以执行Exchange 2013 Preview 的安装程序了，安装步骤跟随向导完成，非常简单，就不在叙述！详细过程可参考下面的截图：
1.把Exchange2013的安装光盘放在光驱里，第一步选择“现在不检查更新”，下一步：
![](http://blog.51cto.com/attachment/201211/165444646.png)
 2.开始复制文件操作：
![](http://blog.51cto.com/attachment/201211/165503504.png)
![](http://blog.51cto.com/attachment/201211/165703852.png)
3.软件简介，下一步![](http://blog.51cto.com/attachment/201211/165744878.png)
 4.接受许可协议，下一步：
![](http://blog.51cto.com/attachment/201211/165828907.png)
5.选择推荐安装，下一步：
![](http://blog.51cto.com/attachment/201211/165859771.png)
 6.选择要安装的服务器角色，邮箱服务器角色和客户端访问服务器角色：
![](http://blog.51cto.com/attachment/201211/170016866.png)
7.选择安装的目录，下一步：
![](http://blog.51cto.com/attachment/201211/170104197.png)
 8.“恶意软件保护”，暂时不选：
![](http://blog.51cto.com/attachment/201211/170135865.png)
9.进行安装钱的检查操作:
![](http://blog.51cto.com/attachment/201211/170228817.png)
下图是在没有任何准备的情况下执行Exchange 2013 Preview 安装的检测结果！虽然提示需要安装3个软件包，但是并未提示所需的IIS 组件，希望RTM 版会有改进……
![](http://blog.51cto.com/attachment/201211/170306364.png)
11.开始安装Exchange2013，耐心等待：
![](http://blog.51cto.com/attachment/201211/170325399.png)
焦急的等待着Exchange 2013 Preview 完成安装，经历40多分钟的等待后，Exchange 2013 Preview 终于安装完毕！
![](http://blog.51cto.com/attachment/201211/170622970.png)
迫不及待地去找EMC 看看新版的管理界面，可是找了半天也没发现EMC 的影儿！！！唯一的一个MMC 管理器还是Exchange Toolbox。
![](http://blog.51cto.com/attachment/201211/170650357.png)
基于PowerShell命令行管理控制台：
![](http://blog.51cto.com/attachment/201211/170704595.png)
之后看了Exchange 2013  Post-Installation tasks才知道，除了使用PowerShell 配置和管理以外，要图形化配置和管理Exchange 2013 Preview，必须通过Web 方式，即：ECP！OK，打开浏览器输入地址“[https://exchange2013服务器的FQDN名/ecp](https://ex2013/ecp)”，Exchange Admin Center 便呈现在眼前！简约的设计风格，看起来挺舒服！
![](http://blog.51cto.com/attachment/201211/084414315.png)
在这个ECP 下我们就可以执行之前版本的所有配置和管理工作，微软在这个改进上可谓大胆！
1、收件人管理：
![](http://blog.51cto.com/attachment/201211/170957583.png)
2、权限管理：
![](http://blog.51cto.com/attachment/201211/171023369.png)
 3、合规性管理：
![](http://blog.51cto.com/attachment/201211/171042964.png)
 4、组织管理：
![](http://blog.51cto.com/attachment/201211/171139323.png)
 5.保护管理：
![](http://blog.51cto.com/attachment/201211/171148351.png)
 6、邮件流管理：
![](http://blog.51cto.com/attachment/201211/171200232.png)
 7、移动管理：
![](http://blog.51cto.com/attachment/201211/171215178.png)
 8、公用文件夹管理：
![](http://blog.51cto.com/attachment/201211/171223428.png)
 9、统一消息管理：
![](http://blog.51cto.com/attachment/201211/171301180.png)
 10、服务器管理：
![](http://blog.51cto.com/attachment/201211/171309430.png)
 11、混合管理：
![](http://blog.51cto.com/attachment/201211/171319174.png)
下来还是看看OWA 的变化吧，登录界面跟前面的ECP 没区别。
![](http://blog.51cto.com/attachment/201211/171332199.png)
![](http://blog.51cto.com/attachment/201211/171338776.png)
![](http://blog.51cto.com/attachment/201211/171346596.png)
![](http://blog.51cto.com/attachment/201211/090534903.png)
![](http://blog.51cto.com/attachment/201211/090556230.png)
![](http://blog.51cto.com/attachment/201211/090610554.png)
使用界面也是十分的简约！看来这将是未来的设计趋势，返朴归真了……不管怎样，新界面虽然朴素，但真的漂亮！挺喜欢这样的风格。下面这种图就是撰写邮件时的布局！清新雅致……
使用OWA发送测试邮件成功！
![](http://blog.51cto.com/attachment/201211/090651531.png)
另外在右上角的设置图标下可以看到“管理应用程序”该选项也可以通过“配置”-“应用程序”进入，如下图所示，现在Office 全面支持apps，可以添加不少实用的应用，除了能够增强Office，还方便了用户在同一个软件下实现更丰富的工作和使用体验！目前针对Outlook 除了默认的三个app 外，我们还可以添加Linkedin。微软推出这样模式将为个人、公司带来更多的商业机遇，还等什么！加入到Windows 开发阵营来吧！！！
![](http://blog.51cto.com/attachment/201211/093559935.png)
![](http://blog.51cto.com/attachment/201211/093629756.png)
![](http://blog.51cto.com/attachment/201211/093643459.png)
![](http://blog.51cto.com/attachment/201211/093656845.png)
![](http://blog.51cto.com/attachment/201211/093727899.png)
![](http://blog.51cto.com/attachment/201211/093739338.png)
![](http://blog.51cto.com/attachment/201211/093751595.png)
![](http://blog.51cto.com/attachment/201211/093801876.png)
![](http://blog.51cto.com/attachment/201211/093808259.png)
在新版的OWA 下，还留意到一个功能“离线使用邮件”，顾名思义，应该就是离线阅读吧！根据提示所述，在启用该功能后，将在本地存储邮件，可以没有网络连接的时候进行访问。绝！！！强大！！！但是，位于外网的用户貌似并不能使用该功能，想想嘛，在内网的用户一般都是用Outlook。
最后使用Outlook2013测试下邮件：     
1.使用域用户zhangsan登录系统，打开[Outlook2013](http://blog.51cto.com/attachment/201211/101045228.png)：
![](http://blog.51cto.com/attachment/201211/101045228.png)
2.配置[Outlook2013](http://blog.51cto.com/attachment/201211/101126128.png)账户：
![](http://blog.51cto.com/attachment/201211/101126128.png)
3.使用当前登录的域帐户zhangsan作为[Outlook2013](http://blog.51cto.com/attachment/201211/101146598.png)的邮件帐户：
![](http://blog.51cto.com/attachment/201211/101146598.png)
4.连接邮件服务器配置：
![](http://blog.51cto.com/attachment/201211/101224690.png)
5.进入[Outlook2013](http://blog.51cto.com/attachment/201211/101247401.png)界面：
![](http://blog.51cto.com/attachment/201211/101247401.png)
6.使用zhangsan给administrator发送一份测试邮件：
![](http://blog.51cto.com/attachment/201211/101311227.png)
7.使用administrator用户打开OWA，查看zhangsan发来的测试邮件：
![](http://blog.51cto.com/attachment/201211/101342947.png)
结束语：到此Exchange2013的预览测试已经基本完成，我想大家也对Exchange2013的正式版的发布齿目以待，我也相信Exchange2013正式版的界面和功能远远高于预览版，让我们大家期待微软的Exchange2013正式版吧！
