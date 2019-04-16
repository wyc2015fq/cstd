# Rest风格WEB服务(Rest Style Web Service)的真相 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年12月21日 21:55:37[gloomyfish](https://me.csdn.net/jia20003)阅读数：17897
个人分类：[J2EE](https://blog.csdn.net/jia20003/article/category/871633)









写这篇文章是目的不是介绍Web-Service, 而是从Restful Web Service说起来剖析一下

什么才是真正的Restful Style的架构与协议，从而更好的理解web服务的设计理念与架

构本质。

**一：Web Service基础知识**

一个最简单web服务就一个web页面等待请求与处理。更容易理解的方式是Web 

Service可以把一个应用变成一个基本WEB方式的请求与处理的应用。常见的两种

Web Service处理方式为：

a.      基于WSDL/SOAP的方式

b.      Rest方式

方式a是比较正统的，客户端调用必须先取得WSDL文件，然后生成调用的API才可

以使用。它不是我要说的重点，基本调用流程如下：

![](https://img-my.csdn.net/uploads/201212/21/1356098187_5987.png)




方式b是Rest方式，Rest的Web Service的设计原则是基于CRUD，其支持四种操作分

别为：

GET – 获取信息/请求信息内容，绝大多数浏览器获取信息时使用该方式。

POST – 增加信息内容，显示以前的信息内容，可以看作是insert操作

PUT – 更新信息内容，相当与update

DELETE – 删除信息内容可以看作是delete



Rest方式更加简单便捷，如果从设计原则上看HTTP协议本身已经是最Restful风格的

协议了HTTP协议很好的支持了CRUD的操作。正是因为如此，WEB2.0以来, 基于

Restful的Web Service越来越多的成为首选。



**二：认识RestfulStyle**

Rest的全称是可表述状态迁移(RepresentationalState Transfer), 可能从字面看有点奇怪

HTTP协议本身无状态协议，其保持连接通过设置请求头字段Connection: keep-alive与

设置过期时间来同时控制。其实Rest方式的WebService也是无状态的这样做的好处最少

有以下两个：

1.      更好的负载平衡，减轻服务器端负担

2.      更快的客户端响应，减少不必要的状态检查。



Restful 风格的兴起，要感谢互联网巨头Google,Facebook等他们提供大量基于Restful

风格的web服务，从谷歌地图到天气预报到翻译，国内的互联网巨头腾讯，新浪微博也

发布自己的web服务，吸引更多的开发者加入他们的阵营。Rest除了满足基本的CRUD

设计原则之外，还要遵循如下约定：

1.      资源操作可以通过描述来实现即Representation

2.      消息本身是无状态与自我描述(传输支持XML与JSON)

3.      可以发送与接受多个Representation



Rest风格(Restful Style)架构原则：

1.      客户服务器方式

2.      无状态协议传输

3.      支持缓存

4.      统一接口定义

5.      分层系统设计

这样发布了Rest的Web服务API其改变不会影响到客户端程序与实现。如果你的系统

不能适用Rest风格的架构怎么办，重新设计一个新的架构，扩展Rest风格架构。但是

这个世界上绝大数的系统与应用要做的事情就是CRUD。



**三. Rest与HTTP**

上面已经提到过HTTP协议可能是最Rest风格的协议，而HTTP1.1协议设计的一个原则

就要实现Rest风格。所以毫无疑问HTTP的GET, POST, PUT, DELETE就是最好的证明

但是Rest风格是否可以应用到其它一些协议与系统设计中嘛，答案是肯定的，一个最好

的例子证明就POP3协议, POP3支持Fetch 数据记录，查询记录，更新记录与删除记录

(记录代表email)多么完美的Rest风格协议。




**已经存在的HTTP协议应用：**

1.      浏览器客户端(你天天上网，不是IE就是Chrome，或者其它浏览器，你懂的)

2.      即时消息通信，MSN/Skype支持

3.      各种内容管理系统

4.      博客系统与微博客户端应用。

5.      你可以来补充/？



Rest消息详解:

1.      跟我们现在知道的HTTP URI没有什么分别，Google静态地图就是一个很好的例子

         只是URL加上不同参数就可以fetch不同的地图内容。

2.      可以支持任何类型的数据传输，这点与基于XML与JSON的信息传输有点同，后者

         更希望传输文本内容与结构化文本内容

3.      SOAP与XML-RPC有严格的消息格式限制，rest没有消息格式要求。客户端调用方

         便啊！



Rest风格Web服务的好处，显然易见一个好处就是简化了客户端的调用，不再像WSDL

那般麻烦。从而减低第三方开发者的学习成本，减短了学习曲线。有利于服务推广与普

及，吸引更多用户数量从而带来潜在的商业利益。



在软件即服务(SaaS - Software As A Service)与软件即平台(PasS-Platform

As A Service)中有着重要的地位与应用。这正是那些互联网巨头对Rest风

格感兴趣的原因之一。



**四：Rest风格架构**

Rest风格可以用在非WEB的系统设计与架构中嘛/?打答案是肯定的，Rest可以用在任何

系统设计中，从本质是上Rest不是一种技术，而是一种架构原则，当然可以用来架构非

WEB的系统。系统越大风格越要象Rest方式如此才是一个成功的架构。



WEB中的面向对象编程

ExtJS, KendoUI(基于JQuery)等JavaScript库已经支持非常方便的从URL中fetch内容

更新数据，前端设计越来越趋向于更加细化的分层设计，而不仅仅是MVC。客户端

程序员应该更多的专注前台用户体验，由于这些框架良好的封装与可扩展行，

JavaScript等语言编程越来越多的引入面向对象的概念与实践。可以好不夸张的说如

今的JavaScript编程与十年之前已经有本质不同。




**个人观点，仅供拍砖**





