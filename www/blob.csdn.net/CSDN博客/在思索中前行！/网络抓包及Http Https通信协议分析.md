# 网络抓包及Http Https通信协议分析 - 在思索中前行！ - CSDN博客





2014年10月24日 13:38:19[_Tham](https://me.csdn.net/txl16211)阅读数：12062









之前写过一篇博客：[用 Fiddler 来调试HTTP，HTTPS](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html)。 这篇文章介绍另一个好用的抓包工具wireshark， 用来获取网络数据封包，包括http,TCP,UDP，等网络协议包。

记得大学的时候就学习过TCP的三次握手协议，那时候只是知道，虽然在书上看过很多TCP和UDP的资料，但是从来没有真正见过这些数据包， 老是感觉在云上飘一样，学得不踏实。有了wireshark就能截获这些网络数据包，可以清晰的看到数据包中的每一个字段。更能加深我们对网络协议的理解。
对我而言， wireshark 是学习网络协议最好的工具。

阅读目录

- [wireshark介绍](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#introduction)
- [wireshark不能做的](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#cannotdo)
- [wireshark VS Fiddler](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#wiresharkfiddler)
- [同类的其他工具](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#othertool)
- [什么人会用到wireshark](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#who)
- [wireshark 开始抓包](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#start)
- [wireshark 窗口介绍](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#mainui)
- [wireshark 显示过滤](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#filter)
- [保存过滤](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#savefilter)
- [过滤表达式](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#filterRegular)
- [封包列表(Packet List Pane)](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#PacketList)
- [封包详细信息 (Packet Details Pane)](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#packetDetails)
- [wireshark与对应的OSI七层模型](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#osi)
- [TCP包的具体内容](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#tcpdetails)
- [实例分析TCP三次握手过程](http://www.cnblogs.com/tankxiao/archive/2012/10/10/2711777.html#threehand)



# wireshark介绍

wireshark的官方下载网站： [http://www.wireshark.org/](http://www.wireshark.org/)

wireshark是非常流行的网络封包分析软件，功能十分强大。可以截取各种网络封包，显示网络封包的详细信息。

wireshark是开源软件，可以放心使用。 可以运行在Windows和Mac OS上。



使用wireshark的人必须了解网络协议，否则就看不懂wireshark了。

# Wireshark不能做的

为了安全考虑，wireshark只能查看封包，而不能修改封包的内容，或者发送封包。



# Wireshark VS Fiddler

Fiddler是在windows上运行的程序，专门用来捕获HTTP，HTTPS的。

wireshark能获取HTTP，也能获取HTTPS，但是不能解密HTTPS，所以wireshark看不懂HTTPS中的内容



总结，如果是处理HTTP,HTTPS 还是用Fiddler,  其他协议比如TCP,UDP 就用wireshark

# 同类的其他工具

微软的network monitor

sniffer 



# 什么人会用到wireshark

1. 网络管理员会使用wireshark来检查网络问题

2. 软件测试工程师使用wireshark抓包，来分析自己测试的软件

3. 从事socket编程的工程师会用wireshark来调试

4. 听说，华为，中兴的大部分工程师都会用到wireshark。



总之跟网络相关的东西，都可能会用到wireshark.

# wireshark 开始抓包

开始界面

![](http://pic002.cnblogs.com/images/2012/263119/2012100608483858.png)

wireshark是捕获机器上的某一块网卡的网络包，当你的机器上有多块网卡的时候，你需要选择一个网卡。

点击Caputre->Interfaces.. 出现下面对话框，选择正确的网卡。然后点击"Start"按钮, 开始抓包

![](http://pic002.cnblogs.com/images/2012/263119/2012100608494283.png)



# Wireshark 窗口介绍

![](http://pic002.cnblogs.com/images/2012/263119/2012100609122040.png)



WireShark 主要分为这几个界面

1. Display Filter(显示过滤器)，  用于过滤

2. Packet List Pane(封包列表)， 显示捕获到的封包， 有源地址和目标地址，端口号。 颜色不同，代表

3. Packet Details Pane(封包详细信息), 显示封包中的字段

4. Dissector Pane(16进制数据)

5. Miscellanous(地址栏，杂项)



# Wireshark 显示过滤

![](http://pic002.cnblogs.com/images/2012/263119/2012100617113979.png)

使用过滤是非常重要的， 初学者使用wireshark时，将会得到大量的冗余信息，在几千甚至几万条记录中，以至于很难找到自己需要的部分。搞得晕头转向。

过滤器会帮助我们在大量的数据中迅速找到我们需要的信息。

过滤器有两种，

一种是显示过滤器，就是主界面上那个，用来在捕获的记录中找到所需要的记录

一种是捕获过滤器，用来过滤捕获的封包，以免捕获太多的记录。 在Capture -> Capture Filters 中设置



# 保存过滤

在Filter栏上，填好Filter的表达式后，点击Save按钮， 取个名字。比如"Filter 102",

![](http://pic002.cnblogs.com/images/2012/263119/2012100617144741.png)

Filter栏上就多了个"Filter 102" 的按钮。

![](http://pic002.cnblogs.com/images/2012/263119/2012100617155580.png)

# 过滤表达式的规则

表达式规则

 1. 协议过滤

比如TCP，只显示TCP协议。

2. IP 过滤

比如 ip.src ==192.168.1.102 显示源地址为192.168.1.102，

ip.dst==192.168.1.102, 目标地址为192.168.1.102

3. 端口过滤

tcp.port ==80,  端口为80的

tcp.srcport == 80,  只显示TCP协议的愿端口为80的。

4. Http模式过滤

http.request.method=="GET",   只显示HTTP GET方法的。

5. 逻辑运算符为 AND/ OR

常用的过滤表达式
|过滤表达式|用途|
|----|----|
|http|只查看HTTP协议的记录|
|ip.src ==192.168.1.102 or ip.dst==192.168.1.102|源地址或者目标地址是192.168.1.102|
|||
|||















# 封包列表(Packet List Pane)

封包列表的面板中显示，编号，时间戳，源地址，目标地址，协议，长度，以及封包信息。 你可以看到不同的协议用了不同的颜色显示。

你也可以修改这些显示颜色的规则，  View ->Coloring Rules.

![](http://pic002.cnblogs.com/images/2012/263119/2012100622181376.png)



# 封包详细信息 (Packet Details Pane)

这个面板是我们最重要的，用来查看协议中的每一个字段。

各行信息分别为

Frame:   物理层的数据帧概况

Ethernet II: 数据链路层以太网帧头部信息

Internet Protocol Version 4: 互联网层IP包头部信息


Transmission Control Protocol:  传输层T的数据段头部信息，此处是TCP


Hypertext Transfer Protocol:  应用层的信息，此处是HTTP协议




# wireshark与对应的OSI七层模型

![](http://pic002.cnblogs.com/images/2012/263119/2012100709030740.png)

# TCP包的具体内容

 从下图可以看到wireshark捕获到的TCP包中的每个字段。



![](http://pic002.cnblogs.com/images/2012/263119/2012100717254656.png)



# 实例分析TCP三次握手过程

看到这， 基本上对wireshak有了初步了解， 现在我们看一个TCP三次握手的实例



 三次握手过程为

![](http://pic002.cnblogs.com/images/2012/263119/2012100722541987.png)



这图我都看过很多遍了， 这次我们用wireshark实际分析下三次握手的过程。

打开wireshark, 打开浏览器输入 http://www.cnblogs.com/tankxiao

在wireshark中输入http过滤， 然后选中GET /tankxiao HTTP/1.1的那条记录，右键然后点击"Follow TCP Stream",

这样做的目的是为了得到与浏览器打开网站相关的数据包，将得到如下图

![](http://pic002.cnblogs.com/images/2012/263119/2012100723095165.png)

图中可以看到wireshark截获到了三次握手的三个数据包。第四个包才是HTTP的， 这说明HTTP的确是使用TCP建立连接的。



第一次握手数据包

客户端发送一个TCP，标志位为SYN，序列号为0， 代表客户端请求建立连接。 如下图

![](http://pic002.cnblogs.com/images/2012/263119/2012100723171949.png)

第二次握手的数据包

服务器发回确认包, 标志位为 SYN,ACK. 将确认序号(Acknowledgement Number)设置为客户的I S N加1以.即0+1=1, 如下图

![](http://pic002.cnblogs.com/images/2012/263119/2012100723221390.png)

第三次握手的数据包

客户端再次发送确认包(ACK) SYN标志位为0,ACK标志位为1.并且把服务器发来ACK的序号字段+1,放在确定字段中发送给对方.并且在数据段放写ISN的+1, 如下图:

![](http://pic002.cnblogs.com/images/2012/263119/2012100723244189.png)

 就这样通过了TCP三次握手，建立了连接




Fiddler是最强大最好用的Web调试工具之一，它能记录所有客户端和服务器的http和https请求，允许你监视，设置断点，甚至修改输入输出数据. 使用Fiddler无论对开发还是测试来说，都有很大的帮助。

阅读目录
- [Fiddler的基本介绍](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#introduce)
- [Fiddler的工作原理](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#yuanli)
- [同类的其它工具](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#qita)
- [Fiddler如何捕获Firefox的会话](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#firefox)
- [Firefox 中使用Fiddler插件](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#fiddlerhook)
- [Fiddler如何捕获HTTPS会话](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#https)
- [Fiddler的基本界面](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#basic)
- [Fiddler的统计视图](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#summaryview)
- [QuickExec命令行的使用](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#quickexec)
- [Fiddler中设置断点修改Request](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#request)
- [Fiddler中设置断点修改Response](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#response)
- [Fiddler中创建AutoResponder规则](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#autoresponder)
- [Fiddler中如何过滤会话](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#filter)
- [Fiddler中会话比较功能](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#compare)
- [Fiddler中提供的编码小工具](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#encodingtool)
- [Fiddler中查询会话](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#findsession)
- [Fiddler中保存会话](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#savesession)
- [Fiddler的script系统](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#script)
- [如何在VS调试网站的时候使用Fiddler](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#vs)
- [Response 是乱码的](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html#decodeResponse)

# Fiddler的基本介绍

Fiddler的官方网站:  [www.fiddler2.com](http://www.fiddler2.com/)

Fiddler官方网站提供了大量的帮助文档和视频教程， 这是学习Fiddler的最好资料。

Fiddler是最强大最好用的Web调试工具之一，它能记录所有客户端和服务器的http和https请求，允许你监视，设置断点，甚至修改输入输出数据，Fiddler包含了一个强大的基于事件脚本的子系统，并且能使用.net语言进行扩展

你对HTTP 协议越了解， 你就能越掌握Fiddler的使用方法. 你越使用Fiddler,就越能帮助你了解HTTP协议.

Fiddler无论对开发人员或者测试人员来说，都是非常有用的工具

# Fiddler的工作原理

Fiddler 是以代理web服务器的形式工作的,它使用代理地址:127.0.0.1, 端口:8888. 当Fiddler会自动设置代理， 退出的时候它会自动注销代理，这样就不会影响别的程序。不过如果Fiddler非正常退出，这时候因为Fiddler没有自动注销，会造成网页无法访问。解决的办法是重新启动下Fiddler.

![](http://pic002.cnblogs.com/images/2012/263119/2012020409075327.png)

![](http://pic002.cnblogs.com/images/2012/263119/2012020409081574.png)



# 同类的其它工具

同类的工具有: httpwatch, firebug, wireshark



# Fiddler 如何捕获Firefox的会话

能支持HTTP代理的任意程序的数据包都能被Fiddler嗅探到，Fiddler的运行机制其实就是本机上监听8888端口的HTTP代理。 Fiddler2启动的时候默认IE的代理设为了127.0.0.1:8888，而其他浏览器是需要手动设置的，所以将Firefox的代理改为127.0.0.1:8888就可以监听数据了。

Firefox 上通过如下步骤设置代理

点击: Tools -> Options,  在Options 对话框上点击Advanced tab - > network tab -> setting.



# Firefox 中安装Fiddler插件

修改Firefox 中的代理比较麻烦， 不用fiddler的时候还要去掉代理。 麻烦

推荐你在firefox中使用fiddler hook 插件, 这样你非常方便的使用Fiddler获取firefox中的request 和response

当你安装fiddler后， 就已经装好了Fiddler hook插件， 你需要到firefox中去启用这个插件

打开firefox   tools->Add ons -> Extensions 启动 FiddlerHook

![](http://pic002.cnblogs.com/images/2012/263119/2012050910410873.jpg)



![](http://pic002.cnblogs.com/images/2012/263119/2012050722424470.png)



# Fiddler如何捕获HTTPS会话

默认下，Fiddler不会捕获HTTPS会话，需要你设置下, 打开Fiddler  Tool->Fiddler Options->HTTPS tab

![](http://pic002.cnblogs.com/images/2012/263119/2012020409194719.png)

 选中checkbox, 弹出如下的对话框，点击"YES"

![](http://pic002.cnblogs.com/images/2012/263119/2012020409204531.png)

![](http://pic002.cnblogs.com/images/2012/263119/2012020409211815.png)

点击"Yes" 后，就设置好了

# Fiddler的基本界面

 看看Fiddler的基本界面

![](http://pic002.cnblogs.com/images/2012/263119/2012020521421977.png)



Inspectors tab下有很多查看Request或者Response的消息。 其中Raw Tab可以查看完整的消息，Headers tab 只查看消息中的header. 如下图

![](http://pic002.cnblogs.com/images/2012/263119/2012021417205917.png)



# Fiddler的HTTP统计视图

通过陈列出所有的HTTP通信量，Fiddler可以很容易的向您展示哪些文件生成了您当前请求的页面。使用Statistics页签，用户可以通过选择多个会话来得来这几个会话的总的信息统计，比如多个请求和传输的字节数。

选择第一个请求和最后一个请求，可获得整个页面加载所消耗的总体时间。从条形图表中还可以分别出哪些请求耗时最多，从而对页面的访问进行访问速度优化

![](http://pic002.cnblogs.com/images/2012/263119/2012020613564321.png)

# QuickExec命令行的使用

Fiddler的左下角有一个命令行工具叫做QuickExec,允许你直接输入命令。

常见得命令有

help  打开官方的使用页面介绍，所有的命令都会列出来

cls    清屏  (Ctrl+x 也可以清屏)

select  选择会话的命令

?.png  用来选择png后缀的图片

bpu  截获request

![](http://pic002.cnblogs.com/images/2012/263119/2012020409303433.png)



# Fiddler中设置断点修改Request

[[作者：小坦克]](http://www.cnblogs.com/TankXiao/admin/%5B%E4%BD%9C%E8%80%85%EF%BC%9A%E5%B0%8F%E5%9D%A6%E5%85%8B%5D%20http:/www.cnblogs.com/TankXiao/)  Fiddler最强大的功能莫过于设置断点了，设置好断点后，你可以修改httpRequest
 的任何信息包括host, cookie或者表单中的数据。设置断点有两种方法

第一种：打开Fiddler 点击Rules-> Automatic Breakpoint  ->Before Requests(这种方法会中断所有的会话)

如何消除命令呢？  点击Rules-> Automatic Breakpoint  ->Disabled

第二种:  在命令行中输入命令:  bpu www.baidu.com   (这种方法只会中断www.baidu.com)

如何消除命令呢？  在命令行中输入命令 bpu



![](http://pic002.cnblogs.com/images/2012/263119/2012020520463978.jpg)

看个实例，模拟博客园的登录， 在IE中打开博客园的登录页面，输入错误的用户名和密码，用Fiddler中断会话，修改成正确的用户名密码。这样就能成功登录

1. 用IE 打开博客园的登录界面  http://passport.cnblogs.com/login.aspx

2. 打开Fiddler,  在命令行中输入bpu http://passport.cnblogs.com/login.aspx

3. 输入错误的用户名和密码 点击登录

4. Fiddler 能中断这次会话，选择被中断的会话，点击Inspectors tab下的WebForms tab 修改用户名密码，然后点击Run to Completion 如下图所示。

5. 结果是正确地登录了博客园

![](http://pic002.cnblogs.com/images/2012/263119/2012020521214252.png)





# Fiddler中设置断点修改Response

当然Fiddler中也能修改Response

第一种：打开Fiddler 点击Rules-> Automatic Breakpoint  ->After Response  (这种方法会中断所有的会话)

如何消除命令呢？  点击Rules-> Automatic Breakpoint  ->Disabled

第二种:  在命令行中输入命令:  bpafter www.baidu.com   (这种方法只会中断www.baidu.com)

如何消除命令呢？  在命令行中输入命令 bpafter,

![](http://pic002.cnblogs.com/images/2012/263119/2012020612051996.jpg)

具体用法和上节差不多，就不多说了。

# Fiddler中创建AutoResponder规则

Fiddler 的AutoResponder tab允许你从本地返回文件，而不用将http request 发送到服务器上。

看个实例. 1. 打开博客园首页，把博客园的logo图片保存到本地，并且对图片做些修改。

2. 打开Fiddler 找到logo图片的会话， http://static.cnblogs.com/images/logo_2012_lantern_festival.gif，  把这个会话拖到AutoResponer Tab下

3. 选择Enable automatic reaponses 和Unmatched requests passthrough

4. 在下面的Rule Editor 下面选择 Find a file... 选择本地保存的图片.  最后点击Save 保存下。

5.  再用IE博客园首页, 你会看到首页的图片用的是本地的。

![](http://pic002.cnblogs.com/images/2012/263119/2012020612312960.png)



![](http://pic002.cnblogs.com/images/2012/263119/2012020612332060.png)





# Fiddler中如何过滤会话

每次使用Fiddler, 打开一个网站，都能在Fiddler中看到几十个会话，看得眼花缭乱。最好的办法是过滤掉一些会话，比如过滤掉图片的会话. Fiddler中有过滤的功能, 在右边的Filters tab中，里面有很多选项, 稍微研究下，就知道怎么用。



# Fiddler中会话比较功能

选中2个会话，右键然后点击Compare，就可以用WinDiff来比较两个会话的不同了 (当然需要你安装WinDiff)

![](http://pic002.cnblogs.com/images/2012/263119/2012020612094928.png)



# Fiddler中提供的编码小工具

点击Fiddler 工具栏上的TextWizard,  这个工具可以Encode和Decode string.

![](http://pic002.cnblogs.com/images/2012/263119/2012020612125974.png)

# Fiddler中查询会话

用快捷键Ctrl+F 打开 Find Sessions的对话框，输入关键字查询你要的会话。 查询到的会话会用黄色显示

![](http://pic002.cnblogs.com/images/2012/263119/2012020612460937.png)

# Fiddler中保存会话

有些时候我们需要把会话保存下来，以便发给别人或者以后去分析。  保存会话的步骤如下：

选择你想保存的会话，然后点击File->Save->Selected Sessions

# Fiddler的script系统

Fiddler最复杂的莫过于script系统了 官方的帮助文档: [http://www.fiddler2.com/Fiddler/dev/ScriptSamples.asp](http://www.fiddler2.com/Fiddler/dev/ScriptSamples.asp)

首先先安装SyntaxView插件，Inspectors tab->Get SyntaxView tab->Download and Install SyntaxView Now... 如下图

![](http://pic002.cnblogs.com/images/2012/263119/2012020613160323.png)



安装成功后Fiddler 就会多了一个Fiddler Script tab, 如下图

![](http://pic002.cnblogs.com/images/2012/263119/2012020613173260.png)

在里面我们就可以编写脚本了， 看个实例 让所有cnblogs的会话都显示红色。

把这段脚本放在OnBeforeRequest(oSession: Session) 方法下，并且点击"Save script"

```
if (oSession.HostnameIs("www.cnblogs.com")) {
            oSession["ui-color"] = "red";
        }
```

这样所有的cnblogs的会话都会显示红色



# 如何在VS调试网站的时候使用Fiddler

我们在用visual stuido 开发ASP.NET网站的时候也需要用Fiddler来分析HTTP， 默认的时候Fiddler是不能嗅探到localhost的网站。不过只要在localhost后面加个点号，Fiddler就能嗅探到。

例如：原本ASP.NET的地址是 http://localhost:2391/Default.aspx，  加个点号后，变成 http://localhost.:2391/Default.aspx 就可以了

![](http://pic002.cnblogs.com/images/2012/263119/2012020620575359.png)



第二个办法就是在hosts文件中加入  127.0.0.1  localsite

如何你访问http://localsite:端口号   。  这样Fiddler也能截取到了。



# Response 是乱码的

有时候我们看到Response中的HTML是乱码的， 这是因为HTML被压缩了， 我们可以通过两种方法去解压缩。

1. 点击Response Raw上方的"Response is encoded any may need to be decoded before inspection. click here to transform"

2. 选中工具栏中的"Decode"。  这样会自动解压缩。

![](http://pic002.cnblogs.com/images/2012/263119/2012031512494991.png)





附： Fiddler 系列教程, (连载中, 敬请期待）

[Fiddler (一) 教程](http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html)

[Fiddler (二) Script用法](http://www.cnblogs.com/TankXiao/archive/2012/04/25/2349049.html)

[Fiddler (三) Composer创建和发送HTTP Request](http://www.cnblogs.com/TankXiao/archive/2012/12/25/2829709.html)

[Fiddler (四) 实现手机的抓包](http://www.cnblogs.com/TankXiao/p/3063871.html)

[Fiddler (五) Mac下使用Fiddler](http://www.cnblogs.com/TankXiao/archive/2013/04/18/3027971.html)

[Fiddler (六) 最常用的快捷键](http://www.cnblogs.com/TankXiao/archive/2013/06/12/3132405.html)















