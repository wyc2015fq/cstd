# C++web开发框架 - youfangyuan - CSDN博客
2012年02月15日 14:52:34[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：9740
[**C++ Web工具包 ****Wt**](http://www.oschina.net/p/wt)
Wt(音同'witty')是一个C++库，同时也是开发和部署web应用的服务器。 Wt不是所谓框架(framework)，它只是一个库，它不会将编程方式强加于开发者。
Wt的API是以widget为中心(widget-centric)的，并受到现有C++图形用户界面(GUI) 的应用编程接口(APIs)的启发。Wt为开发者提供了几乎所有web实现细节的抽象，其中 包括事件处理和图像支持。
基于页面的web应用开发框架(Page-based frameworks)，如PHP、JSP/JSF (+servlets+struts)等，没有实现对一些最基本技术的特性的抽象，这些技术包括 HTML/XHTML、JavaScript、CSS、AJAX、 Forms、DHTML、SVG/VML/Canvas等，显然基 于页面的开发框架(frameworks)必须要求开发者随时熟悉和掌握这些基本技术。另外， 当选择某一种基本技术时(如当前流行的AJAX)，开发者则要对该技术的优雅降级 (graceful degradation)负责，比如当AJAX或JavaScript已经不复存在或某些功能失效
 时。同时，应用程序结构还要遵循早期HTML以页面为基础的开发范式。作为一个AJAX开 发者，这就意味着他需要手工设计和维护客户端/服务器端的通讯。
尽管开发者有时不希望，但为了生成HTML代码或填充HTML模板会在页面中插入 JavaScript，如此操作则会带来安全风险，如跨站脚本(XSS:Cross-Site-Scripting)。 但是，模板框架(template frameworks)本身无法避免这种情况的发生，因为开发者总是 需要插入自定义的JavaScript来提高web应用。
相比之下，使用Wt开发的web应用就要好许多，因为这样的应用只使用一种编译 语言(C++)，而且生成必要的HTML/XHTML、Javascript、CGI、SVG/VML/Canvas以及 AJAX代码的任务都交给Wt库完成。自然地，web开发的安全性和浏览器可移植性问题也都 交给了Wt。另外，在可能的情况下，Wt将最大限度地使用JavaScript和AJAX；特别 地，在不支持AJAX的情况下，采用Wt开发的web应用仍能正常工作，因为Wt采用了 转回普通HTML/CGI的机制(reverting to
 a plain HTML/CGI mechanism)，保证了 浏览器与服务器之间的正常通讯。
#### 典型应用举隅：
- 
由于Wt web应用服务器占用较低的内存(low footprint)，故而可以为 可联网的嵌入式系统(web-enabled embedded systems)提供基于web的图形 用户界面(GUI)；
- 
Wt可以为需要整合现有C++库的应用提供基于web的图形用户界面(GUI), 这些应用包括科学上的或工程上的应用；
- 
Wt可以创建从现有C++桌面应用到web的有效渠道(port)。
#### 使用Wt的好处
- 
可以象开发C++桌面应用一样开发web应用；
- 
Wt提供最基本的(plain)widgets，这些widgets可以在运行环境不支持 JavaScript的情况下照常工作(当然，如果支持的话效果会更好)。同时Wt::Ext命名 空间下的widgets工作效果会更酷(more polished)，而且可以提供更高级的功能， 但是会需要对JavaScript的支持以及更为流行的浏览器。以上两类widgets在Wt中共享 同一套应用编程接口(API)；
- 
Wt提供两种开发与部署模式，即使用Wt内置(build-in)httpd或者FastCGI连接器 (connector)，前者在Wt内较容易实现，而后者则实际上是在开发FastCGI应用；
- 
客户端与服务器端在验证(validation)和事件处理(event handling)方面采用同 一原则(a single specification)，即两种应用下，开发者在服务器端使用C++编写， Wt既在服务器端生成C++代码，又同时在客户端生成JavaScript代码；其中事件处理特 指使用无状态槽(stateless slot)实现的情况；
- 
可以产生标准的、符合HTML/XHTML规范的代码；
- 
使用内联VML、SVG、HTML 5 Canvas可以提供可移植的(portable)、抗锯齿图像 (anti-aliased graphics)；
- 
无跨站脚本(XSS)风险，因为Wt完全控制表示层(presentation layer)，并提前 (proactively)过滤掉活动(active) 标签(tags)和属性(attibutes)，这样很好地
 隐藏了驻留在服务器端的业务逻辑(business logic)；
- 
基于Wt的应用载入时间极短(ultra-fast load time)，同时占用较低带宽 (low bandwidth usage)，当然也会受到显示内容复杂程度(screen complexity)的影响， 但肯定不会是应用程序的大小。
![](http://www.oschina.net/uploads/img/200901/27102103_QGSN.jpg)
[**C++的Web开发框架 ****CppCMS**](http://www.oschina.net/p/cppcms)
CppCMS是一个C++的Web开发框架（不是一个CMS） 。它不同于大多数其他Web开发框架，如巨蟒Django ， Java的Servlets ，或C++ Wt因为它在设计和调整来处理极高的负荷，而且它的目的是发展的网站，而不是“GUI-Like” Web应用程序。
下面是一个模板脚本示例：
<% c++ #include "data.h" %>  
<% namespace my_view %>  
<% class message uses data::message %>  
<% template render() %>  
<html>  
  <body>  
    <h1><% message %> World!</h1>  
  </body>  
<html>  
<% end template %>  
<% end class %>  
<% end namespace %>
类似的项目还有一个 [CppServ](http://www.oschina.net/p/cppserv)
[**C++的Web框架 ****runasdog**](http://www.oschina.net/p/runasdog)
它是网络开发中的一个异类，开发网络应用，但是完全不需要网络接口的参与，可以将你的本地程序瞬间变成一个网络服务器，这就是runasdog！
runasdog，dog意指watchdog（看门狗），就是说它会像看门狗一样，检视着网络的一举一动，并将之通知给本地程序。目前官方给出了两个 demo，一个是将bash变成remote bash server，启动后可以远程执行bash命令，另一个是聊天室程序，程序本身用lua编写，没有用到第三方库，通过runasdog将程序变成一个网络 聊天室，支持查看玩家列表，发送消息给指定玩家，广播消息，踢人，lua后台又调用bash等等。功能相当强大。
