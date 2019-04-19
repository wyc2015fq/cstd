# win7 开机优化系列-中篇 - 三少GG - CSDN博客
2013年01月21日 14:20:51[三少GG](https://me.csdn.net/scut1135)阅读数：1632
[Windows7开机加速全攻略五：系统服务优化](http://www.win7china.com/html/5398.html)
**五、Win7系统服务优化攻略**
　　前文提到Windows系统启动的原理，其中加载各项系统服务是影响系统启动时间的重要因素，之前[软媒](http://www.ruanmei.com/)在[Win7之家](http://www.win7china.com/)（[www.win7china.com](http://www.win7china.com/)）和[Vista之家](http://www.vista123.com/)（[www.vista123.com](http://www.vista123.com/)）上都写过类似的服务优化攻略，今天重新整理一下，列入这个开机加速全攻略里面，保持这次开机提速技术上的完整性。
**1、了解 Windows 7 服务的基础知识**
　　与Windows XP和Windows 2003才七十多个服务相比，到[Windows 7](http://www.win7china.com/)时代，系统已经增加到一百五十多个服务（[Vista](http://www.vista123.com/)系统有130多个），这不可避免地加大了系统资源占用，拖慢了系统速度，占据了系统CPU和内存资源。
　　当然，在[Windows 7](http://www.win7china.com/) 的各个版本中，启动默认加载的服务数量是明显不同的，功能最多的是Ultimate版本（旗舰版），肯定加载的服务也最多。
　　Windows 系统的服务加载数量严重影响[Win7](http://www.win7china.com/)的开机速度，因此，优化服务就显得更加重要。
**2、如何设置、开启、关闭Windows7系统的各项服务**
　　1）用Win7系统自带的服务管理软件：
　　方式A、在Win7系统中随时按下 Win键+R键快捷键打开运行窗口，输入 Services.msc 回车；
![](http://img.win7china.com/NewsUploadFiles/20100327_132932_218_u.gif)
　　方式B、点击开始菜单——搜索框中输入“服务”两个汉字，或者输入 Services.msc 回车；
　　方式C、点击控制面板——管理工具——服务也可以到达同样的界面。
　　2）用[Windows7优化大师](http://www.win7china.com/windows7master/)里面的服务优化大师（或者[魔方](http://mofang.ithome.com/)的服务管理也可以），在系统优化菜单栏点击左侧的服务优化即可打开。
**3、用Win7优化大师中的向导进行设置**
　　打开[Win7优化大师](http://www.win7china.com/windows7master/)第一次都会自动弹出优化向导，如果没有弹出，请点击[Win7优化大师](http://www.win7china.com/windows7master/)左侧的“优化向导”，依次点击下一步到第四步——
![](http://img.win7china.com/NewsUploadFiles/20100327_133158_343_u.gif)
　　第四步里面罗列了3个建议关闭的服务（勾选中的服务保存后会被自动关闭并禁止下次启动加载）：
　　1）服务名称 Remote Registry ：本服务允许远程用户修改本机注册表，建议关闭；
　　2）服务名称 Secondary Logon：本服务替换凭据下的启用进程，建议普通用户关闭；
　　3）服务名称 SSDP Discovery：本服务启动家庭网络上的UPNP设备，建议关闭；
　　这四个服务普通用户都禁止掉行了，然后跟随向导进入第五步，这儿列出了6个建议关闭的服务：
![](http://img.win7china.com/NewsUploadFiles/20100327_133234_562_u.gif)
　　1）服务名称 IP Helper：如果您的网络协议不是IPV6，建议关闭此服务；
　　2）服务名称 IPsec Policy Agent：使用和管理IP安全策略，建议普通用户关闭；
　　3）服务名称 System Event Notification Service：记录系统事件，建议普通用户关闭；
　　4）服务名称 Print Spooler：如果您不使用打印机，建议关闭此服务；
　　5）服务名称 Windows Image Acquisition（WIA）：如果不使用扫描仪和数码相机，建议关闭此服务；
　　6）服务名称 Windows Error Reporting Service：当系统发生错误时提交错误报告给微软，建议关闭此服务；
　　这儿选中你不需要的服务，保存，OK，向导中关于不常用的服务列表就到此结束了！这儿还是友情提示一下，一定要看清楚向导里面的那些描述文字哦！
**4、使用服务优化大师进行服务管理**
　　是的，刚才提到，[Windows7优化大师](http://www.win7china.com/windows7master/)中自带了服务管理，和Win7系统自带的相比，Win7服务优化大师的最大好处是，可以对当前的服务方案进行保存备份成你最爱的方案，也可以还原（导入）你之前保存的方案，当然还有还原按钮，可以随时恢复还原到系统初始默认的服务设置状态下。
　　打开[Windows7优化大师](http://www.win7china.com/windows7master/)，点击上面的“系统优化”，再点击左侧的服务优化，OK，打开了Win7服务优化大师。如下图所示：
![](http://img.win7china.com/NewsUploadFiles/20100327_133625_156_u.jpg)
　　Win7服务管理大师还可以自行筛选服务状态，或者点击各个标题栏进行排序等等，比系统自带的服务管理功能要好很多。
**5、全面了解Windows 7系统的百余个服务**
　　OK，上面说了一些适用于初级用户的向导化操作，还给大家介绍了[Win7优化大师](http://www.win7china.com/windows7master/)中自带的一个服务优化大师，那么，用户最关心的还有这些问题——
　　Windows系统里面到底哪些服务可以被禁用掉？
　　某个服务知道名字，不知道是干啥用的啊？
　　好，大家看看下面的列表——
　　在下面的列表里面，大家请注意一下各种颜色，**服务名称** 描述  **最佳建议****第二建议***服务路径*
**.NET Runtime Optimization Service v2.0.50215_X86**
对使用下一代技术编写的程序的运行提供优化。该服务的默认运行方式是手动，**不建议更改**。
*\\WINDOWS\Microsoft.NET\Framework\v2.0.50215\mscorsvw.exe*
估计使用该技术之后，运行.Net技术编写的程序时执行效率将会更高，因为现有系统中运行.Net程序的速度还是有些慢。
**Application Experience**
在应用程序启动时处理应用程序兼容型查询请求。该服务的默认运行方式是自动，**不建议更改**。
*\\WINDOWS\system32\svchost.exe -k netsvcs*
估计使用该技术之后在运行老程序的时候系统会自动选择相应的兼容模式运行，以便取得最佳效果。
**Application Host Helper Service**
为 IIS 提供管理服务，例如配置历史记录和应用程序池帐户映射。如果停止此服务，则配置历史记录将无法工作，也无法用特定于应用程序池的访问控制项锁定文件或目录。
***\\Windows\system32\svchost.exe -k apphost***
IIS相关的东西，如果普通用户**建议禁用**。
**Application Information**
为应用程序的运行提供信息。该服务的默认运行方式是手动，**不建议更改**。
*\\WINDOWS\system32\svchost.exe -k netsvcs*
**Application Layer Gateway Service**
为 Internet 连接共享提供第三方协议插件的支持。该服务的默认运行方式是手动，如果你连接了网络，则该服务会自动启动，**不建议更改**。
*\\WINDOWS\System32\alg.exe*
Windows XP中也有该服务，作用也差不多，是系统自带防火墙和开启ICS共享上网的依赖服务，**如果装有第三方防火墙且不需要用ICS方式共享上网，完全可以禁用掉**。
**Application Management**
为活动目录的智能镜像(IntelliMirror)组策略程序提供软件的安装、卸载和枚举等操作。如果该服务停止，用户将无法安装、删除或枚举任何使用智能镜像方式安装的程序。如果该服务被禁用，任何依赖该服务的其他服务都将无法运行。该服务默认的运行方式为手动，该功能主要适用于大型企业环境下的集中管理，因此**家庭用户可以放心禁用**该服务。
*\\WINDOWS\system32\svchost.exe -k netsvcs*
**Background Intelligent Transfer Service**
在后台传输客户端和服务器之间的数据。如果禁用了BITS，一些功能，如 Windows Update，就无法正常运行。该服务的默认运行方式是自动，这个服务的主要用途还是用于进行Windows Update或者自动更新，**最好不要更改这个服务**。
*\\WINDOWS\System32\svchost.exe -k netsvcs*
**Base Filtering Engine**
基本筛选引擎(BFE)是一种管理防火墙和 Internet 协议安全(IPsec)策略以及实施用户模式筛选的服务。停止或禁用 BFE 服务将大大降低系统的安全。还将造成 IPsec 管理和防火墙应用程序产生不可预知的行为。**建议保持默认**。
*\\WINDOWS\system32\svchost.exe -k LocalServiceNetworkRestricted*
同样为系统防火墙，VPN以及IPsec提供依赖服务，同时也是系统安全方面的服务，如果使用第三方VPN拨号软件并且不用系统的防火墙以及ICS共享上网，**为了系统资源，关闭它吧**，**否则就别动它**。
**Block Level Backup Engine Service**
执行块级备份和恢复的引擎。
*\\Windows\system32\wbengine.exe*
估计是和备份恢复方面用的服务，无任何依赖关系，默认是手动，也从来没有看他启动过。就放那吧，**不用管了**。
**Certificate Propagation**
为智能卡提供证书。该服务的默认运行方式是手动。**如果你没有使用智能卡，那么可以放心禁用该服务**。
*\\WINDOWS\system32\svchost.exe -k netsvcs*
估计到Vista发布的时候，密码已经不是唯一可以用来区分用户身份的凭据了，也许智能卡，或者更加方便的生物识别技术，例如指纹、虹膜识别等应用将会更加广泛。
**CNG Key Isolation**
CNG 密钥隔离服务宿主在 LSA 进程中。如一般原则所要求，该服务为私钥和相关加密操作提供密钥进程隔离。该服务在与一般原则要求相一致的安全进程中存储和使用生存期长的密钥。
*\\Windows\system32\lsass.exe*
如果Wired AutoConfig/WLAN AutoConfig两个服务被打开，而且使用了EAP (Extensible Authentication Protocol)，那么这个服务将被使用，**建议不使用自动有线网络配置和无线网络的可以关掉**。
**COM+ Event System**
支持系统事件通知服务(SENS)，此服务为订阅组件对象模型(COM)组件事件提供自动分布功能。如果停止此服务，SENS 将关闭，而且不能提供登录和注销通知。如果禁用此服务，显式依赖此服务的其他服务将无法启动。一个很原始的古老服务，该服务的默认运行方式为自动，这是一个重要的系统服务，设为手动也会自动运行，设为禁用好像也没什么影响，但是日志中会出现大量的错误。**我们最好不要乱动**。
*\\WINDOWS\system32\svchost.exe -k netsvcs*
**COM+ System Application**
管理基于COM+ 组件的配置和跟踪。如果服务停止，大多数基于 COM+ 组件将不能正常工作。开发的比较清楚，以前的COM+程序设置IIS/.NET中的应用都会用到这个服务。**只要不设置为禁用就行了**，基本上也是很少运行的服务。
*\\WINDOWS\system32\dllhost.exe /Processid :{ 02D4B3F1-FD88-11D1-960D-00805FC79235}*
**Computer Browser**
维护网络上计算机的更新列表，并将列表提供给计算机指定浏览。如果服务停止，列表不会被更新或维护。如果服务被禁用，任何直接依赖于此服务的服务将无法启动。该服务的默认运行方式为自动，**不过如果你没有使用局域网或者你根本就不想使用局域网，该功能就可以放心禁用**，禁用后任然可以使用\\IP这样的UNC路径访问其他共享的计算机。
*\\WINDOWS\system32\svchost.exe -k netsvcs*
**Cryptographic Services**
提供三种管理服务: 编录数据库服务，它确定 Windows 文件的签字; 受保护的根服务，它从此计算机添加和删除受信根证书机构的证书;和密钥(Key)服务，它帮助注册此计算机获取证书。如果此服务被终止，这些管理服务将无法正常运行。如果此服务被禁用，任何依赖它的服务将无法启动。维护和管理系统的所有证书，密钥以及安全数据库。另外访问一些网站所需要的服务，比如微软的网站，Windows Update，或者DRM的网站，很多时候它会提供和确认Windows文件的签名信息。**强烈建议也是必须不能去动它，永远别想禁用这个服务**。
*\\WINDOWS\system32\svchost.exe -k netsvcs*
**DCOM Server Process Launcher**
为 DCOM 服务提供加载功能。该服务的默认运行方式是自动，最好不要乱动。以前的DCOM服务，也就是远程服务，是比COM+更基本的服务，看看注册表就知道Windows系统中有多少DCOM组件，虽然禁用也没什么问题，但是临时用到的设为手动的服务会无法自动启动，而且任务栏的图标也会消失不见，**所以最好不要修改这个选项**。
*\\WINDOWS\system32\svchost.exe -k DcomLaunch*
**Desktop Window Manager Session Manager**
Aero风格必须的，所有Aero Glass和Flip 3D效果均依赖这个服务。**如果喜欢这个风格就要设为自动，否则就禁用吧**。
*\\WINDOWS\system32\svchost.exe -k NetworkService*
DFS Replication
使您能够跨局域网或广域网 (WAN) 网络连接同步多台服务器上的文件夹。此服务使用远程差分压缩 (RDC) 协议只更新自上次复制之后更改的部分文件，分布式文件复制，从2003 R2就有的功能，**如果你不需要从局域网上复制大文件，才可以考虑禁用它**。
*\\Windows\system32\DFSR.exe*
**DHCP Client**
通过注册和更改 IP 地址以及 DNS 名称来管理网络配置。如果该服务停止了，这台计算机将无法收到动态IP地址以及DNS的更新。如果该服务被禁用了，那么任何依赖该服务的其他服务都将无法运行。该服务的默认运行方式是自动，**如果是手动指定的IP，完全可以禁用**。只有WinHTTP Web Proxy Auto-Discovery Service这个服务依赖它。
*\\Windows\system32\svchost.exe -k LocalServiceNetworkRestricted*
**Diagnostic Policy Service**
Diagnostic Policy服务为Windows组件提供诊断支持。如果该服务停止了，系统诊断工具将无法正常运行。如果该服务被禁用了，那么任何依赖该服务的其他服务都将无法正常运行。该服务的默认运行方式是自动，Vista或IE7有时会弹出对话框问你是否需要让它帮忙找到故障的原因，只有1%的情况下它会帮忙修复Internet断线的问题，**可以关掉**。
*\\WINDOWS\System32\svchost.exe -k netsvcs*
**Diagnostic Service Host**
诊断服务主机服务启用 Windows 组件的问题检测、故障排除和解决方案。如果停止该服务，则一些诊断将不再发挥作用。如果禁用该服务，则显式依赖它的所有服务将无法启动。这就是帮上面Diagnostic Policy Service做具体事情的服务，会随着上面的服务启动，**可以一起关掉**。
*\\Windows\System32\svchost.exe -k wdisvc*
**Diagnostic System Host**
诊断系统主机服务启用 Windows 组件的问题检测、故障排除和解决方案。如果停止该服务，则一些诊断将不再发挥作用。如果禁用该服务，则显式依赖它的所有服务将无法启动。**基本和Diagnostic Policy Service/Diagnostic Service Host是同类，可以一起关掉**。
*\\Windows\System32\svchost.exe -k LocalSystemNetworkRestricted*
**Distributed File System Replication**
在多个系统之间的NTFS文件系统上提供多宿主复制(multi-master replication)。该服务的默认运行方式是手动，最好不要乱动。 **虽然禁用也没什么影响，但是建议不要设置为禁用**。
*\\WINDOWS\system32\DFSR.exe*
**Distributed Link Tracking Client:**
在计算机内 NTFS 文件之间保持链接或在网络域中的计算机之间保持链接。该服务的默认运行方式是自动，不过这个功能一般都用不上，**完全可以放心禁用**。
*\\WINDOWS\System32\svchost.exe -k netsvcs*
**Distributed Transaction Coordinator**
在多个来源，例如数据库、消息队列以及文件系统之间进行传送协调。如果该服务停止，这些传输将不会发生。如果该服务被禁用，任何依赖该服务的其他服务都将无法运行。很多应用以及SQL，Exchange Biztalk等服务器软件都依赖这个服务，**可以不启动它，但不要Disabled 它**。
*\\Windows\system32\msdtc.exe*
**DNS Client**
为此计算机解析和缓冲域名系统 (DNS) 名称。如果此服务被停止，计算机将不能解析 DNS 名称并定位 Active Directory 域控制器。如果此服务被禁用，任何明确依赖它的服务将不能启动。该服务的默认运行方式是自动，**如果是在域的环境中要设置为自动**，但是这个服务可以泄露你浏览过哪些网站，所以**一般用户出于安全考虑，禁用了吧**。
*\\WINDOWS\system32\svchost.exe -k NetworkService*
**Extensible Authentication Protocol**
可扩展的身份验证协议(EAP)服务在以下情况下提供网络身份验证: 802.1x 有线和无线、VPN 和网络访问保护(NAP)。EAP 在身份验证过程中也提供网络访问客户端使用的应用程序编程接口(API)，包括无线客户端和 VPN 客户端。如果禁用此服务，该计算机将无法访问需要 EAP 身份验证的网络。**不用802.1x认证、无线网络或VPN 可以不启动它，不要Disabled 它**。
*\\Windows\System32\svchost.exe -k netsvcs*
**Fax**
利用计算机或网络上的可用传真资源发送和接收传真。不用我说了吧，很明显就能看出来是和传真有关的。**手动或禁用**
*\\Windows\system32\fxssvc.exe*
**Function Discovery Provider Host**
功能发现提供程序的主机进程。PnP-X和SSDP相关，**如果无相关设备就关了吧**。
*\\Windows\system32\svchost.exe -k LocalService*
**Function Discovery Resource Publication**
发布该计算机以及连接到该计算机的资源，以便能够在网络上发现这些资源。如果该服务被停止，将不再发布网络资源，网络上的其他计算机将无法发现这些资源。PnP-X和SSDP相关，**如果无相关设备就关了吧**。
*\\Windows\system32\svchost.exe -k LocalService*
**Group Policy Client**
该服务负责通过组策略组件应用管理员为计算机和用户配置的设置。如果停止或禁用该服务，将无法应用设置，并且将无法通过组策略管理应用程序和组件。如果停止或禁用该服务，依赖于组策略的任何组件或应用程序都将无法正常运行。**你无法关闭这个服务**。
*\\Windows\system32\svchost.exe -k GPSvcGroup*
**Health Key and Certificate Management**
为网络访问保护代理(NAPAgent)提供 X.509 证书和密钥管理服务。使用 X.509 证书的强制技术在没有此服务的情况下可能无法正常工作。推测是NAP的一个服务，其中提到要实现一个Health Registration Authority机制。默认即可。
*\\Windows\System32\svchost.exe -k netsvcs*
**Human Interface Device Access**
启用对智能界面设备(HID)的通用输入访问，它激活并保存键盘、远程控制和其它多媒体设备上的预先定义的热按钮。如果此服务被终止，由此服务控制的热按钮将不再运行。如果此服务被禁用，任何依赖它的服务将无法启动。**如果你不想你机器或笔记本键盘上面的那些特别的附加按键起作用、不用游戏手柄之类可以关掉这个服务。**
*\\Windows\system32\svchost.exe -k LocalSystemNetworkRestricted*
**IKE and AuthIP IPsec Keying Modules**
IKEEXT 服务托管 Internet 密钥交换(IKE)和身份验证 Internet 协议(AuthIP)键控模块。这些键控模块用于 Internet 协议安全(IPSec)中的身份验证和密钥交换。停止或禁用 IKEEXT 服务将禁用与对等计算机的 IKE/AuthIP 密钥交换。通常将 IPSec 配置为使用 IKE 或 AuthIP，因此停止或禁用 IKEEXT 服务将导致 IPSec 故障并且危及系统的安全。强烈建议运行 IKEEXT 服务。主要是针对VPN等网络环境的进行认证。**不用VPN或用第三方VPN拨号的话可以禁用**。
*\\Windows\system32\svchost.exe -k netsvcs*
**Interactive Services Detection**
启用交互式服务的用户输入的用户通知，这样当交互式服务创建的对话框出现时可以访问这些对话框。如果此服务已停止，将不再有新的交互式服务对话框通知，而且可能再也无法访问交互式服务对话框。如果此服务已禁用，则不再有新的交互式服务对话框通知，也无法访问这些对话框。我也不清楚什么算交互式服务，默认也是Manual，**保持默认吧**。
*\\Windows\system32\UI0Detect.exe*
**Internet Connection Sharing (ICS)**
为家庭和小型商业网络提供网络地址转换、寻址、名称解析以及/或入侵防御服务。该服务的默认运行方式是禁用，**如果你不打算让这台计算机充当ICS主机，那么该服务可以禁用，否则需要启用**。
*\\WINDOWS\System32\svchost.exe -k netsvcs*
**IP Helper**
在 IPv4 网络上提供自动的 IPv6 连接。如果停止此服务，则在计算机连接到本地 IPv6 网络时，该计算机将只具有 IPv6 连接。主要是提供IPv6的支持，说白了就是让IPv4和IPv6相互兼容，现在的情况下不是特别需要，**其实设置成Disabled 也无妨**。
*\\Windows\System32\svchost.exe -k NetSvcs*
**IPsec Policy Agent**
Internet 协议安全(IPSec)支持网络级别的对等身份验证、数据原始身份验证、数据完整性、数据机密性(加密)以及重播保护。此服务强制执行通过 IP 安全策略管理单元或命令行工具 “netsh ipsec” 创建的 IPSec 策略。停止此服务时，如果策略需要连接使用 IPSec，可能会遇到网络连接问题。同样，此服务停止时，Windows 防火墙的远程管理也不再可用。**某些公司的网络环境要求必须打开**，它提供一个TCP/IP网络上客户端和服务器之间端到端的安全连接。**其他的情况建议设置成禁用**。
*\\Windows\system32\svchost.exe -k NetworkServiceNetworkRestricted*
**KtmRm for Distributed Transaction Coordinator**
协调 MSDTC 和核心事务管理器(KTM)之间的事务。Vista提供的另外一种事务服务，对开发人员来说是比较有用，对于一般的用户或者非开发人员来说，**设置成手动**。
*\\Windows\System32\svchost.exe -k NetworkService*
**Link-Layer Topology Discovery Mapper**
创建网络映射，它由 PC 和设备拓扑(连接)信息以及说明每个 PC 和设备的元数据组成。如果禁用此服务，则网络映射将不能正常工作。应该是支持LLTD (Link Layer Topology Discovery) 技术，可以精确地显示支持LLTD的设备在网络结构中的位置，比如Vista的无线地图，**保持默认手动**。
*\\Windows\System32\svchost.exe -k LocalService*
**Microsoft .NET Framework NGEN v2.0.50727_X86**
NET开发人员都知道NGEN的用法，**保持默认的设置**，以后会有很多基于.NET FX3的应用，那么这个服务会有用的。
*\\Windows\Microsoft.NET\Framework\v2.0.50727\mscorsvw.exe*
**Microsoft iSCSI Initiator Service**
管理从这台计算机到远程 iSCSI 目标设备的 Internet SCSI (iSCSI)会话。如果该服务已停止，则该计算机将无法登录或访问 iSCSI 目标设备。如果该服务已禁用，则所有显式依赖于该服务的服务将不会启动。**如果本机没有iSCSI设备也不需要连接和访问远程iSCSI设备，设置成禁用**。
*\\Windows\system32\svchost.exe -k netsvcs*
**Microsoft Software Shadow Copy Provider**
管理卷影复制服务制作的基于软件的卷影副本。如果该服务被停止，将无法管理基于软件的卷影副本。如果该服务被禁用，任何依赖它的服务将无法启动。卷影拷贝，**不要贸然Disabled它**，因为有时也许它会起作用，**如果确实不需要就设置为禁用吧**。
*\\Windows\System32\svchost.exe -k swprv*
**Multimedia Class Scheduler**
基于系统范围内的任务优先级启用工作的相对优先级。这主要适用于多媒体应用程序。如果此服务停止，个别任务将使用其默认的优先级。主要是针对一些多媒体应用的音／视频流设置优先级，禁用可能会导致声卡功能出现问题，**建议打开这个服务**，设成手动一般也会自动启动。
*\\Windows\system32\svchost.exe -k netsvcs*
**Net.Msmq 侦听器适配器**
通过 net.msmq 协议和 msmq.formatname 协议接收到激活请求，并将其传递给 Windows 进程激活服务。WCF要用的，**一般用户和非开发人员，还是Disabled就行了**。
*\\Windows\Microsoft.NET\Framework\v3.0\Windows Communication Foundation\SMSvcHost.exe -NetMsmqActivator*
**Net.Pipe 侦听器适配器**
通过 net.pipe 协议接收激活请求，并将其传递给 Windows 进程激活服务。WCF要用的，**一般用户和非开发人员，还是Disabled就行了**。
***\\Windows\Microsoft.NET\Framework\v3.0\Windows Communication Foundation\SMSvcHost.exe***
**Net.Tcp 端口共享服务**
提供通过 net.tcp 协议共享 TCP 端口的功能。WCF要用的，**一般用户和非开发人员，还是Disabled就行了**。
***\\Windows\Microsoft.NET\Framework\v3.0\Windows Communication Foundation\SMSvcHost.exe***
**Net.Tcp 侦听器适配器**
通过 net.tcp 协议接收激活请求，并将其传递给 Windows 进程激活服务。WCF要用的，**一般用户和非开发人员，还是Disabled就行了**。
*\\Windows\Microsoft.NET\Framework\v3.0\Windows Communication Foundation\SMSvcHost.exe*
**Netlogon**
为用户和服务身份验证维护此计算机和域控制器之间的安全通道。如果此服务被停用，计算机可能无法验证用户和服务身份并且域控制器无法注册 DNS 记录。如果此服务被禁用，任何依赖它的服务将无法启动。登陆活动目录时，和域服务通讯验证的一个服务，一般验证通过之后，域服务器会注册你的DNS记录，推送软件补丁和策略等等，**登陆域会用到它**。**工作组环境可以设为禁用**。
*\\Windows\system32\lsass.exe*
**Network Access Protection Agent**
在客户端计算机上启用网络访问保护(NAP)功能，这是NAP架构中的客户端，**默认设置即可**。
*\\Windows\System32\svchost.exe -k NetworkService*
**Network Connections**
管理“网络和拨号连接”文件夹中对象，在其中您可以查看局域网和远程连接。当你点击网络和拨号连接时这个服务就开始工作，主要是获得局域网和远程连接的对象，只要你联网这个服务就会启动。**不要关闭它**。
*\\Windows\System32\svchost.exe -k LocalSystemNetworkRestricted*
**Network List Service**
识别计算机已连接的网络，收集和存储这些网络的属性，并在更改这些属性时通知应用程序。这个服务是列举现有的网络，展示目前的连接状态。关闭它会导致网络不正常，所以**不要关闭它**。
*\\Windows\System32\svchost.exe -k LocalService*
**Network Location Awareness**
收集和存储网络的配置信息，并在此信息被修改时向程序发出通知。如果停止此服务，则配置信息可能不可用；如果禁用此服务，则显式依赖此服务的所有服务都将无法启动。就是NLA，能够很好的支持和标示多网卡，或者是你从家庭、个人、公司的网络中进行切换和变化时，给你提供增强的功能，大多数情况会随着Network Connections自动启动。和XP的NLA不同，关闭它网络正常但是会提示没插网线，**最好不要关闭**。
*\\Windows\System32\svchost.exe -k NetworkService*
**Network Store Interface Service**
此服务向用户模式客户端发送网络通知(例如，添加/删除接口等)。停止此服务将导致丢失网络连接。如果禁用此服务，则显式依赖此服务的所有其他服务都将无法启动。这是支持NLA的一个服务，比如保存每个网络的Profile，所以它的运行状态会和NLA相同，**最好不要关闭**。
*\\Windows\system32\svchost.exe -k LocalService*
**Offline Files**
脱机文件服务在脱机文件缓存中执行维护活动，响应用户登录和注销事件，实现公共 API 的内部部分，并将相关的事件分配给关心脱机文件活动和缓存更改的用户。脱机文件服务，使用这个功能系统会将网络上的共享内容在本地进行缓存，**可以关掉**。
*\\Windows\System32\svchost.exe -k LocalSystemNetworkRestricted*
**Parental Controls**
此服务将在系统中启用 Windows 家长控制。如果此服务未运行，则家长控制将不会工作。主要用来让家长为子女的账户设置使用时间、访问网络及电脑游戏的限制，自己用就不必作限制了。**禁用**。
*\\Windows\system32\svchost.exe -k LocalServiceNetworkRestricted*
**Peer Name Resolution Protocol**
在 Internet 上启用无服务器对等名称解析。如果被禁用，则某些点对点应用程序和协作应用程序(如 Windows 会议) 可能无法运行。**如果你不尝试WCF的P2P功能或开发，那么连同下面两个服务都可以关掉**。
*\\Windows\System32\svchost.exe -k LocalServiceNetworkRestricted*
**Peer Networking Grouping**
提供对等网络分组服务。**如果你不尝试WCF的P2P功能或开发，那么连同下面一个和上面一个服务都可以关掉**。
*\\Windows\System32\svchost.exe -k LocalServiceNetworkRestricted*
**Peer Networking Identity Manager**
提供对等网络标识服务。**如果你不尝试WCF的P2P功能或开发，那么连同上面两个服务都可以关掉**。
*\\Windows\System32\svchost.exe -k LocalServiceNetworkRestricted*
**Performance Logs & Alerts**
性能日志和警报根据预配置的计划参数从本地或远程计算机收集性能数据，然后将该数据写入日志或触发警报。如果停止此服务，将不收集性能信息。如果禁用此服务，则明确依赖它的所有服务将无法启动。Event Log和任务调度器等多个服务会用到它，个人认为它也是比较耗费资源的，**但不建议设置成禁用**。
*\\Windows\System32\svchost.exe -k LocalServiceNoNetwork*
**Plug and Play**
使计算机在极少或没有用户输入的情况下能识别并适应硬件的更改。终止或禁用此服务会造成系统不稳定。即插即用，最基本的服务之一，**想关也关不了**。
*\\Windows\system32\svchost.exe -k DcomLaunch*
**PnP-X IP Bus Enumerator**
PnP-X 总线枚举器服务管理虚拟网络总线。该服务使用 SSDP/WS 发现协议来发现网络连接设备并使其存在于 PnP 中。如果停止或禁用此服务，则 NCD 设备将不会继续保持在 PnP 中。所有基于 pnpx 的方案都将停止运行。PnP-X 总线枚举服务器-Windows Connect Now(WCN)，即微软网络和装置平台的组件之一，它是即插即用的扩展，支持某些联网的智能家电装置(比如能联网的电饭锅、冰箱)连接到你的PC上面。**目前还用不上，关了它！**
*\\Windows\system32\svchost.exe -k LocalSystemNetworkRestricted*
**PNRP Machine Name Publication Service**
此服务使用对等名称解析协议发布计算机名称。配置是通过 Netsh 上下文“p2p pnrp peer”管理的。这个是用来对P2P网络中发布服务器进行命名解析的，一般不需要它。**默认即可**。
*\\Windows\System32\svchost.exe -k LocalServiceNetworkRestricted*
**Portable Device Enumerator Service**
为可移动大容量存储设备强制组策略。使应用程序(例如 Windows Media Player 和图像导入向导)能够使用可移动大容量存储设备传输和同步内容。用来让Windows Media Player和移动媒体播放器比如MP3进行数据和时钟同步。**如不需要同步建议关闭**。
*\\Windows\system32\svchost.exe -k LocalSystemNetworkRestricted*
**Print Spooler**
将文件加载到内存供稍后打印。打印服务，不用多说了，**有（包括虚拟的）就开，没有就关**。
*\\Windows\System32\spoolsv.exe*
**Problem Reports and Solutions Control Panel Support**
此服务为查看、发送和删除“问题报告和解决方案”控制面板的系统级问题报告提供支持。开了它基本也解决不了你计算机出的问题。**禁用吧**。
*\\Windows\System32\svchost.exe -k netsvcs*
**Program Compatibility Assistant Service**
为程序兼容性助手提供支持。如果此服务停止，则程序兼容性助手不能正常发挥作用。如果此服务被禁用，则依赖于它的所有服务都将无法启动。如果你使用到Program Compatibility Assistant或者需要将你的程序设置成兼容模式运行，比如运行在Win98 或 Windows 2000的方式下，就修改成自动，**强烈建议设置为自动**。
*\\Windows\system32\svchost.exe -k LocalSystemNetworkRestricted*
**Protected Storage**
为敏感数据(如密码)提供保护存储，以防止未授权的服务、进程或用户访问。2000/XP流传下来的服务，尽管用处不大，**但为了安全还是保留着吧**。
*\\Windows\system32\lsass.exe*
**Quality Windows Audio Video Experience**
质量 Windows 音频视频体验(qWave)是用于 IP 家庭网络上的音频视频(AV)流应用程序的网络平台。通过确保 AV 应用程序的网络服务质量(QoS)，qWave 增强了 AV 流的性能和可靠性。它提供了许可控制机制、运行时监视和实施、应用程序反馈以及流量优先顺序。主要用于改善和加强IP网络上的音频视频流的传输和播放质量，控制流量，个人感觉这个不起什么作用，支持这样技术的网络服务也不多。**还是系统资源比较重要，关了它**。
*\\Windows\system32\svchost.exe -k LocalService*
**ReadyBoost**
使用 ReadyBoost 为改进系统性能提供支持。Vista的新功能，可以通过高速U盘之类移动存储设备较高的随机读写速度来加快的系统启动和运行，具体原理我还不能理解，但显然不是拿U盘当作内存用。如果没有这类设备就关掉它，其实一般用笔记本电脑也不会总插着个U盘到处跑。**一句话，用就开着，不用就关掉**。
*\\Windows\system32\svchost.exe -k LocalSystemNetworkRestricted*
**Remote Access Auto Connection Manager**
无论什么时候当某个程序引用一个远程 DNS 或 NetBIOS 名或者地址就创建一个到远程网络的连接。当你的机器获得一个DNS或NetBIOS的名称或地址时会创建一个到远程网络的连接，一般ADSL拔号和VPN接入都会用到这个服务。如果设置成禁用，就建不了连接了。**还是手动比较好**。
*\\Windows\system32\svchost.exe -k netsvcs*
**Remote Access Connection Manager**
管理从这台计算机到 Internet 或其他远程网络的拨号和虚拟专用网络(VPN)连接。如果禁用该项服务，则明确依赖该服务的任何服务都将无法启动。创建连接的时候使用，ADSL/VPN/其他什么拨号网络都会用到这个服务。关了的话就不能上网了，**不要关闭**
*\\Windows\system32\svchost.exe -k netsvcs*
**Remote Procedure Call (RPC)**
作为终结点映射程序(endpoint mapper)和 COM 服务控制管理器使用。如果此服务被停用或禁用，使用 COM 或远程过程调用(RPC)服务的程序工作将不正常。系统关键服务，**想关也不让你关**。
*\\Windows\system32\svchost.exe -k rpcss*
**Remote Procedure Call (RPC) Locator**
管理 RPC 名称服务数据库。配合RPC的服务，可以设置手动，**但不建议设置成禁用**。
*\\Windows\system32\locator.exe*
**Remote Registry**
使远程用户能修改此计算机上的注册表设置。如果此服务被终止，只有此计算机上的用户才能修改注册表。如果此服务被禁用，任何依赖它的服务将无法启动。**个人使用可以关掉它，公司管理就需要打开了**。
*\\Windows\system32\svchost.exe -k regsvc*
**Routing and Remote Access**
在局域网以及广域网环境中为企业提供路由服务。提供路由服务的。**不用就关**。
*\\Windows\system32\svchost.exe -k netsvcs*
**Secondary Logon**
在不同凭据下启用启动过程。如果此服务被停止，这种类型的登录访问将不可用。如果此服务被禁用，任何明确依赖它的服务都将不能启动。**允许一台机器同时有两个用户登录，个人应用基本不需要**。
*\\Windows\system32\svchost.exe -k netsvcs*
**Secure Socket Tunneling Protocol Service**
提供使用 VPN 连接到远程计算机的安全套接字隧道协议(SSTP)的支持。如果该服务被禁用，则用户将无法使用 SSTP 访问远程服务器。这个服务主要是VPN连接服务的，如果用第三方VPN客户端，可以关闭，但是Remote Access Connection Manager这个服务依赖这个服务，**还是别动吧**。
*\\Windows\system32\svchost.exe -k LocalService*
**Security Accounts Manager**
启动此服务将向其他服务发出信号: 安全帐户管理器(SAM)已准备就绪，可以接受请求。禁用此服务将导致在 SAM 准备就绪时，无法通知系统中的其他服务，从而可能导致这些服务无法正确启动。不应禁用此服务。系统的安全账户管理服务，关了就不能添加用户，修改用户密码等用户操作了，**建议默认别动它**。
*\\Windows\system32\lsass.exe*
**Security Center**
监视系统安全设置和配置。Vista新的安全中心，可以进行安全方面的设置，也监控系统的安全状况。这个服务只是个管理界面，打开其他安全服务即可。**可以关闭**。
*\\Windows\System32\svchost.exe -k LocalServiceNetworkRestricted*
**Server**
支持此计算机通过网络的文件、打印、和命名管道共享。如果服务停止，这些功能不可用。如果服务被禁用，任何直接依赖于此服务的服务将无法启动。保证本机接入网络的文件、打印机和命名管道共享管理，**如果不需要在网络上共享什么东西就可以关掉**。
*\\Windows\system32\svchost.exe -k netsvcs*
**Shell Hardware Detection**
为自动播放硬件事件提供通知。对于自动播放的设备或硬件提供通知，**如果你不喜欢Autoplay功能，那么设置成Manual**，这样你新插入一个U盘，可能系统没有任何提示。
*\\Windows\System32\svchost.exe -k netsvcs*
**SL UI Notification Service**
提供软件授权激活和通知。和Software Licensing 一起是用于Vista或其他一些软件激活服务的，不要想着将其设置成禁用，就可以不用激活Vista了。**默认吧**。
*\\Windows\system32\svchost.exe -k LocalService*
**Smart Card**
管理此计算机对智能卡的取读访问。如果此服务被终止，此计算机将无法取读智能卡。如果此服务被禁用，任何依赖它的服务将无法启动。Smart Card 服务，拨入公司网络、连接VPN等所必需的，**如果你没有应用的场景，建议设置成禁用**。
*\\Windows\system32\svchost.exe -k LocalService*
**Smart Card Removal Policy**
允许系统配置为移除智能卡时锁定用户桌面，**如果希望在用户拿走智能卡之后计算机锁定，那么打开这个服务；其他情况下设置成手动或关闭**。
*\\Windows\system32\svchost.exe -k netsvcs*
**SNMP Trap**
接收本地或远程简单网络管理协议 (SNMP) 代理程序生成的陷阱消息并将消息转发到此计算机上运行的 SNMP 管理程序。如果此服务被停用，此计算机上基于 SNMP 的程序将不会接收 SNMP 陷阱消息。如果此服务被禁用，任何依赖它的服务将无法启动。允许你的机器处理简单网络管理协议，很多网管协议是基于SNMP的。**不是网管的话建议关闭**。
*\\Windows\System32\snmptrap.exe*
**Software Licensing**
启用 Windows 和 Windows 应用程序的数字许可证的下载、安装和实施。如果禁用该服务，操作系统和许可的应用程序可能以缩减功能模式运行。负责Vista系统的License管理和验证，以及提供接口/API服务供Windows系统或其他应用程序使用。Vista的新增特性均会使用这个服务，如果设置成禁用可能会激活Vista的保护功能，导致系统的部分功能不可用。**强烈建议开启设为自动**。
*\\Windows\system32\SLsvc.exe*
**SSDP Discovery**
发现了使用 SSDP 发现协议的网络设备和服务，如 UPnP 设备。同时还公告了运行在本地计算机上的 SSDP 设备和服务。如果停止此服务，基于 SSDP 的设备将不会被发现。如果禁用此服务，任何显式依赖于它的服务都将无法启动。该服务在网络中搜索使用了SSDP发现协议的一些设备，比如一些非即插即用的设备，如果没有相关设备，**可以关了它**。
*\\Windows\system32\svchost.exe -k LocalService*
**Superfetch**
维护和提高一段时间内的系统性能。毫无疑问，这是Vista最好的功能之一，可以维护和提高系统的性能，**尽管效果不明显，但没有理由设置成其他的选项**。
*\\Windows\system32\svchost.exe -k LocalSystemNetworkRestricted*
**System Event Notification Service**
监视系统事件并通知订户这些事件的 COM+ 事件系统。SENS提供了一个唯一的系统追踪、通知的机制，使用于系统的登陆、设备连接、网络连接、电源和内部事件的订阅及通知，**不建议设置成关闭**。
*\\Windows\system32\svchost.exe -k netsvcs*
**Tablet PC Input Service**
启用 Tablet PC 笔和墨迹功能，**非Table PC及不使用手写板就可以关掉它**。
*\\Windows\System32\svchost.exe -k LocalSystemNetworkRestricted*
**Task Scheduler**
使用户能在此计算机上配置和制定自动任务的日程。如果此服务被终止，这些任务将无法在日程时间里运行。如果此服务被禁用，任何依赖它的服务将无法启动。已经不单是以前操作系统的计划任务调度管理器了，Vista和第三方的应用都会用到，**你无法关闭它**。
*\\Windows\system32\svchost.exe -k netsvcs*
**TCP/IP NetBIOS Helper**
提供 TCP/IP (NetBT) 服务上的 NetBIOS 和网络上客户端的 NetBIOS 名称解析的支持，从而使用户能够共享文件、打印和登录到网络。如果此服务被停用，这些功能可能不可用。如果此服务被禁用，任何依赖它的服务将无法启动。主要是支持 NetBIOS 名称的解析，使得你可以在计算机之间进行文件和打印机共享、网络登录。**不需要可关闭**。
*\\Windows\system32\svchost.exe -k LocalServiceNetworkRestricted*
**Telephony**
提供电话服务 API (TAPI)支持，以便各程序控制本地计算机上的电话服务设备以及通过 LAN 同样运行该服务的服务器上的设备。为应用程序提供 TAPI 的支持，很多时候这个服务会自己启动。**保持默认**。
*\\Windows\System32\svchost.exe -k NetworkService*
**Terminal Services**
允许用户以交互方式连接到远程计算机。远程桌面和终端服务器依赖此服务。若要防止远程使用此计算机，请清除“系统”属性控制面板项目的“远程”选项卡上的复选框。管理员账户的远程桌面、远程协助、远程终端服务或远程管理功能，很多时候这个服务会自动启动。**可以关闭，但建议保持默认**。
*\\Windows\System32\svchost.exe -k NetworkService*
**Terminal Services Configuration**
终端服务配置服务(TSCS)负责需要 SYSTEM 上下文的与所有终端服务和远程桌面相关的配置和会话维护活动。这些包括每会话临时文件夹、TS 主题和 TS 证书。管理员的远程桌面或进行远程管理设置，**如果不打算使用这些功能，可以设置成禁用。**
*\\Windows\System32\svchost.exe -k netsvcs*
**Terminal Services UserMode Port Redirector**
允许为 RDP 连接重定向打印机/驱动程序/端口，支持远程连接上的打印机/驱动器/端口重定向功能，如果不打算使用远程功能，**建议设置成禁用**。
*\\Windows\System32\svchost.exe -k LocalSystemNetworkRestricted*
**Themes**
为用户提供使用主题管理的经验。为了XP风格就要先启动这个服务，使用Aero更是如此。除非你用经典界面，**否则开启它**。
*\\Windows\System32\svchost.exe -k netsvcs*
**Thread Ordering Server**
提供特定期间内一组线程的排序执行。提供特别的线程排序和调度服务，如果用不上，**可以设置成手动，但不建议设置成自动**。
*\\Windows\system32\svchost.exe -k LocalService*
**TPM Base Services**
允许访问受信任的平台模块(TPM)，该模块向系统组件和应用程序提供基于硬件的加密服务。如果此服务已停止或禁用，应用程序将无法使用 TPM 保护的密钥。TPM是Trusted Platform Module的缩写，TPM平台会提供基于硬件的加密服务，如果关闭服务，那么Vista或应用程序可能无法访问或使用加密的密钥，**可以设置成手动**。
*\\Windows\System32\svchost.exe -k LocalService*
**UPnP Device Host**
允许 UPnP 设备宿主在此计算机上。如果停止此服务，则所有宿主的 UPnP 设备都将停止工作，并且不能添加其他宿主设备。如果禁用此服务，则任何显式依赖于它的服务将都无法启动。这是系统中通用即插即用的设备的宿主程序，它将作为通用即插即用的设备和操作系统通讯和工作的主体，**不建议设置成禁用**。
*\\Windows\system32\svchost.exe -k LocalService*
**User Profile Service**
此服务负责加载和卸载用户配置文件。如果已停止或禁用此服务，用户将无法再成功登录或注销，应用程序在获取用户数据时可能会出问题，而且为接收配置文件事件通知注册的组件将无法接收这些通知。**建议不要动它，否则会麻烦**。
*\\Windows\system32\svchost.exe -k netsvcs*
**Virtual Disk**
提供用于磁盘、卷、文件系统和存储阵列的管理服务。提供存储设备软件卷和硬件卷的管理，**不要将其设置成禁用**。
*\\Windows\System32\vds.exe*
**Volume Shadow Copy**
管理并执行用于备份和其它目的的卷影复制。如果此服务被终止，备份将没有卷影复制，并且备份会失败。如果此服务被禁用，任何依赖它的服务将无法启动。卷影复制，在Vista中和备份功能一起被调用，**不建议设置成禁用**。
*\\Windows\system32\vssvc.exe*
**WebClient**
使基于 Windows 的程序能创建、访问和修改基于 Internet 的文件。如果此服务被停止，这些功能将不可用。如果此服务被禁用，任何依赖它的服务将无法启动。**简单的说如果你不需要Web目录或类似功能，就可以关掉它**。
*\\Windows\system32\svchost.exe -k LocalService*
**Windows Audio**
管理基于 Windows 的程序的音频。如果此服务被停止，音频设备和效果将不能正常工作。如果此服务被禁用，任何依赖它的服务将无法启动。**除非你不想让电脑发声，否则就要自动启动它**。
*\\Windows\System32\svchost.exe -k LocalServiceNetworkRestricted*
**Windows Audio Endpoint Builder**
管理 Windows 音频服务的音频设备。如果此服务被停止，音频设备和效果将不能正常工作。如果此服务被禁用，任何依赖它的服务将无法启动，**除非你不想让电脑发声，否则就要自动启动它**。
*\\Windows\System32\svchost.exe -k LocalSystemNetworkRestricted*
**Windows Backup**
提供 Windows 备份和还原功能。Windows备份和版本恢复功能，一直都不好使，**可以关掉**。
*\\Windows\System32\svchost.exe -k SDRSVC*
**Windows CardSpace**
安全启用数字标识符的创建、管理和公开。像Smart Card一样的个人标识管理，.NET Framework 3.0提供的一个WCF编程模型。**一般用户关闭**。
*\\Windows\Microsoft.NET\Framework\v3.0\Windows Communication Foundation\infocard.exe*
**Windows Color System**
WcsPlugInService 服务宿主第三方 Windows 颜色系统颜色设备模型和 gamut 映射模型插件模块。这些插件模块是 Windows 颜色系统基线颜色设备和 gamut 映射模型的特定于供应商的扩展。停止或禁用 WcsPlugInService 服务将禁用此扩展功能，并且 Windows 颜色系统将使用其基线模型处理过程，而不是供应商所需的处理过程。这可能导致颜色显示不正确。色彩管理模块，Vista支持外挂的色彩管理。**默认即可**。
*\\Windows\system32\svchost.exe -k wcssvc*
**Windows Connect Now - Config Registrar**
作为注册器，为注册人颁发网络凭据。如果禁用该服务，则 Windows 立即连接 - 配置注册器将不能正常运行。**默认即可**。
*\\Windows\System32\svchost.exe -k LocalService*
**Windows Defender**
扫描计算机以找出可能不需要的软件，设置扫描，并获取最新可能不需要软件定义。Vista新功能之一，可以加强安全，防范木马和一些恶意程序，最主要的是免费。**不需要可以关闭**。
*\\Windows\System32\svchost.exe -k secsvcs*
**Windows Driver Foundation - User-mode Driver Framework**
管理用户模式驱动程序主机进程。管理用户模式驱动的主进程，如果关闭系统会出现很多问题，**建议不要轻易设置成禁用** 。
*\\Windows\system32\svchost.exe -k LocalSystemNetworkRestricted*
**Windows Error Reporting Service**
允许在程序停止运行或停止响应时报告错误，并允许提供现有解决方案。还允许为诊断和修复服务生成日志。如果此服务被停止，则错误报告将无法正确运行，而且可能不显示诊断服务和修复的结果。没人喜欢错误，对你和微软而言，错误报告传送过去都没什么用。**关了它**。
*\\Windows\System32\svchost.exe -k WerSvcGroup*
**Windows Event Collector**
此服务将管理对支持 WS-Management 协议的远程源中事件的永久订阅。这包括 Windows Vista 事件日志、硬件以及启用 IPMI 的事件源。该服务将转发的事件存储在本地活动日志中。如果停止或禁用此服务，将无法创建事件订阅，并且无法接受转发的事件。这个主要是性能收集分析和系统监控中的一些功能使用，也是Vista新的事件管理工具的支持服务。**默认即可**。
*\\Windows\system32\svchost.exe -k NetworkService*
**Windows Event Log**
此服务管理事件和事件日志。它支持日志记录事件、查询事件、订阅事件、归档事件日志以及管理事件元数据。它可以用 XML 和纯文本两种格式显示事件。停止该服务可能危及系统的安全性和可靠性。Vista和其他系统程序经常会用到，这个不是必须的服务，建议设置成Manual。**默认即可**。
*\\Windows\System32\svchost.exe -k LocalServiceNetworkRestricted*
**Windows Firewall**
Windows 防火墙通过阻止未授权用户通过 Internet 或网络访问您的计算机来帮助保护计算机，不用多说什么了。**如果不需要就禁用**。
*\\Windows\system32\svchost.exe -k LocalServiceNoNetwork*
**Windows Image Acquisition (WIA)**
为扫描仪和照相机提供图像采集服务。专门为扫描仪和数码相机等提供图像捕获和获取功能。**有就开着，没有就关**。
*\\Windows\system32\svchost.exe -k imgsvc*
**Windows Installer**
添加、修改和删除以 Windows Installer (*.msi)程序包提供的应用程序。如果禁用了此服务，任何完全依赖它的服务不会被启动。MSI安装包的服务，许多安装程序都需要这个服务，**设置成手动就好了**。
***\\Windows\system32\msiexec /V***
**Windows Management Instrumentation**
提供共同的界面和对象模式以便访问有关操作系统、设备、应用程序和服务的管理信息。如果此服务被终止，多数基于 Windows 的软件将无法正常运行。如果此服务被禁用，任何依赖它的服务将无法启动。系统管理服务， Vista启动初始化都会用到，即使设置成Manual，也会被启动。**不要去动它**。
***\\Windows\system32\svchost.exe -k netsvcs***
**Windows Media Center Extender Service**
允许 Windows Media Center 扩展器设备查找并连接到计算机。通过网络为Windows Media Extender（像ＸＢｏｘ）等传送多媒体文件，**建议禁止，除非你需要这个功能**。
***\\Windows\system32\svchost.exe -k LocalService***
**Windows Media Center Receiver Service**
电视或 FM 广播接收的 Windows Media Center 服务。**建议禁止，除非你需要这个功能**。
***\\Windows\ehome\ehRecvr.exe***
**Windows Media Center Scheduler Service**
在 Windows Media Center 中开始和停止录制电视节目。**建议禁止，除非你需要这个功能**。
***\\Windows\ehome\ehsched.exe***
**Windows Media Center Service Launcher**
如果在 Windows Media Center 中启用了电视，则在开机时启动 Windows Media Center 计划程序和 Windows Media Center 接收程序服务。**建议禁止，除非你需要这个功能**。
***\\Windows\system32\svchost.exe -k LocalServiceNoNetwork***
**Windows Media Player Network Sharing Service**
使用通用即插即用设备与其他网络播放机和媒体设备共享 Windows Media Player 媒体库。**建议禁止，除非你需要这个功能**
***\\Program Files\Windows Media Player\wmpnetwk.exe***
**Windows Modules Installer**
启用 Windows 更新和可选组件的安装、修改和移除。如果此服务被禁用，则此计算机的 Windows 更新的安装或卸载可能会失败。**Windows Updates 需要的，如果你不使用Windows Updates，那么可以禁止这个服务**。
***\\Windows\servicing\TrustedInstaller.exe***
**Windows Presentation Foundation Font Cache 3.0.0.0**
通过缓存常用的字体数据来优化 Windows 演示基础(WPF)应用程序的性能。WPF 应用程序将启动此服务(如果尚未启动)。可以禁用此服务，尽管这样做会降低 WPF 应用程序的性能。NET Framework 3.0中的WPF应用必须的，一般这个服务启动，证明你的机器上运行了新的WPF的应用。**默认即可**。
***\\Windows\Microsoft.Net\Framework\v3.0\WPF\PresentationFontCache.exe***
**Windows Process Activation Service**
Windows Process Activation Service (WAS)为通过消息激活的应用程序提供进程激活、资源管理和运行状况管理服务。不明。**建议默认**。
***\\Windows\system32\svchost.exe -k iissvcs***
**Windows Remote Management (WS-Management)**
Windows 远程管理(WinRM)服务执行 WS-Management 协议来实现远程管理。WS-Management 是用于远程软件和硬件管理的标准 Web 服务协议。WinRM 服务侦听网络上的 WS-Management 请求并对它们进行处理。通过组策略或使用 winrm.cmd 命令行工具的侦听程序，来配置 WinRM 服务，以使其可通过网络侦听。WinRM 服务提供对 WMI 数据的访问并启用事件集合。事件集合及对事件的订阅需要服务处于运行状态。传输 WinRM 消息时使用 HTTP 和 HTTPS
 协议。WinRM 服务不依赖于 IIS ，但在同一计算机上预配置为与 IIS 共享端口。WinRM 服务保留 /wsman URL 前缀。若要防止与 IIS 发生冲突，管理员应确保 IIS 上承载的所有网站均不使用 /wsman URL 前缀。允许从远程进行计算机管理或信息收集。**建议设置为手动**。
***\\Windows\System32\svchost.exe -k NetworkService***
**Windows Search**
为文件、电子邮件以及其他内容(通过可扩展性 API)提供内容索引和属性缓存。该服务响应文件和电子邮件通知，从而对已修改的内容编制索引。如果该服务已停止或被禁用，资源管理器将无法显示项目的虚拟文件夹视图，在资源管理器中搜索将回退为速度较慢的逐项搜索。新的桌面搜索功能，默认范围太小，扩大了又很耗费资源，可以试试。**建议手动**。
***\\Windows\system32\SearchIndexer.exe /Embedding***
**Windows Time**
维护在网络上的所有客户端和服务器的时间和日期同步。如果此服务被停止，时间和日期的同步将不可用。如果此服务被禁用，任何明确依赖它的服务都将不能启动。**和服务器同步时间的，一般我都关闭它**。
***\\Windows\system32\svchost.exe -k LocalService***
**Windows Update**
启用检测、下载和安装 Windows 和其他程序的更新。如果此服务被禁用，这台计算机的用户将无法使用 Windows Update 或其自动更新功能，并且这些程序将无法使用 Windows Update Agent (WUA) API。**Windows Update 这个功能取决于你了**，它和Background Intelligent Transfer Service、Modules Installer服务关联。
***\\Windows\system32\svchost.exe -k netsvcs***
**WinHTTP Web Proxy Auto-Discovery Service**
WinHTTP 实现了客户端 HTTP 堆栈并向开发人员提供 Win32 API 和 COM 自动化组件以供发送 HTTP 请求和接收响应。此外，通过执行 Web 代理自动发现(WPAD)协议，WinHTTP 还提供对自动发现代理服务器配置的支持。WPAD是一种协议，可以让HTTP客户端自动发现代理服务器配置，该服务使应用程序支持WPAD协议的应用，建议设置成Manual和Disabled，因为大多数的情况下不会用到。**建议关闭**。
***\\Windows\system32\svchost.exe -k LocalService***
**Wired AutoConfig**
此服务对以太网接口执行 IEEE 802.1X 身份验证。其实很多的时候，还是要自己设置的。**默认即可**。
***\\Windows\system32\svchost.exe -k LocalSystemNetworkRestricted***
**WLAN AutoConfig**
该服务枚举 WLAN 适配器，管理 WLAN 连接和配置文件。很多的时候，还是要自己设置的，**不用无线网络可以关掉**。
***\\Windows\system32\svchost.exe -k LocalSystemNetworkRestricted***
**WMI Performance Adapter**
Provides performance library information from Windows Management Instrumentation (WMI) providers to clients on the network. This service only runs when Performance Data Helper is activated. 微软的解释一堆英文，当每个中国人都过了英语六级吗？WMI信息转换，为性能工具、事件工具提供服务，**建议手动**。
***\\Windows\system32\wbem\WmiApSrv.exe***
**Workstation**
使用 SMB 协议创建并维护客户端网络与远程服务器之间的连接。如果此服务已停止，这些连接将无法使用。如果此服务已禁用，任何明确依赖它的服务将无法启动。创建和管理到远程服务器的网络连接，一般在网络环境中，特别是局域网中是一个必须的服务，**不需访问别人的共享资源时可以设为手动**。
***\\Windows\System32\svchost.exe -k LocalService***
**6、其他关于Win7系统服务的注意点**
是不是看花眼了？准备这个文章花了好久的功夫，不花眼才怪呢。为了尊重之家的劳动果实，记得保留[软媒](http://www.ruanmei.com/)或者[Win7之家](http://www.win7china.com/)（[www.win7china.com](http://www.win7china.com/)）的网站链接喔。下面说下其他的注意点。
Windows 7和[Vista](http://www.vista123.com/)在服务的启动上除了原先NT内核系统的Automatic，Manual，Disable选项之外，还多出了Automatic（Delayed Start），它的作用大概是延缓服务启动以减小系统载入时的负荷，使系统尽早进入用户响应状态后再启动一些非关键的服务。由于很多系统服务之间都是有连带关系的，这个选项用不好，例如将某些服务的先决服务配置为延迟启动，则可能拖慢整体的启动。考虑到软件应用环境的复杂性，通常不建议大家将服务设置为
 Disable，否则很可能在某一天用到某个应用时由于系统服务被禁用而发生异常并且得不到任何提示。因此在下文中只推荐使用Automatic和 Manual。某些情况下会发生系统服务设成Manual后，在软件需要时仍不能启动从而产生故障，到时各位需要仔细排查。某些关键的服 务是不能调整启动模式的，像Remote Procedure Call、Plug and Play等，而某些服务会被其他服务连带着一块启动，因而设成自动还是手动效果都是一样的。
**7、快速参考**
全部手动：将[Win7](http://www.win7china.com/)的系统服务全部调成Manual（手动）后，[Windows7](http://www.win7china.com/)进入了最原始状态，甚至连资源管理器和控制面板都无法正常运作，只能通过快捷键调用服务管理终端进行修改，各位还是不要尝试了。
最基本系统：Remote Procedure Call，Plug and Play，DCOM Server Process Launcher，User Profile Service，Windows Management Instrumentation将这几个服务设为自动，系统就算可用了，类似Windows 2000的界面。建议把Software Licensing服务也设为自动，这是[Win7](http://www.win7china.com/)正版验证和系统运作的重要一环，这个服务不启动，系统就和试用到期的盗版一样，下面的很多功能都启动不
 了。
访问网络：除了最基本系统里面的服务外，需要自动运行DHCP Client，DNS Client，Network Connections。
完整体验：除了最基本系统和访问网络里面的服务外，需要自动运行：Themes(打开它就有了XP界面)； Desktop Windows Manager Session Manager（再打开它就能开启Aero效果）；Windows Audio及Windows Audio Endpoint Builder（这两个相当于原来的Windows Audio，打开了系统才能发声）；Windows Search，Windows Defender, Windows Firewall,　Superfetch(这些都是Win7和Vista的新功能、新特性)。
其他服务自动运行建议：Distributed Link Tracking Client (使用NTFS文件系统)；Function Discovery Resource Publication， Server，Workstation（访问局域网共享资源）；Printer Spooler（使用打印机）；Group Policy Client（在集中管理的IT环境中）；IKE and AuthIP IPsec Keying Modules (网络认证与防护)；Task Scheduler（计划任务）；Windows
 Update（自动更新系统补丁）；ReadyBoost（如果你有容量大、速度快的U盘）。
World Wide Web Publishing Service,　IIS Admin Service, FTP Publishing Service ,MSMQ 服务都是IIS7的相关服务，默认没有安装，安装后一般都设置成Manual即可。
UAC 用户账户控制是[Win7](http://www.win7china.com/)系统安全性的重要一环（比[Vista](http://www.vista123.com/)时代的UAC要强劲很多），同微软官方一样，我也不建议你关闭它。不过要是它也弄得你像我一样抓狂的话，那还是把它关了吧。方法是开
 始——运行或（Win+R），输入msconfig,在最右侧的工具一栏里找到禁用UAC,执行即可。msconfig作为调整系统的一个入口，在[Win7](http://www.win7china.com/)中比先前系统更加强大好用。
对于大多数初级用户，建议把上面所有的启动均设置为自动，而不在列表中的均设置为手动，即可适应绝大多数环境，并在资源占用和功能上取得比较完美的均衡。
**8、写在服务优化的后面的话**
一定记得用[Windows7优化大师](http://www.win7china.com/windows7master/)中服务管理大师的备份和还原功能（[魔方](http://mofang.ithome.com/)中相同），保存自己的服务方案。实在遇到问题也不用怕，Win7服务管理大师里面有恢复Windows
 7系统默认服务的功能，上面的图中也用红线圈起来了。
这篇文章不仅仅是服务管理的宝典，大家有时候碰到系统的问题的时候，也能在这儿找到一些答案的。
哈，希望大家没有看晕，并能从中学到一点东西，这样，[Win7之家](http://www.win7china.com/)努力才没有白费。
OK，继续我们的征途，开机优化攻略之注册表清理。
