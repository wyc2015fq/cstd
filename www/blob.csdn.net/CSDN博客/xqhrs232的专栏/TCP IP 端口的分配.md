# TCP/IP 端口的分配 - xqhrs232的专栏 - CSDN博客
2016年04月01日 23:20:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1261
原文地址::[http://blog.chinaunix.net/uid-20415521-id-1949978.html](http://blog.chinaunix.net/uid-20415521-id-1949978.html)
相关文章
1、[TCP/IP协议——TCP/IP协议号和端口](http://blog.chinaunix.net/uid-9236609-id-2005880.html) ----[http://blog.chinaunix.net/uid-9236609-id-2005880.html](http://blog.chinaunix.net/uid-9236609-id-2005880.html)
2、TCP所有端口号----[http://wenku.baidu.com/link?url=8C9r6H8noEku_M_Mnts6l-T7IQBynrDNEz9krjT4bAYPV_GidkevRmy59lRQK9Q4NUuwHW7DTBuPKHG4WdKyVmZKEzk_bKa9DCMe7u0au_G###](http://wenku.baidu.com/link?url=8C9r6H8noEku_M_Mnts6l-T7IQBynrDNEz9krjT4bAYPV_GidkevRmy59lRQK9Q4NUuwHW7DTBuPKHG4WdKyVmZKEzk_bKa9DCMe7u0au_G#%23%23)
3、TCP/IP 端口号大全----[https://www.douban.com/group/topic/7647051/](https://www.douban.com/group/topic/7647051/)
TCP和UDP采用16 bit的端口号来识别应用程序。TCP/IP端口号可以分为三个范围：“已知端口”、“注册端口”以及“动态和/或专用端口”。
- “已知端口”是从 0 到 1023 的端口。
- “注册端口”是从 1024 到 49151 的端口。
- “动态和/或专用端口”是从 49152 到 65535 的端口。
[](http://support.microsoft.com/kb/174904/zh-cn#top)
### 已知端口号
     “已知端口”由 IANA（Internet Assigned Numbers Authority）分配，并且在大多数系统中只能由系统（或根）进程或有特权的用户所执行的程序使用。TCP [RFC793] 中使用的端口用于命名进行长期对话的逻辑连接末端。为了向未知的呼叫方提供服务，系统定义了一个服务联系端口。
    联系端口有时也称为“已知端口”、“标准端口”、“保留端口”等。为了尽可能利用这些端口，UDP [RFC768] 使用了同样的端口分配。分配的端口只使用了一小部分可用的端口号。很多年以来，分配的端口一直处在 0-255 的范围内，256～1023之间的端口号通常都是由Unix系统占用，以提供一些特定的Unix服务—也就是说，提供一些只有Unix系统才有的、而其他操作系统可能不提供的服务。现在IANA管理1～1023之间所有的端口号。
例如：
|Internet标准服务|(保留的)标准端口号|
|----|----|
|ftp|21|
|telnet|23|
|whois|43|
|gopher|70|
|finger|79|
|www|80|
|nntp|119|
|irc|194|
|talk|517|
|uucp|540|
[](http://support.microsoft.com/kb/174904/zh-cn#top)
### 注册端口号
     “注册端口”由 IANA 列出，并且在大多数系统上可以由普通用户进程或普通用户所执行的程序使用。TCP [RFC793] 中使用的端口用于命名进行长期对话的逻辑连接末端。为了向未知的呼叫方提供服务，系统定义了一个服务联系端口。
    IANA 会注册这些端口的使用情况，从而向社区提供方便。为了尽可能利用这些端口，UDP [RFC768] 使用了同样的端口分配。“注册端口”的范围为 1024-49151。
    目前，端口分配已被列为活动文档，通常在出现新信息和新的分配时对此文档进行更新和修订。现在，可以从[这里](http://www.iana.org/assignments/port-numbers)查看端口号的分配信息。
http://www.iana.org/assignments/port-numbers
