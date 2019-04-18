# pushlet实现服务端推送消息到浏览器 - z69183787的专栏 - CSDN博客
2014年03月22日 13:30:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4273
1.添加服务器推送功能
pushlet下载地址[http://www.pushlets.com/](http://www.pushlets.com/)，添加jar包pushlet.jar到项目的库中，需要用到的js文件ajax-pushlet-client.js,拷贝下载的压缩包中的文件pushlet.properties,sources.properties到项目的路径下
在web.xml中添加：
**[html]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/12260927#)[copy](http://blog.csdn.net/yangnianbing110/article/details/12260927#)
- <servlet>
- <servlet-name>pushlet</servlet-name>
- <servlet-class>nl.justobjects.pushlet.servlet.Pushlet</servlet-class>
- <load-on-startup>1</load-on-startup>
- </servlet>
- <servlet-mapping>
- <servlet-name>pushlet</servlet-name>
- <url-pattern>/pushlet.srv</url-pattern>
- </servlet-mapping>
2.服务器定义发送信息到客户端的实现：
首先定义一个事件源，该事件源负责推送消息到客户端
**[java]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/12260927#)[copy](http://blog.csdn.net/yangnianbing110/article/details/12260927#)
- publicclass HelloWorldPushlet extends EventPullSource implements Serializable {  
- privatestaticfinallong serialVersionUID = -4378845831200885879L;  
- 
- /**
-      * 推送时间间隔
-      */
- protectedlong getSleepTime() {  
- return1000;  
-     }  
- 
- /**
-      * 推送时所触发的方法
-      */
- protected Event pullEvent() {  
- //创建事件，并制定事件的主题
-         Event event = Event.createDataEvent("/ynb/helloworld");  
-         String data = "hello world,microbingbing" + System.currentTimeMillis();  
- try {  
-             data=new String(data.getBytes("UTF-8"),"ISO-8859-1");  
-         } catch (UnsupportedEncodingException e) {  
-             e.printStackTrace();  
-         }  
-         event.setField("message", data);  
- return event;  
-     }  
- }  
接下来我们需要注册该事件源,在sources.properties配置文件中添加
source7=com.ynb.servlet.HelloWorldPushlet
ajax形式 jsp页面：
**[html]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/12260927#)[copy](http://blog.csdn.net/yangnianbing110/article/details/12260927#)
- <%@ page language="java"contentType="text/html; charset=UTF-8"pageEncoding="UTF-8"%>
- <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=UTF-8">
- <title>pushlet测试页面</title>
- <scripttype="text/javascript"src="/pushlet/js/ajax-pushlet-client.js"></script>
- </head>
- <body>
- <divid="target"></div>
- <scripttype="text/javascript">
- ** //PL._init(); 无需初始化 否则chrome下会报错  **
-         PL.joinListen("/ynb/helloworld");//监听该主题的事件，如果发生该主题的事件，那么onData()方法会被调用  
- 
-         function onData(event){  
-             document.getElementById("target").innerText = event.get("message");  
-         }     
- </script>
- </body>
- </html>
iframe形式 jsp页面，需要调用 p_embed(),及载入另一个js-pushlet-client.js 文件，同时还需要 js-pushlet-net.html,
看下面源码：
 var pushletLayer = '<iframe id="pushletFrame" name="pushletFrame" src="' + pushletNetURI + '" style="visibility: hidden; width: 0px; height: 0px; border: 0px;"></iframe>';
self.document.write(pushletLayer);// 此处就是放入
可以修改源码 放入自定义的 div中 
```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<script type="text/javascript" src="/pushletDemo/lib/js-pushlet-client.js"></script>
<title>Insert title here</title>
</head>
<body onload="init();">
	<div id="target"></div>
	<div id="zs"></div>
	<input type="button" onclick="sendnews()" value="发送"/>
	<script type="text/javascript">
	function init() {
		//initDHTML();
		p_embed("/pushletDemo/");
		p_join_listen('/ynb/helloworld');
	}
		
	function onData(event){
		//alert(event.get("message"));
		}
	
		
		//点击发送按钮的时候触发事件的推送，p_publish()函数的第一个参数为触发事件的主题，其后的第2n个和2n+1个参数为事件对象中的属性
		function sendnews(){
			p_publish("/ynb/helloworld", "message", "message from other1111111111");
		}
	</script>
</body>
</html>
```
3.可控的消息推送，即由客户端控制推送消息的时间。
新建一个servlet处理消息的推送
**[java]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/12260927#)[copy](http://blog.csdn.net/yangnianbing110/article/details/12260927#)
- @WebServlet("/PushletServlet")  
- publicclass PushletServlet extends HttpServlet {  
- privatestaticfinallong serialVersionUID = 1L;  
- public PushletServlet() {  
- super();  
-     }  
- 
- @Override
- protectedvoid service(HttpServletRequest request, HttpServletResponse response)throws ServletException, IOException {  
-         Event event = Event.createDataEvent("/ynb/helloworld");  
-         event.setField("message", "message from service" + System.currentTimeMillis());  
-         Dispatcher.getInstance().multicast(event);//消息的推送有单播，组播，广播三种方式，对应不同的api调用
-     }  
- }  
每次调用该servlet都会触发主题为“/ynb/helloworld”的事件，如果监听了该事件的客户端就会收到推送的消息
4.在客户端触发消息的推送，而不需要自己编写servlet.
**[html]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/12260927#)[copy](http://blog.csdn.net/yangnianbing110/article/details/12260927#)
- <%@ page language="java"contentType="text/html; charset=UTF-8"
- pageEncoding="UTF-8"%>
- <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=UTF-8">
- <scripttype="text/javascript"src="/pushlet/js/ajax-pushlet-client.js"></script>
- <title>Insert title here</title>
- </head>
- <body>
- <divid="target"></div>
- <inputtype="button"onclick="sendnews()"value="发送"/>
- <scripttype="text/javascript">
-         PL._init();  
-         PL.joinListen("/other/helloworld");  
- 
-         function onData(event){  
-             document.getElementById("target").innerText = event.get("message");  
-         }  
-         //点击发送按钮的时候触发事件的推送，p_publish()函数的第一个参数为触发事件的主题，其后的第2n个和2n+1个参数为事件对象中的属性  
-         function sendnews(){  
-             p_publish("/ynb/helloworld", "message", "message from other" + new Date());  
-         }  
- </script>
- </body>
- </html>
Event.setField(name, value)的时候value的值不能为null，否则接受不到信息。
