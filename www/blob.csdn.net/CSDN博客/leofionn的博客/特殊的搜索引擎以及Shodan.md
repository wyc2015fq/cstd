
# 特殊的搜索引擎以及Shodan - leofionn的博客 - CSDN博客


2018年08月24日 21:19:56[leofionn](https://me.csdn.net/qq_36142114)阅读数：445


[https://www.zhihu.com/question/20251786](https://www.zhihu.com/question/20251786)
以及：[https://www.shodan.io/](https://www.shodan.io/)
# 什么是Shodan？
Shodan是用于连接互联网的设备的搜索引擎。网络搜索引擎，例如Google和Bing，非常适合查找网站。但是，如果您有兴趣测量哪些国家变得更加紧密，那该怎么办？或者，如果您想知道哪个版本的Microsoft IIS最受欢迎？或者您想找到恶意软件的控制服务器？也许出现了一个新的漏洞，您想看看它可能会影响多少个主机？传统的网络搜索引擎不允许您回答这些问题。
Shodan收集有关直接连接到Internet的所有设备的信息。如果设备直接连接到Internet，则Shodan会查询它以获取各种公开信息。索引的设备类型可能有很大差异：从小型台式机到核电站，以及介于两者之间的所有设备。
那么Shodan指数呢？大部分数据来自**横幅广告**，**横幅广告**是有关在设备上运行的软件的元数据。这可以是有关服务器软件的信息，服务支持的选项，欢迎消息或客户端在与服务器交互之前想要了解的任何其他信息。例如，以下是FTP横幅：
`220``kcg.cz FTP server (Version``6.00``LS) ready.`这告诉我们服务器的潜在名称（kcg.cz），FTP服务器的类型（Solaris ftpd）及其版本（6.00LS）。对于HTTP，横幅看起来像：
`HTTP/1.0``200``OK``Date:``Tue,``16``Feb``2010``10``:03:04``GMT``Server:``Apache/1.3.26``(Unix)``AuthMySQL/2.20``PHP/4.1.2``mod_gzip/1.3.19.1a``mod_ssl/2.8.9``OpenSSL/0.9.6g``Last-Modified:``Wed,``01``Jul``1998``08``:51:04``GMT``ETag:``"135074-61-3599f878"``Accept-Ranges:``bytes``Content-Length:``97``Content-Type:``text/html`从这些服务中获得的信息适用于许多领域：
网络安全：密切关注贵公司面向互联网的所有设备
市场调查：了解人们在现实世界中使用的产品
网络风险：将供应商的在线风险列为风险指标
物联网：跟踪智能设备日益增长的使用情况
跟踪勒索软件：测量受勒索软件影响的设备数量
正如您所知，数据的用例是多种多样的。我们提供的平台可确保面向互联网的设备提供准确，一致和最新的信息 - 由您自行决定您最感兴趣的信息类型。
**它与谷歌有什么不同？**
最根本的区别在于，Shodan抓取互联网，而Google抓取万维网。然而，为万维网供电的设备只占实际连接到互联网的一小部分。Shodan的目标是提供完整的互联网图片。
与Google的另一个区别是，Shodan要求您了解搜索查询语法。例如，您不能简单地将**电厂**输入Shodan并期望获得适当的结果。我们为工程师/开发人员设计了Shodan，并充分利用了解搜索查询语法所需的数据。
[了解搜索查询基础知识](https://help.shodan.io/the-basics/search-query-fundamentals)

