# HTML5 Server-Sent Events with Java Servlets example - z69183787的专栏 - CSDN博客
2016年09月13日 11:24:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：707
个人分类：[前端-Html5																[Websocket](https://blog.csdn.net/z69183787/article/category/2837859)](https://blog.csdn.net/z69183787/article/category/5816199)
## 1. Introduction to Server-Sent Events
The Server-Sent Events are the new APIs for opening an HTTP connection for receiving push notifications from a server in the form of DOM events. Consider below javascript code:
```php
```php
//...
var eventSource = new EventSource("/some/data.html");
eventSource.onmessage = function(event) {
	alert(event.data);
};
//...
```
```
The above code can be executed in any modern browser (ofcourse the server-side script /some/data.html needs to be implemented first). We create an object of class EventSource. We pass a server URL which implements Server-sent events protocol. And finally we
 add a handler function on .onmessage. Every time server sends a message, the handler is called and the event object will have the data.
No need for pooling. The javascript handler will be asynchronously called.
The server needs to specific. First the response content type must be set to `text/event-stream`.
 And the data needs to be sent in following format.
```
...
data: This is some data
data: a quick brown fox
data: jumps over a lazy dog.
...
```
You got the idea. Lets quickly create a Java Servlet based application with Client code for Server-sent event.
## 2. Server-Sent Events, Hello World Servlet
For our hello world example, we create an html page that has a button to start server-sent event. The server will send us a timestamp every second which we just display on page.
index.jsp
```php
```php
<!DOCTYPE HTML>
<html>
<body>
	Time: <span id="foo"></span>
	
	<br><br>
	<button onclick="start()">Start</button>
	<script type="text/javascript">
	function start() {
		var eventSource = new EventSource("HelloServlet");
		
		eventSource.onmessage = function(event) {
		
			document.getElementById('foo').innerHTML = event.data;
		
		};
		
	}
	</script>
</body>
</html>
```
```
In above javascript code we created an event source for “/HelloServlet” path. On each message we just prints the data in span “foo”.
Now lets check Servlet code which sents the Server-Sent Events.
HelloServlet.java
```java
```java
package net.viralpatel.servlets;
import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
public class TestServlet extends HttpServlet {
	
	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
	
		//content type must be set to text/event-stream
		response.setContentType("text/event-stream");	
		//encoding must be set to UTF-8
		response.setCharacterEncoding("UTF-8");
		PrintWriter writer = response.getWriter();
		for(int i=0; i<10; i++) {
			writer.write("data: "+ System.currentTimeMillis() +"\n\n");
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		writer.close();
	}
}
```
```
The servlet is quite simple. Content type must be set to “text/event-stream” and the character encoding must be UTF-8.
Also each message that we send must ends with \n\n.
web.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app id="WebApp_ID" version="2.4"
	xmlns="http://java.sun.com/xml/ns/j2ee" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd">
	<display-name>ServerSentEvent_HttpServlet_example</display-name>
	<welcome-file-list>
		<welcome-file>index.jsp</welcome-file>
	</welcome-file-list>
	<servlet>
		<display-name>HelloServlet</display-name>
		<servlet-name>HelloServlet</servlet-name>
		<servlet-class>net.viralpatel.servlets.HelloServlet</servlet-class>
	</servlet>
	<servlet-mapping>
		<servlet-name>HelloServlet</servlet-name>
		<url-pattern>/HelloServlet</url-pattern>
	</servlet-mapping>
</web-app>
```
Output
![html5-server-sent-events-java-servlet](http://img.viralpatel.net/2013/10/html5-server-sent-events-java-servlet.png)
Source code:[https://gist.github.com/viralpatel/7007662](https://gist.github.com/viralpatel/7007662)
## 3. Multiple Events in Server-Sent Events
In above example we tracked a single event. The servlet sent only single data entity. In real world, you might want to send a number of events and want to track same on client side.
For example, consider below Javascript snippet.
```php
```php
var eventSource = new EventSource("HelloServlet");
		eventSource.addEventListener('up_vote', function(event) {
			
				document.getElementById('up').innerHTML = event.data;
				
			}, false);
		
		eventSource.addEventListener('down_vote', function(event) {
			
				document.getElementById('down').innerHTML = event.data;
				
			}, false);
```
```
In above code we used `addEventListener()` method
 of EventSource to add a handler function. We added handler to event “up_vote” and “down_vote”. Whenever any of this event changes, we need to update the count on html page.
The server must send the data in format:
```
...
event: up_vote
data: 10
event: down_vote
data: 5
event: up_vote
data: 12
event: down_vote
data: 9
...
```
The Java Servlet code for this example should be:
HelloServlet.java
```java
```java
package net.viralpatel.servlets;
import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
public class HelloServlet extends HttpServlet {
	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		response.setContentType("text/event-stream");
		response.setCharacterEncoding("UTF-8");
		PrintWriter writer = response.getWriter();
		int upVote = 0;
		int downVote = 0;
		for (int i = 0; i < 20; i++) {
			upVote = upVote + (int) (Math.random() * 10);
			downVote = downVote + (int) (Math.random() * 10);
			writer.write("event:up_vote\n");
			writer.write("data: " + upVote + "\n\n");
			writer.write("event:down_vote\n");
			writer.write("data: " + downVote + "\n\n");
			writer.flush();
			
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		writer.close();
	}
}
```
```
In above servlet code, we randomly incremented two ints upVote and downVote. Also we sent these ints to client with:
```java
```java
writer.write("event:up_vote\n");
writer.write("data: " + upVote + "\n\n");
```
```
Notice single \n in event and double \n\n in
 data.
You should get following output when the servlet is executed.
![custom-event-server-sent-events-servlet](http://img.viralpatel.net/2013/10/custom-event-server-sent-events-servlet.png)
### 3.1. The EventSource API
In the examples above we used the onmessage event to get messages. But other events are also available:
|Events|Description|
|----|----|
|onopen|When a connection to the server is opened|
|onmessage|When a message is received|
|onerror|When an error occurs|
## 4. Browser compatibility
As of Oct 2013 – Internet explorer is your enemy :) Server-sent events are supported in all major browsers (Firefox, Opera, Chrome, Safari.) except for Internet Explorer.
