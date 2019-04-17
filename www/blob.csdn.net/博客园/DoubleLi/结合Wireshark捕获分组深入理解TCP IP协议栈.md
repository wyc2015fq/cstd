# 结合Wireshark捕获分组深入理解TCP/IP协议栈 - DoubleLi - 博客园






**摘要：**

本文剖析了浏览器输入URL到整个页面显示的整个过程，以百度首页为例，结合Wireshark俘获分组进行详细分析整个过程，从而更好地了解TCP/IP协议栈。



**一、俘获分组**

**1.1 准备工作**

**(1) 清空浏览器缓存**

首先清空Web浏览器的高速缓存，确保Web网页是从网络中获取，而不是从高速缓冲取得[1]。谷歌浏览器，**Options --> Under the Hood --> Clear browsing data**。

**(2)清空DNS缓存**

在客户端清空DNS高速缓存，确保Web服务器域名到IP地址映射是从网络请求。在Windows XP，在命令行输入**ipconfig/flushdns**完成操作。

![](http://blog.chinaunix.net/attachment/201205/17/9112803_1337245999OFvr.png)

图1 清空DNS高速缓存

**(3)设置过滤规则**

为了便于分析，在截取数据包之前，设置过滤规则。在Filter ToolBar，输入过渡规则正则表达式，这里过滤ARP协议(地址解析协议)，如下：

![](http://blog.chinaunix.net/attachment/201205/17/9112803_13372461210q44.png)

图2 设置过滤规则

**(4)关闭网络应用程序**

为使俘获的报文仅跟访问URL相关，关闭其他网络应用程序(如QQ)。

**1.2 启动Wireshrk分组俘获器**

**Capture --> Interfaces**，弹出如下窗口，设置接口，点击Start启动分组俘获器。

![](http://blog.chinaunix.net/attachment/201205/17/9112803_1337246122Z99E.png)

图3 启动Wireshark分组俘获器

**1.3 浏览器输入URL**

这里，以百度为例，在浏览器输入：[http://www.baidu.com](http://www.baidu.com/)，回车。

**1.4 停止分组俘获**

![](http://blog.chinaunix.net/attachment/201205/17/9112803_1337246123Z1T6.png)

图4 Wireshark俘获分取实例

**二、概述**

Web的应用层协议是超文本传输协议HTTP，HTTP协议由两部分程序实现：客户机程序、服务器程序，协议定义了这些报文的格式以及客户机和服务器如何进行报文交换的。Web服务器用于存储Web对象，每个对象由URL寻址，Web客户机通常指浏览器。**浏览器向服务器发出对Web页中所包含对象的HTTP请求报文，服务器接受请求并用包含这些对象的HTTP响应报文进行响应。**Web页是由对象组成，对象简单来说就是文件(如图形文件、Java小程序、声音剪辑文件)，这些文件通过一个URL地址寻址。Web页通常含有一个基本的HTML文件和几个引用对象。

从在浏览器输入[http://www.baidu.com/](http://www.baidu.com/)并回车，到浏览器窗口显示百度首页，这中间经历多个过程，也涉及到诸多协议(如图4)，接下来，结合俘获的分组分析这期间都发生了些什么。

**2.1 域名解析(17~18)**

首先需要将URL(**存放对象的服务器主机名和对象的路径名)**解析成IP地址，具体步骤为：

> 
(1)同一台用户主机上运行着DNS应用的客户机端(如浏览器)

(2)从上述URL抽取主机名[www.baidu.com](http://www.baidu.com/)，传给DNS应用的客户机端(浏览器)

(3)该DNS客户机向DNS服务器发送一个包含主机名的请求(**DNS查询报文**)

(4)该DNS客户机收到一份回答报文(即**DNS回答报文**)，该报文包含该主机名对应的IP地址119.75.218.70


步骤(3)、(4)分别对应于上图的前两条DNS报文17、18，进一步获取这两步的细节，就得理解DNS协议，详情见：

博文《[结合Wireshark捕获分组深入理解TCP/IP协议栈之DNS协议](http://blog.chinaunix.net/uid-9112803-id-3212065.html)》

**2.2 建立TCP链接(19~24)**

HTTP使用TCP作为底层传输协议，需要先建立连接，即浏览器由IP地址定位的HTTP服务器发送一个TCP链接。TCP连接建立及TCP报文分析，见：

博文《[结合Wireshark捕获分组深入理解TCP/IP协议栈之TCP协议(TCP报文格式+三次握手实例)](http://blog.chinaunix.net/uid-9112803-id-3212041.html)》

**2.3 提取内容**

**2.3.1 概述**

客户机发送HTTP请求报文给服务器，服务器返回HTTP响应报文，关于HTTP报文格式及实例，详情见：

博文《[结合Wireshark捕获分组深入理解TCP/IP协议栈之HTTP协议](http://blog.chinaunix.net/uid-9112803-id-3212057.html)》

服务器返回客户机所请求的页面内容，交给浏览器，浏览器解释HTML文件(浏览器本质上是一个HTML解释器)，显示文本。服务器返回的实体主体部分内容如下(通过浏览器查看源代码或者查看响应报文的实体主体)：

![](http://blog.chinaunix.net/attachment/201205/17/9112803_1337246124u595.png)

图5 百度首页HTML语言

**2.3.2 俘获分组分析(26~32)**

从图4可以看出，HTTP请求报文到HTTP响应报文，中间还有若干TCP报文段，如下：

![](http://blog.chinaunix.net/attachment/201205/17/9112803_1337246124s5sK.png)

图6 请求至响应间相关报文段

看上去挺乱的，先给出我分析后的示意图，而后再分析，如下：

![](http://blog.chinaunix.net/attachment/201205/17/9112803_1337246125FcaE.png)

图7 请求至响应间相关报文段示意图

首先，客户机发送HTTP请求报文25，服务器响应该报文26(因为HTTP传输层协议是TCP，可靠传输)。接着，服务器返回HTTP响应报文，因HTTP报文太大(3835字节)，网络层对其进行分片，共4片，如下图(截自Wireshark俘获的HTTP响应报文)：

![](http://blog.chinaunix.net/attachment/201205/17/9112803_1337246126cXAo.png)

图8 数据分片实例

那为什么每两个TCP报文段才有一个确认，这是由于为了提高效率，TCP实行累积确认，即收到多个报文段之后，再一次确认。

**2.3 浏览器显示文本内容**

至此，浏览器收到百度首页基本HTML页面，浏览器解释该HTML页面(是不是也调用了JS解释器解释JavaScript脚本了？)，结果如下：

![](http://blog.chinaunix.net/attachment/201205/17/9112803_1337246126i4Z4.png)

图9 浏览器解释百度首页基本HTML页面

很明显，这与平时看到的首页不同，带X的框框、百度一下按钮，这是因为这些对象还未获得。

**2.4 浏览器取回并显示文件中的所有对象**

浏览器(客户机的代理)会继续向相应的服务器请求所需内容，从Wireshark俘获分组可知，浏览器请求了图片、JavaScript对象，如下：

![](http://blog.chinaunix.net/attachment/201205/17/9112803_1337246127O03u.png)

图10 浏览器请求文件中其他对象

有一点我没搞懂，为何请求/favicon.ico对象？在HTML文件找不到相关的代码，页面也没显示该图标。求指点，谢谢:-)

这下Web页面所有对象都到齐了，浏览器解释所有对象并显示，最终如果如下：

![](http://blog.chinaunix.net/attachment/201205/17/9112803_1337246128fjfN.png)

图11 百度首页

浏览器解释对象有3种方式：内置解释器(如HTML解释器)、插件、辅助应用程序。通过服务器上的各种脚本生成完整Web页面，服务器返回一个页面也返回一些有关页面的附加信息(包括MEME类型)，对于内置类型的对象直接由内置解释器解释，其他的，浏览器参照MIME类型表，调用相应的查看器处理该对象。





三、其他问题

**3.1 其他DNS报文**

Wireshark俘获的分组中包含着很多DNS报文，事实上，只有前面两组DNS报文是需要的，其他的则是预取。可以看出，浏览器将百度首页HTML所涉及的URL(充分利用空闲时间，减少等待时间)

![](http://blog.chinaunix.net/attachment/201205/17/9112803_1337246129fr85.png)

图12 DNS报文实例

这么多的分组，如何快速配对呢？通过标识数快速配对(如上图所示)，或者打开报文在Domain Name System底下的一行Request In或Response In会告知与哪个报文编号匹配。

**3.2 其他TCP报文段**

其他TCP报文段，要么是TCP连接建立，要么是传输数据，根据博文《[结合Wireshark捕获分组深入理解TCP/IP协议栈之TCP协议(TCP报文格式+三次握手实例)](http://blog.chinaunix.net/uid-9112803-id-3212041.html)》很容易分析的。

**3.3 SSDP协议**

从图4可以看出，访问页面之前和之后有大量SSDP报文。SSDP(简单服务发现协议，Simple Service Discovery Protocol)定义了如何在网络上发现网络服务的方法。不论是控制点，或是UPnP设备，工作中都必然用到SSDP，设备接入网络之后，要利用它向网络广播自己的存在(广播的信息中还有设备位置的描述)，以便尽快与对应的控制点建立联系；控制点则利用SSDP来搜索自己将要控制的设备在哪里.并且可以排除已经存在的设备和控制点――只为新近的或尚未"联络"上的双方服务[1]。

**3.4 NBNS**

Wireshark还俘获一个NBNS报文。网络基本输入/输出系统 (NetBIOS) 名称服务器 (NBNS) 协议是 TCP/IP 上的 NetBIOS (NetBT) 协议族的一部分，它在基于 NetBIOS 名称访问的网络上提供主机名和地址映射方法。





**参考资料：**

[1] 百度百科词条：[SSDP](http://baike.baidu.com/view/277232.htm)

[2] Andrew S.Tanenbaum.计算机网络[M].

[3] 《计算机网络--自顶向下方法与Internet特色》[M].

[4] 百度百科词条：[NBNS](http://baike.baidu.com/view/1403776.htm)

本实例的Wireshark俘获分组pcapng原文件![](http://blog.chinaunix.net/blog/image/attachicons/rar.gif) Wireshark俘获分组实例.rar 

请求至响应间相关报文报visio原文件![](http://blog.chinaunix.net/blog/image/attachicons/rar.gif) 请求至响应间相关报文段.rar 

from:http://blog.chinaunix.net/uid-9112803-id-3212207.html









