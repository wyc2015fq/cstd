# 使用Pushlet将消息从服务器端推送到客户端 - z69183787的专栏 - CSDN博客
2014年03月23日 12:13:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：806
使用Pushlet来实现服务器端向客户端推送信息
# 1.实现方式：
有两种实现方式：
1.通过配置文件来实现定时的从服务器端向客户端推送信息
2.通过API主动向另外一端推送信息
以下分别给予介绍。
# 2.特别注意
在开始测试之前，有三点非常重要，需要实现讲明，否则程序将会无法正常运行：
# 2.1.JSP页面上的设定
JSP页面上必须添加以下代码以确保Pushlet能够正确的获得后台服务的地址：
<base href="<%=request.getContextPath()%>">
# 2.2.Pushlet的JS文件的Bug修改
需要修改被引用的JS文件ajax-pushlet-client.js的内容，找到
PL.pushletURL = PL._getWebRoot() + 'pushlet.srv';
将其修改为
PL.pushletURL = 'pushlet.srv';
修改的原因是Pushlet进行地址解析的方法在某些应用中会解析错误，导致请求的路径是nullpushlet.srv?，最终导致无法正确的请求到服务器的信息。
# 2.3.中文问题
一般情况下，如果不做特殊处理，中文问题将会导致Pushlet的客户端停止响应，解决办法是，在使用Pushlet的客户端代码发送消息之前，将其进行转码，代码为
encodeURIComponent( msg)
# 3.正式开始
以上准备工作完毕，就可以正式的开发测试样例了。
# 3.1.定时的从后台向前台push信息
(1)在eclipse中创建一个动态的web工程
(2)配置及库文件文件：从[http://www.pushlets.com/](http://www.pushlets.com/)下载最新的pushlet的开发包，将其中的以下文件按照描述进行设定
|**序号**|**文件名**|**源位置**|**目标位置**|**备注**|
|----|----|----|----|----|
|1.|pushlet.jar|{pushlet-2.0.4}\lib|项目类路径|如果使用的是applet的话，还需要将pushletclient.jar设置到项目的类路径中去|
|2.|log4j.propertiespushlet.propertiessources.properties|{pushlet-2.0.4}\webapps\pushlet\WEB-INF\classes|项目的src根路径|注意稍后需要修改sources.properties，其他两个文件的内容不需要修改|
|3.|ajax-pushlet-client.js|{pushlet-2.0.4}\webapps\pushlet\lib|项目的webroot\lib|需要按照之前的描述修改其中的内容|
(3)修改web.xml，将pushlet的自启动servlet添加进去
 <servlet>
 <servlet-name>pushlet</servlet-name>
 <servlet-class>nl.justobjects.pushlet.servlet.Pushlet</servlet-class>
 <load-on-startup>1</load-on-startup>
 </servlet>
 <servlet-mapping>
 <servlet-name>pushlet</servlet-name>
 <url-pattern>/pushlet.srv</url-pattern>
 </servlet-mapping>
(4)创建服务器端代码，特别注意类和内部静态类的名字
**package**com.guoguo;
**import**java.io.Serializable;
**import**java.io.UnsupportedEncodingException;
**import**sun.rmi.runtime.Log;
**import**nl.justobjects.pushlet.core.Event;
**import**nl.justobjects.pushlet.core.EventPullSource;
**publicclass****HelloWorldPushlet****implements**Serializable
 {
**privatestaticfinallong***serialVersionUID*=
 -8940934044114406724L;
**publicstaticclass****HWPushlet****extends**EventPullSource
 {
 Loglog=
 Log.*getLog*(HWPushlet.**class**.getName(),
 HWPushlet.**class**.getName(),**true**);
@Override
**protectedlong**getSleepTime()
 {
**return**1000;//每一秒钟自动执行一次
 }
@Override
**protected**Event
 pullEvent() {
 //注意，一下是设定消息的主题/guoguo/helloworld，号称主题是可以继承的
 //但是笔者的测试是失败的，也许方法不对，呵呵
 Event
 event = Event.*createDataEvent*("/guoguo/helloworld");
 String
 data="hello,world郭强"+System.*currentTimeMillis*();
**try**{
 data=**new**String(data.getBytes("UTF-8"),"ISO-8859-1");
 }**catch**(UnsupportedEncodingException
 e) {
 e.printStackTrace();
 }
 event.setField("hw",data);
**return**event;
 }
 }
}
(5)注册服务器端代码为事件源，在sources.properties文件中，添加以下行
source7=com.guoguo.**HelloWorldPushlet**$**HWPushlet**
以上方式适用于有内部类的情况，如果没有内部类的话，使用以下的方式进行注册(这时外部类必须继承父类EventPullSource)
source7=com.guoguo.**HelloWorldPushlet**
(6)页面（可以参考页面内注释信息）
<%@pagelanguage=*"java"*contentType=*"text/html; charset=ISO-8859-1"*
pageEncoding=*"ISO-8859-1"*%>
<!DOCTYPEhtmlPUBLIC"-//W3C//DTD
 HTML 4.01 Transitional//EN""http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<metahttp-equiv=*"Content-Type"*content=*"text/html; charset=ISO-8859-1"*>
<title>Pushlet
 Test</title>
<metahttp-equiv=*"pragma"*content=*"no-cache"*>
<metahttp-equiv=*"cache-control"*content=*"no-cache"*>
<basehref=*"*<%=request.getContextPath()%>*"*>
</head>
<body>
<scripttype=*"text/javascript"*src=*"lib/ajax-pushlet-client.js"*></script>
<divid=*"guoguo"*></div>
<scripttype=*"text/javascript"*>
 //初始化pushlet客户端
** // PL._init();最新版本无需初始化 ，不然 chrome 会报错**
 //设定运行时显示调试信息，不需要时，直接删掉即可
 PL.setDebug(**true**);
 //设定监听主题：/guoguo/helloworld，与服务器端的主题完全一致
 PL.joinListen('/guoguo/helloworld');
 //接收到事件后，显示服务器信息
**function**onData(event)
 {
 guoguo.innerText=(event.get("hw"));
 }
</script>
<p1>Pushlet
 Test</p1>
</body>
</html>
(7)启动服务器，即可看到页面上的信息每秒钟一次，进行定时的更新
# 3.2.主动控制发送消息
### 3.2.1.有刷新的提交信息（服务器端主动发送消息）
(1)创建一个servlet，并且注册到web.xml中
Servlet代码
package com.guoguo;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import nl.justobjects.pushlet.core.Dispatcher;
import nl.justobjects.pushlet.core.Event;
import nl.justobjects.pushlet.core.SessionManager;
public class ChatServlet extends HttpServlet {
 private static final long serialVersionUID = 1L;
 public ChatServlet() {
 super();
 }
 protected void service(HttpServletRequest request,
 HttpServletResponse
 response) throws ServletException, IOException {
 //
 myUnicast();
 myMulticast();
 //
 myBroadcast();
 request.getRequestDispatcher("chat.jsp").forward(request,
 response);
 }
 private void myUnicast() {
 Event
 event = Event.createDataEvent("/guoguo/myevent1");
 event.setField("key1",
 "Unicast msg");
 Dispatcher.getInstance().unicast(event,
 "piero"); //向ID为piero的用户推送
 System.out.println("success....");
 }
 private void myMulticast() {
 Event
 event = Event.createDataEvent("/guoguo/myevent1");
 //Event
 event = Event.createDataEvent("/guoguo");
 event.setField("key1",
 "Multicast msg");
 Dispatcher.getInstance().multicast(event);
 //向所有和myevent1名称匹配的事件推送
 System.out.println("wa
 success....");
 }
 private void myBroadcast() {
 Event
 event = Event.createDataEvent("/guoguo/myevent1"); //向所有的事件推送，不要求和这儿的myevent1名称匹配
 event.setField("key1",
 "Broadcast msg");
 Dispatcher.getInstance().broadcast(event);
 System.out.println("asw
 success....");
 }
}
Web.xml
<servlet>
<display-name>ChatServlet</display-name>
<servlet-name>ChatServlet</servlet-name>
<servlet-class>com.guoguo.ChatServlet</servlet-class>
</servlet>
<servlet-mapping>
<servlet-name>ChatServlet</servlet-name>
<url-pattern>/ChatServlet</url-pattern>
</servlet-mapping>
(2)页面端代码
发送端
<base href="<%=request.getContextPath()%>">
<formaction=*"*<%=request.getContextPath()%>*/ChatServlet"*>
<inputtype=*"submit"*>
</form>
接收端
<base href="<%=request.getContextPath()%>">
<scripttype=*"text/javascript"*src=*"lib/ajax-pushlet-client.js"*></script>
<divid=*"guoguo"*></div>
<scripttype=*"text/javascript"*>
 PL._init();
 PL.joinListen('/guoguo/myevent1');
**function**onData(event)
 {
 guoguo.innerText
 = (event.get("key1"));
 }
</script>
启动服务器，从发送端提交信息，内容会在接收端显示出来
### 3.2.2.无刷新的提交信息（从客户端发送消息）
发送端
<base href="<%=request.getContextPath()%>">
<scripttype=*"text/javascript"*src=*"lib/ajax-pushlet-client.js"*></script>
<scripttype=*"text/javascript"*>
 PL._init();
**function**sendnews(msg)
 {
 p_publish('/guoguo/myevent1','key1',encodeURIComponent(msg),’key2’,’msg2’);
 }
</script>
<inputtype=*"text"*name=*"mymsg"*>
<inputtype=*"button"*value=*"**发消息"*onclick="sendnews(mymsg.value)"/>
接收端
<base href="<%=request.getContextPath()%>">
<scripttype=*"text/javascript"*src=*"lib/ajax-pushlet-client.js"*></script>
<divid=*"guoguo"*></div>
<scripttype=*"text/javascript"*>
 PL._init();
 PL.joinListen('/guoguo/myevent1');
**function**onData(event)
 {
 guoguo.innerText
 = (event.get("key1"));
 }
</script>
启动服务器，从发送端提交信息，内容会在接收端显示出来
