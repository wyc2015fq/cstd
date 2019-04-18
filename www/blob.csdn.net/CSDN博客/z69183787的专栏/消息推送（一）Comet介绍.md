# 消息推送（一）Comet介绍 - z69183787的专栏 - CSDN博客
2014年03月16日 07:59:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：19256

转载请注明：[小林博客](http://linjunlong.com/) » [消息推送（一）Comet介绍](http://linjunlong.com/p/1013.html)
这篇文章主要讲述B/S架构中服务器“推送”消息给浏览器。内容涉及ajax论询（polling），comet（streaming，long polling）。后面会附上源代码。
[小林](http://linjunlong.com/)最近在工作有这么一个需求，需要在门户首页获取服务器“推送”过来的消息，像小林这种菜鸟，一般首先想到的是用ajax。本着好奇的精神，到网上查了一下，相关方面的知识，收获还真不小，于是就记录下分享给大家。
一般要实现网页的消息提醒，不外乎两种情况（[小林](http://linjunlong.com/)想当然的）：
- 客户端主动定时的去拿服务器端，有消息就提醒（polling）；
- 服务器主动"推送"消息给客户端，这里说的主动推送，并不是真的，而是客户端申请了需要显示消息提醒的信息，而服务端暂时没给客户端答复，把请求hold住了。。（comet）。
#### "服务器推"推技术简介
##### 基于HTTP长连接的"服务器推"技术
- 基于html file流（streaming，浏览器不兼容）
- iframe streaming（streming的扩展，浏览器兼容）
- 基于ajax长轮询（long-polling，浏览器兼容）
##### 基于客户端套接口的"服务器推"技术
- Flash XML Socket
- Java Applet 套接口这两种都不是我们这篇文章要说的主题，而且小林也没往这方面研究，因为，偶的应用是跑在weblogic的j2ee程序。
#### 示例环境
eclipse+tomcat
struts1.3+jsp+[jquery](http://www.w3school.com.cn/jquery/)
本代码中所有示例都是在eclipse+tomcat下运行通过的，浏览器使用ie9+chrome进行测试，运用了struts+jquery框架，来辅助实现。如果你熟悉strust的配置，可以跳过下面，直接看[polling](http://linjunlong.com/p/1013.html#polling)。
##### web.xml的配置
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app id="WebApp_ID" version="2.4" xmlns="http://java.sun.com/xml/ns/j2ee" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd">
    <display-name>test</display-name>
    <servlet>
        <servlet-name>action</servlet-name>
        <servlet-class>org.apache.struts.action.ActionServlet</servlet-class>
        <init-param>
            <param-name>config</param-name>
            <param-value>/WEB-INF/struts-config/struts-config-push.xml</param-value>
        </init-param>
    </servlet>
    <servlet-mapping>
        <servlet-name>action</servlet-name>
        <url-pattern>*.do</url-pattern>
    </servlet-mapping>
    <welcome-file-list>
        <welcome-file>index.html</welcome-file>
    </welcome-file-list>
</web-app>
```
新建/WEB-INF/struts-config/struts-config-push.xml加入如下内容struts的配置
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE struts-config PUBLIC "-//Apache Software Foundation//DTD Struts Configuration 1.1//EN"
                               "http://jakarta.apache.org/struts/dtds/struts-config_1_1.dtd">
<struts-config>
    <action-mappings>
        <action path="/push/comet" parameter="method" type="com.linjunlong.test.push.action.CometAction"  >
        </action> 
    </action-mappings>
</struts-config>
```
#### polling
在介绍comet之前，先介绍一些传统的[ajax](https://zh.wikipedia.org/wiki/Ajax)轮询（polling），轮询最简单也最容易实现，每隔一段时间向服务器发送查询，有更新再触发相关事件。对于前端，使用js的setInterval以AJAX或者JSONP的方式定期向服务器发送request。他可以让用户不需要刷新浏览器，也可以即时的获取服务器更新。
##### 前端jsp代码
我们新建一个在/WebContent/push下新建一个polling.jsp页面，把jquery脚本复制到/WebContent/static/js/jquery-1.8.0.min.js下。下面是polling.jsp代码，脚本部分我们设置每3秒进行一次轮询。
```java
<%@ page language="java" contentType="text/html; charset=GBK"
    pageEncoding="GBK"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<script type="text/javascript" src="../../static/js/jquery-1.8.0.min.js"></script>
<title >polling test</title>
</head>
<body>
<div id="message"></div>
</body>
<script type="text/javascript">
    var polling = function(){
        $.post('../comet.do?method=polling', function(data, textStatus){
            $("#message").append(data+"<br>");
        });
    };
    interval = setInterval(polling, 3000);
</script>
</html>
```
##### 后端action代码
我们在com.linjunlong.test.push.action.CometAction中添加polling方法
```java
public ActionForward polling(ActionMapping mapping,ActionForm form
        ,HttpServletRequest request,HttpServletResponse response) throws Exception{
    System.out.println("-----CometAction.polling.start");
    PrintWriter writer = response.getWriter();
    //TODO 编写一些CRUD的代码进行数据库查询，看看用户需不需要弹出消息提醒
    writer.print("your hava a new message");
    writer.flush();
    writer.close();
    System.out.println("-----CometAction.polling.end");
    return null;
}
```
##### 效果展示
当我们启动tomcat，在浏览器中输入[http://localhost:8080/test/push/comet/polling.jsp](http://localhost:8080/test/push/comet/polling.jsp)，浏览器上就不断的显示我们的ajax从后台获取的信息了。
![image](http://linjunlong.com/wp-content/uploads/2013/05/image_thumb10.png)
下面是chrome developer tool中url请求的信息，可以看到，ajax轮询就是不断的在后端进行访问。。如果服务器反映慢一点。。前面一个请求还没相应完，后面一个请求已经发送。会怎么样呢？
![image](http://linjunlong.com/wp-content/uploads/2013/05/image_thumb11.png)
采用这种方式要获取即使的消息推送，而且应用可能需要集群，小林想估计要弄一个队列表，然后模块有需要向某个人推送一条消息的话，就需要插入一条信息到数据库，然后客户端ajax访问后台，后台进行数据库查询，看当前用户在队列表里是否有记录，有的话，就取出来，返回给ajax，然后删除数据库中的记录。。。（这些都是小林想当然的啦，偶还没开始做。。。）
通过chrome的开发工具可以看到，浏览器不断的向后台进行请求（如果用户多的话，这得要多大的并发啊，估计压力测试，服务器直接挂了。）。而且每次请求服务器端不一定有数据返回，用在聊天系统还好说，小林只是想在门户首页弄个提醒而已啊，您有新的短消息，您有新的邮件- -。。。这种也许开一天浏览器都不一定有一条消息的- -。
#### comet
基于Comet的技术主要分为流（streaming）方式和长轮询（long-polling）方式。 首先看Comet这个单词，很多地方都会说到，它是“彗星”的意思，顾名思义，彗星有个长长的尾巴，以此来说明客户端发起的请求是长连的。即用户发起请求后就挂起，等待服务器返回数据，在此期间不会断开连接。流方式和长轮询方式的区别就是：对于流方式，客户端发起连接就不会断开连接，而是由服务器端进行控制。当服务器端有更新时，刷新数据，客户端进行更新；而对于长轮询，当服务器端有更新返回，客户端先断开连接，进行处理，然后重新发起连接。
 会有同学问，为什么需要流（streaming）和长轮询（long-polling）两种方式呢？是因为：对于流方式，有诸多限制。如果使用AJAX方式，需要判断XMLHttpRequest 的 readystate，即readystate==3时（数据仍在传输），客户端可以读取数据，而不用关闭连接。问题也在这里，IE 在 readystate 为 3 时，不能读取服务器返回的数据，所以目前 IE 不支持基于 Streaming AJAX，而长轮询由于是普通的AJAX请求，所以没有浏览器兼容问题。另外，由于使用streaming方式，控制权在服务器端，并且在长连接期间，并没有客户端到服务器端的数据，所以不能根据客户端的数据进行即时的适应（比如检查cookie等等），而对于long
 polling方式，在每次断开连接之后可以进行判断。所以综合来说，long polling是现在比较主流的做法（如facebook，Plurk）。 接下来，我们就来对流（streaming）和长轮询（long-polling）两种方式进行演示。
#### streaming
##### 前端jsp代码
/test/WebContent/push/comet/streaming.jsp，脚本中有个游标pos因为服务器端是一段一段的发送消息过来的。
```java
<%@ page language="java" contentType="text/html; charset=GBK"
    pageEncoding="GBK"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<script type="text/javascript" src="../../static/js/jquery-1.8.0.min.js"></script>
<title >streaming test</title>
</head>
<body>
<div id="message"></div>
</body>
<script type="text/javascript">
    try {
        var request = new XMLHttpRequest(); 
    } catch (e) {
        alert("Browser doesn't support window.XMLHttpRequest");
    }
                             
    var pos = 0;
    request.onreadystatechange = function () {
        if (request.readyState === 3) { 
            var data = request.responseText; 
            $("#message").append(data.substring(pos));
            pos = data.length;
        }
    };
    request.open("POST", "../comet.do?method=streaming", true);
    request.send(null); 
</script>
</html>
```
##### 后端action代码
我们在com.linjunlong.test.push.action.CometAction中添加polling方法
```java
```
    public ActionForward streaming(ActionMapping mapping,ActionForm form
            ,HttpServletRequest request,HttpServletResponse response) throws Exception{
        System.out.println("-----CometAction.streaming.start");
        StreamingThread st = new StreamingThread(response);
        st.run();
        System.out.println("-----CometAction.streaming.end");
        return null;
    }
下面是StreamingThread的代码。
```java
public class StreamingThread extends Thread {
        private HttpServletResponse response = null;
        
        public StreamingThread(HttpServletResponse response){
            this.response = response;
        }
        
        @Override
        public void run() {
            try{
                String message = "your hava a new message";
                PrintWriter writer = response.getWriter();
                for(int i = 0 ,max = message.length(); i < max ; i++) {
                    writer.print(message.substring(i,i+1));
                    writer.flush();
                    sleep(1000);
                }
                writer.close();
            }catch (Exception e) {
                // TODO: handle exception
            }
        }
    }
```
StreamingThread逻辑上是把我们想要输出的内容一个一个输出，每输出一个字，然后就休眠1秒钟，其实这个类想表达的意思是，服务器端接收到客户端想要获取信息的请求，可以先不做任何操作，只要永远不调用writer.close(); 服务器端就随时可以给客户端发送消息。这里的精髓是writer.flush(); sleep(1000);
##### 效果展示
在浏览器中输入http://localhost:8080/test/push/comet/streaming.jsp，浏览器上就一个字一个字的显示我们从后端取得的信息了。
![image](http://linjunlong.com/wp-content/uploads/2013/05/image_thumb12.png)
![image](http://linjunlong.com/wp-content/uploads/2013/05/image_thumb13.png)
这里可以看到这里请求数只有一个，但是请求时间却很长，在这很长的时间里，服务器只要一有消息就可以主动的推送消息过来。不过缺点就是。浏览器不兼容（ie下无法实现），为了达到浏览器兼容，于是就有了下面的即使iframe-streaming
#### iframe-streaming
这也是早先的常用做法。首先我们在页面里放置一个iframe，它的src设置为一个长连接的请求地址。Server端的代码基本一致，只是输出的格式改为HTML，用来输出一行行的Inline Javascript。由于输出就得到执行，因此就少了存储游标（pos）的过程。
##### 前端jsp代码
/test/WebContent/push/comet/iframe.jsp中编写下面代码
```java
<%@ page language="java" contentType="text/html; charset=GBK"
    pageEncoding="GBK"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<script type="text/javascript" src="../../static/js/jquery-1.8.0.min.js"></script>
<title >streaming test</title>
</head>
<body>
<div id="message"></div>
    <iframe id="iframe" src="about:blank" style="display: none;" ></iframe>
</body>
<script type="text/javascript">
    var add_content = function(str){
        $("#message").append(str);
    };
    $(document).ready(function(){
      $("#iframe")[0].src="../comet.do?method=iframe";
    });
</script>
</html>
```
可以看到我们在这jsp中定义了一个隐藏的iframe，他的地址是空的，因为在ie下，如果你写入一个地址，那么浏览器就会一直打转，给人一种页面还未加载万的假象，于是这里小林采用延迟加载的方式去等页面加载完再初始化请求地址
##### 后端action代码
我们在com.linjunlong.test.push.action.CometAction中添加iframe方法
```java
public ActionForward iframe(ActionMapping mapping,ActionForm form
        ,HttpServletRequest request,HttpServletResponse response) throws Exception{
    System.out.println("-----CometAction.iframe.start");
    IframeThread st = new IframeThread(response);
    st.run();
    System.out.println("-----CometAction.iframe.end");
    return null;
}
```
下面是IframeThread代码，与streaming逻辑上一样，只是输出的时候采用返回html脚本片段的方式，调用父页面的add_content 函数进行进行消息的添加，界面上的显示效果和streaming方式无异。
```java
public class IframeThread extends Thread {
        private HttpServletResponse response = null;
        
        public IframeThread(HttpServletResponse response){
            this.response = response;
        }
        
        @Override
        public void run() {
            try{
                String message = "your hava a new message";
                PrintWriter writer = response.getWriter();
                for(int i = 0 ,max = message.length(); i < max ; i++) {
                    writer.print("<script>parent.add_content('"+message.substring(i,i+1)+"');</script>");
                    writer.flush();
                    sleep(1000);
                }
                writer.close();
            }catch (Exception e) {
                // TODO: handle exception
            }
        }
    }
```
![image](http://linjunlong.com/wp-content/uploads/2013/05/image_thumb14.png)
用这种方式可以解决跨浏览器问题。
#### long-polling
长轮询是现在最为常用的方式，和流方式的区别就是服务器端在接到请求后挂起，有更新时返回连接即断掉，然后客户端再发起新的连接。很多大型网站都用这种技术。
##### 前端jsp代码
/test/WebContent/push/comet/long.jsp中编写下面代码
```java
<%@ page language="java" contentType="text/html; charset=GBK"
    pageEncoding="GBK"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<script type="text/javascript" src="../../static/js/jquery-1.8.0.min.js"></script>
<title >polling test</title>
</head>
<body>
<div id="message"></div>
</body>
<script type="text/javascript">
    var updater = {
    poll: function(){
        $.ajax({url: "../comet.do?method=longPolling", 
                type: "POST", 
                dataType: "text",
                success: updater.onSuccess,
                error: updater.onError});
    },
    onSuccess: function(data, dataStatus){
        try{
            $("#message").append(data);
        }
        catch(e){
            updater.onError();
            return;
        }
        interval = window.setTimeout(updater.poll, 0);
    },
    onError: function(){
        console.log("Poll error;");
    }
};
updater.poll();
</script>
</html>
```
后台action代码 
```java
public ActionForward longPolling(ActionMapping mapping,ActionForm form
        ,HttpServletRequest request,HttpServletResponse response) throws Exception{
    System.out.println("-----CometAction.longPolling.start");
    PrintWriter writer = response.getWriter();
    Thread longThread = new Thread() {
        public void run() {
            try {
                //这里模拟全局事件监听，如果其他模块有需要发送消息就发送一个事件，然后休眠停止，发送消息。
                sleep(5000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        };
    };
    longThread.run();
    writer.print("your hava a new message");
    writer.flush();
    writer.close();
    System.out.println("-----CometAction.longPolling.end");
    return null;
}
```
这里我们代码中，模拟型的休息5秒钟（其实要表达的意思是，这里让服务器hold住这个请求访问，等待服务器有消息了，就推送给用户）
##### 效果
![image](http://linjunlong.com/wp-content/uploads/2013/05/image_thumb15.png)
这里每次请求刚好5秒钟左右，但是实际运用中可能不止这么久。
#### WebSocket：未来方向
现在，很多网站为了实现即时通讯，所用的技术都是[轮询](https://zh.wikipedia.org/wiki/%E8%BC%AA%E8%A9%A2)。轮询是在特定的的时间间隔（如每1秒），由浏览器对伺服器发出HTTP request，然后由伺服器返回最新的数据给客户端的浏览器。这种传统的模式带来很明显的缺点，即浏览器需要不断的向伺服器发出请求，然而HTTP
 request 的header是非常长的，里面包含的数据可能只是一个很小的值，这样会占用很多的带宽和服务器资源。
而比较新的技术去做轮询的效果是[comet](https://zh.wikipedia.org/wiki/Comet_(web%E6%8A%80%E6%9C%AF))，使用了AJAX。但这种技术虽然可达到双向通信，但依然需要发出请求，而且在Comet中，普遍采用了[长链接](https://zh.wikipedia.org/wiki/HTTP%E6%8C%81%E4%B9%85%E9%93%BE%E6%8E%A5)，这也会大量消耗服务器带宽和资源。
面对这种状况，[HTML5](https://zh.wikipedia.org/wiki/HTML5)定义了WebSocket协议，能更好的节省服务器资源和带宽并达到实时通讯。关于WebSocket想了解更多，请看[这里](https://zh.wikipedia.org/wiki/WebSocket)
#### 总结
[小林](http://linjunlong.com/)这篇文章只是简单的演示了实现消息推送的方式，并没有比较系统的解决如何进行消息推送。实际过程中如果我们想要用于实战，可能要考虑客户端和服务器端直接的交流，服务器的压力，全局消息队列等等等。。。。。。最后希望这篇文章还能有下一篇，在这里先给自己挖个坑。。
