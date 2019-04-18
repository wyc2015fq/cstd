# 通过 Tomcat Advanced I/O 获得高性能的 Ajax - z69183787的专栏 - CSDN博客
2014年04月05日 12:01:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：717

**使用不同的请求处理模型减少并发线程**
使用非阻塞 I/O（Non-Blocking I/O，NIO）可以极大地提高服务器的性能，因为它能充分地利用系统资源（线程）。在具有长轮询机制的 Asynchronous JavaScript + XML (Ajax) 应用程序中，性能的提升非常显著。它还允许在受限制的条件下控制服务器系统资源的使用。本文解释了如何在处理 Ajax 和常规请求期间优化服务器的性能。
0![](http://dw1.s81c.com/developerworks/i/v17/dw-cmts-arrow.png)[评论：](http://www.ibm.com/developerworks/cn/web/wa-aj-hiperform/#icomments)
[Adriaan
 de Jonge](http://www.ibm.com/developerworks/cn/web/wa-aj-hiperform/#authorN1001C), 专业软件开发人员, 自由职业者
2008 年 11 月 03 日
- ![+](http://www.ibm.com/i/c.gif)内容
浏览器通过发送请求和显示来自服务器的响应与 Web 服务器进行通信。这似乎不是很重要，但从几个方面考虑，它都与本文相关。
富 Web 应用程序逐渐尝试提供一些与请求/响应模型不同的功能。Ajax 框架由底层的请求/响应模型抽象而来，它提供了一种运行在客户端的类似于图形用户界面（GUI）的模型。它像一个本地的重量级 Microsoft® Windows® 或 KDE 应用程序。这意味着您可以创建响应事件的代码，而不是响应请求的代码。细粒度的事件处理对富用户界面至关重要。
## Ajax 资源中心
请访问 [Ajax 资源中心](http://www.ibm.com/developerworks/cn/ajax/)，这是有关 Ajax 编程模型信息的一站式中心，包括很多文档、教程、论坛、blog、wiki
 和新闻。任何 Ajax 的新信息都能在这里找到。
但在底层，仍然有一个请求/响应模型。这不仅是一个有限制的编程模型 — 您可以使用 API 抽象处理它 — 在很多情况下，它还导致性能问题，并且损害服务器在繁忙时控制资源的能力。如果不按照它的设计初衷使用，该模型还会导致更严重的问题。
例如，浏览器客户端期望收到关于服务器更改的通知。这原先是不可能实现的；您只能每隔 5 分钟重新加载整个页面来模仿这个行为。因为引入了 JavaScript 代码和 Ajax，所以能够以更高效更美观的方式让页面实现更改通知。
页面可能会定期轮询服务器，从而创建大量的服务器请求，以及事件和通知之间的大幅度延迟。或者它可以在等待响应时，始终为服务器打开一个连接。第二个模型称为长轮询：本文将讨论这一轮询模型 — 因为它以最快的速度响应服务器上的事件 — 以及一些需要克服的困难。
在典型的 servlet 模型中，一个打开的连接意味着一个不执行任何操作的专用线程，它将一直等待，直到在响应事件时更新客户端。线程还是比较耗费资源的，一个服务器上可用的线程是有限制的。并发访问的增加会快速消耗服务器上的资源。单个的访问不只是意味着一些偶然的页面请求；它还意味着在访问期间有一个持续打开的连接（带有等待线程）。如果有几百个访问者在他们离开电脑时不关闭浏览器，就会带来一些问题。抽象并不能解决这样的性能问题。
## 解决方案
低级别的问题可以在低级别的 API 中找到解决方案。在坚持使用请求/响应模型的同时，您可以使用非阻塞 I/O（NIO）来保持连接的打开状态，而不需要浪费等待线程。为了方便在 servlet 中使用 NIO，您需要一个基于事件的 API，它会在打开的连接上适时发出恰当的读写行为。Tomcat 6 提供的 CometProcessor API 为这种基于事件的模型提供了便利。本文引入了 CometProcessor API。
[回页首](http://www.ibm.com/developerworks/cn/web/wa-aj-hiperform/#ibm-pcon)
## NIO
首先，您需要对 NIO 有一定的了解。在典型的阻塞 I/O（blocking I/O）中，您始终从一个线程中读取流，并且该线程会一直等待，直到整个流完成。对于需要在一次迭代中处理的、生命周期很短的流而言，这很有效：开始读取数据直到读完所有数据，然后关闭连接。这在一次的迭代中占有很少时间，所以只是短时间占用系统资源。大量并发的短生命周期连接不会造成问题。
为了获得通知，您需要一个机制，它只在需要读时才读，需要写时才写，但又保持连接打开，以迅速响应发生的事件。为了便利这个过程，要用到 NIO，它已是 1.4 版本以后的 Java™ 语言的一部分。
通过使用 NIO，就会在流完成之后才遍历它。在没有可用数据时才遍历流，这样就可以让流执行其他工作。当发生影响流的事件时 — 例如，进入更多的数据 — 您将继续遍历流。清单 1 展示了一个针对这种事件的示例处理程序；注意 `is.available()`。
##### 清单 1. 针对 `InputStream` 的单个读迭代
InputStream is = request.getInputStream();
byte[] buf = new byte[512];
do {
    int n = is.read(buf); //can throw an IOException
    if (n > 0) {
        //read n bytes
    } else if (n < 0) {
        //error
        return;
    }
} while (is.available() > 0);
您可以以类似的方式使用 `OutputStream` 和 `Writer`。确保在执行写迭代后刷新数据；否则，它将滞留在缓冲区中，直到向流写入下一次迭代。
[回页首](http://www.ibm.com/developerworks/cn/web/wa-aj-hiperform/#ibm-pcon)
## Comet
本文关注的是 Tomcat 6，它的 Advanced I/O 模块支持 NIO。Jetty 也有类似的功能，但 API 不同。当然，您不希望编写一个特定于某个容器实现的解决方案。即将发布的 Servlet 3.0 规范将带来好消息：当它由通用的容器实现时，会有一个标准的机制来支持基于事件的 NIO。
现在，通过特定于 Tomcat 的 CometProcessor 可以更方便地使用这一功能。在 Servlet 3.0 规范发布之前试一试 Tomcat 是很有价值的。如果想建立自己的框架，Tomcat 实现是很好用的；因此本文使用它。Jetty 实现与现有的框架和解决方案紧密集成，如果想在不深入了解技术细节的情况下使用 Comet 的其他功能，它是比较实用的选择。在 [参考资料](http://www.ibm.com/developerworks/cn/web/wa-aj-hiperform/#resources) 中有另一篇文章，它通过
 WebSphere® Community Edition 展示了类似的技术。
要在 Tomcat 6.0 上使用 CometProcessor，必须进行两处更改：一是配置，二是代码。
首先，在 server.xml 文件中找到连接器。它类似于：
<Connector connectionTimeout="20000" port="8080" protocol="HTTP/1.1" redirectPort="8443"/>
使用下面的内容替换 HTTP/1.1 协议：
<Connector connectionTimeout="20000" port="8080" 
	protocol="org.apache.coyote.http11.Http11NioProtocol" redirectPort="8443"/>
第二步是通过 servlet 实现 `org.apache.catalina.CometProcessor` 接口。这个接口要求实现一个称为 `event()` 的方法。在第一步中配置的`Http11NioProtocol` 调用 `event()` 方法来处理请求，而不是 `doGet` 或 `doPost`。
最基本的支持 Comet 的 servlet 实现如清单 2 所示。
##### 清单 2. 处理基本的 Comet 事件
package eu.adriaandejonge.comet;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import org.apache.catalina.CometEvent;
import org.apache.catalina.CometProcessor;
import org.apache.catalina.CometEvent.EventType;
public class CometServlet extends HttpServlet implements CometProcessor {
   
    	public void event(CometEvent event) throws IOException, ServletException {
       		if(event.getEventType() == EventType.BEGIN) {
          			// fill in code handling here
       		}
       		// and continue handing other events
    	}
}
当连接开始时（`BEGIN`）、每次新数据可用时（`READ`），以及连接结束时（`END`），抛出该事件。这是理想的流程。如果出现错误，将接收到一个 `ERROR` 事件，它可能有几个子事件类型。例如，为了记录日志或调查事务回滚的需求，将 `TIMEOUT` 事件从 `SERVER_SHUTDOWN` 事件分离出来是很有意义的。
Comet 允许针对不同的事件指定不同的连接超时。这意味着可以给常规的请求设置很短的生命周期，但是对于响应长轮询请求的机制，也可以将这个生命周期延长至几分钟。这样，您就不会犯下为了方便长轮询而延长所有超时的错误。当常规的请求没被恰当地终止并保持挂起时，这种错误会导致意外的麻烦。
[回页首](http://www.ibm.com/developerworks/cn/web/wa-aj-hiperform/#ibm-pcon)
## Ajax 处理
看一下代码示例，接收 `CometEvent` 好像比处理常规请求要困难得多。在某种程度上，的确如此。至少，工作量增加了。一个单一请求被分成多个阶段，因此，现在不是处理单一的 `doGet`，而是必须处理一个 `BEGIN`、至少一个 `READ`，以及一个 `END`。
造成这种复杂性的部分原因是出现了一种常见的错误：通过大量的 `if` 语句捕捉所有处理逻辑来实现 `event()` 方法。问题并不完全归结在 `if` 语句上，`if` 语句中代码的数量才是最关键的。除了代码的可维护性外，这个方法还有其他缺陷。
当数据可用时，`if` 语句使您可以从连接进行读取，然后马上处理请求。这并不适合于通知机制。您需要采用另一种方式：在事件发生前（您希望向客户端发送新信息来响应它），连接应该保持打开。这可以通过几种方式来实现，但它们有一个共同点：它们都引入了恰当的对象模型，该模型处理程序逻辑而不是捕捉 `if` 语句中的所有代码。
其中的一种方法是保持一个中央队列，并在事件发生时使用它来发送所有通知。然后，由客户端负责重新连接到该队列。然而，对于 `END` 或`ERROR` 事件，`CometProcessor` 需要从该队列注销连接。如清单
 3 所示。
##### 清单 3. 注册 Comet 事件
package eu.adriaandejonge.comet;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.catalina.CometEvent;
import org.apache.catalina.CometProcessor;
import org.apache.catalina.CometEvent.EventType;
public class CometServlet extends HttpServlet implements CometProcessor {
   
   	private static final long serialVersionUID = 3616604581787849064L;
 
	   private static final String EVENT_REGISTRY = "event.registry";
 
   	private EventRegistry eventRegistry;
   	public CometServlet() {
      eventRegistry = (EventRegistry) this.getServletContext()
      				.getAttribute(EVENT_REGISTRY);                          
   	}    
   	public void event(CometEvent event) throws IOException, ServletException {
      		HttpServletRequest request = event.getHttpServletRequest();
      		HttpServletResponse response = event.getHttpServletResponse();
      		if(event.getEventType() == EventType.BEGIN) {
         			eventRegistry.register(request, response);
      		} else if(event.getEventType() == EventType.END) {
         			eventRegistry.deregister(request, response);
      		} else if(event.getEventType() == EventType.ERROR) {
         			eventRegistry.deregister(request, response);
      		}
   	}   	  	    
}
另一种办法是保持一个中央 `ListenerRegistry`，客户端可以将它用作事件监听器。不同之处是在事件通知之后连接仍然保持打开。NIO
 允许随后再返回到这个连接并发送额外的数据。在客户端，您可以使用标准的 API（比如 Bayeux）来支持这一功能。但当前，它是由 Dojo 实现的，并且和 Jetty Comet 实现结合使用可以获得最好的效果。
Comet 主要是服务器端技术。在客户端，处理请求和常规请求差不多。为了进行演示，清单 4 测试了最基本的 Ajax 请求：来自 Wikipedia 的典型的基于 `XMLHttpRequest` 的示例。它能够很好地响应来自
 Comet 服务器的事件。
##### 清单 4. 发送一个 Ajax 请求
<script>
function ajax(url, vars, callbackFunction) {
  var request =  new XMLHttpRequest();
  request.open("POST", url, true);
  request.setRequestHeader("Content-Type",
                           "application/x-javascript;");
 
  request.onreadystatechange = function() {
    if (request.readyState == 4 && request.status == 200) {
      if (request.responseText) {
        callbackFunction(request.responseText);
      }
    }
  };
  request.send(vars);
}
function testFunction(myText) {
	alert("myText = " + myText);
}
</script>
<input type="button" caption="test"
	onclick="ajax('http://localhost:8080/comettest/CometServlet', 
	'', testFunction);">
这表明您可以使用其他框架（比如 Prototype）来处理 Comet 事件。然而，阅读 Bayeux 规范并了解更加规范化的事件交互协议是一个有趣的实践。
[回页首](http://www.ibm.com/developerworks/cn/web/wa-aj-hiperform/#ibm-pcon)
## 总体性能调优
将队列或事件监听器与 CometProcessor 结合使用能够节省大量打开的线程，但这个方法不能控制某些并发线程的数量（这些线程处理将要返回给浏览器的页面）。因此，服务器可能面临大量需要同时处理的请求。由 CometProcessor 接口引入的基于事件的模型也有助于改善总体性能。
解决方案是引入一个机制，它不做其他工作，而是尽快地处理事件，然后通过 `WorkerQueue` 注册这些事件并结束处理。同时，您必须运行一定数量的 `WorkerThread`，它们从 `WorkerQueue` 获取请求，然后逐个处理它们。这样，处理将要返回给浏览器的页面的并发线程比实现相同目的的 `WorkerThread` 要少。
当请求的数量增加，并且工作线程的数量太少时，处理请求则需要很长时间。但在能承受的负载范围内，服务器不会（至少很少）崩溃。请求处理时间和队列长度是一个很好的度量指标，系统管理员通过监控它们来决定是增加服务器，还是关掉多余的服务器。这种处理请求的方法在云计算环境中表现很出色。
将 Comet 事件传递给处理函数机制的代码如清单 5 所示。本示例省略了队列的实际处理过程，因为它特定于您的需求，并涉及到许多微妙的并发性问题。但它展示了如何通过更加面向对象的解决方案来实现很大的 `if` 语句。要想进一步增强面向对象特性，可以将 `if` 语句的最后一小部分替换为 `Map`，后者将 `EventType` 连接到 `EventHandler` 工厂对象。但本示例只展示总体思路。
##### 清单 5. 在一个 `EventWorker` 队列上注册 Comet 事件
package eu.adriaandejonge.comet;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.catalina.CometEvent;
import org.apache.catalina.CometProcessor;
import org.apache.catalina.CometEvent.EventType;
public class CometServlet extends HttpServlet implements CometProcessor {
   private static final long serialVersionUID = 365737675389366477L;
   	private static final String EVENT_WORKER = "event.worker";
   	private EventWorker eventWorker;
   	public CometServlet() {
      		eventWorker = (EventWorker) this.getServletContext()
         			.getAttribute(EVENT_WORKER); 
   	}
   public void event(CometEvent event) throws IOException, ServletException {
      		HttpServletRequest request = event.getHttpServletRequest();
		      HttpServletResponse response = event.getHttpServletResponse();
      		if (event.getEventType() == EventType.BEGIN) {
         			eventWorker.enqueue(new BeginEvent(request, response));
      		} else if (event.getEventType() == EventType.READ) {
         			eventWorker.enqueue(new ReadEvent(request, response));
      		} else if (event.getEventType() == EventType.END) {
         			eventWorker.enqueue(new EndEvent(request, response));
      		} else if (event.getEventType() == EventType.ERROR) {
         			eventWorker.enqueue(new ErrorEvent(request, response));
      		}
   	}
}
[回页首](http://www.ibm.com/developerworks/cn/web/wa-aj-hiperform/#ibm-pcon)
## 处理常规请求和 Ajax 请求
一个更通用的场景要求您同时处理常规请求和 Ajax 请求。 一些 Ajax 请求必须立即处理，其他 Ajax 请求应该作为长轮询处理，普通的请求应该发布到 `WorkerQueue`。
可以创建多个 servlet 来处理这些不同的请求，并设定各个 servlet 的任务。但问题是，对本文前面定义的整个 HTTP 连接器而言，连接超时是一样的 —— 至少为超时配置的值是一样的。Comet 事件允许为每个请求指定另一个连接超时。这需要进行动态计算，但当为不同的 servlet 指定不同的值时，它能提供帮助。
[回页首](http://www.ibm.com/developerworks/cn/web/wa-aj-hiperform/#ibm-pcon)
## 结束语
在处理 Ajax 请求和常规请求期间优化服务器的性能，就像实现 `CometProcessor` 接口和向
 server.xml 文件添加不同的协议处理函数一样简单。但是，要管理请求处理，则需要软件开发技巧。您需要考虑队列、事件监听器、工作线程，以及注册和注销请求/响应。
如果您亲自开发自己的框架，就必须克服这个困难，以获得更加稳定的、性能更佳的服务器。但如果您开发的是一个简单的 Web 应用程序，复杂的事件处理则不仅仅是耗费额外的资源。
对于后一种情况，最好的办法是对事件通知请求限制 `CometProcessor` 的使用，而其他所有请求则可以依靠典型的 `doGet` 和 `doPost`。
注意关注 Servlet 3.0 规范的发布。它可能鼓励进一步在 Web 服务器上采用 NIO。
