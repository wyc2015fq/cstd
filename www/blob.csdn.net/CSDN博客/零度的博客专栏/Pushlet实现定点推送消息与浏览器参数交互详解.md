# Pushlet实现定点推送消息与浏览器参数交互详解 - 零度的博客专栏 - CSDN博客
2016年07月18日 14:04:28[零度anngle](https://me.csdn.net/zmx729618)阅读数：1093
今天在这里讲解一下关于开源框架Pushlet中的定点推送消息和与浏览器参数交互 
　  通过上面的方法我就可以完成点对点的网页版本的聊天软件了，当然需要达到上面的要求我们这里需要对Pushlet的源码进行改进。 
　　首先，我这里就讲述Pushlet的入门配置，默认大家是了解Pushlet框架的。 
1. 与浏览器参数交互
　　需要定位一个客户端就必须要给这个客户设置一个唯一的key值，我这里就叫它为clientid，当然也有朋友会说，我使用Pushlet中每个Session的id来作为clientid，这样当然是可以的，但为了便于管理我们一般会生成自己的clientid来进行操作，我在这里要做的就是使用自己生成的clientid来替换Pushlet自己生成的Sesssion的id。 
　　在Pushlet初始化去监听一个事件的时候我们是没有办法传递参数的，这里我就需要改动一下ajax-pushlet-client.js中的代码，来满足我们的需求，在_doRequest函数最下面加上如下代码： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/21961211#)[copy](http://blog.csdn.net/z69183787/article/details/21961211#)[print](http://blog.csdn.net/z69183787/article/details/21961211#)[?](http://blog.csdn.net/z69183787/article/details/21961211#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253579/fork)
- _doRequest: function(anEvent, aQuery) {  
-     ......  
- 
- if(PL.parameters.length > 0) {  
- for (var i = 0; i < PL.parameters.length; i++) {  
-             var para = PL.parameters[i];  
-             url += "&" + para.name + "=" + para.value;  
-         }  
-     }  
- 
-     ......      
- },  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
_doRequest: function(anEvent, aQuery) {
         ......
         
         if(PL.parameters.length > 0) {
             for (var i = 0; i < PL.parameters.length; i++) {
                 var para = PL.parameters[i];
                 url += "&" + para.name + "=" + para.value;
             }
         }
         
         ......    
     },
```
    加入代码后的_doRequest函数如下： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/21961211#)[copy](http://blog.csdn.net/z69183787/article/details/21961211#)[print](http://blog.csdn.net/z69183787/article/details/21961211#)[?](http://blog.csdn.net/z69183787/article/details/21961211#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253579/fork)
- _doRequest: function(anEvent, aQuery) {  
-      ......  
- 
- // Construct base URL for GET
-      var url = PL.pushletURL + '?p_event=' + anEvent;  
- 
- // Optionally attach query string
- if (aQuery) {  
-          url = url + '&' + aQuery;  
-      }  
- 
- // Optionally attach session id
- if (PL.sessionId != null) {  
-          url = url + '&p_id=' + PL.sessionId;  
- if (anEvent == 'p_leave') {  
-              PL.sessionId = null;  
-          }  
-      }  
- 
- if(PL.parameters.length > 0) {  
- for (var i = 0; i < PL.parameters.length; i++) {  
-              var para = PL.parameters[i];  
-              url += "&" + para.name + "=" + para.value;  
-          }  
-      }  
- 
-      PL.debug('_doRequest', url);  
-      PL._getXML(url, PL._onResponse);  
- 
-  },  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
_doRequest: function(anEvent, aQuery) {
         ......
         
         // Construct base URL for GET
         var url = PL.pushletURL + '?p_event=' + anEvent;
 
         // Optionally attach query string
         if (aQuery) {
             url = url + '&' + aQuery;
         }
 
         // Optionally attach session id
         if (PL.sessionId != null) {
             url = url + '&p_id=' + PL.sessionId;
             if (anEvent == 'p_leave') {
                 PL.sessionId = null;
             }
         }
         
         if(PL.parameters.length > 0) {
             for (var i = 0; i < PL.parameters.length; i++) {
                 var para = PL.parameters[i];
                 url += "&" + para.name + "=" + para.value;
             }
         }
         
         PL.debug('_doRequest', url);
         PL._getXML(url, PL._onResponse);
 
     },
```
    代码改好后就可以这样写我的Pushlet初始化代码了，传入我们所需要的clientid给服务器，代码如下： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/21961211#)[copy](http://blog.csdn.net/z69183787/article/details/21961211#)[print](http://blog.csdn.net/z69183787/article/details/21961211#)[?](http://blog.csdn.net/z69183787/article/details/21961211#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253579/fork)
- var initPushlet = function() {  
-      PL._init();  
-      PL.parameters.push({"clientid":"servicekey", "value":"4214f0c0da6760a9e95e3c164998ac06"});  
-      PL.joinListen('/conversation');  
-  };  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
var initPushlet = function() {
     PL._init();
     PL.parameters.push({"clientid":"servicekey", "value":"4214f0c0da6760a9e95e3c164998ac06"});
     PL.joinListen('/conversation');
 };
```
　　那么，客户端把参数传过来了服务端要怎么接收它呢？这就就需要修改一下Pushlet的java源码，首先我们修改nl.justobjects.pushlet.core.SessionManager中的createSession方法，如下： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/21961211#)[copy](http://blog.csdn.net/z69183787/article/details/21961211#)[print](http://blog.csdn.net/z69183787/article/details/21961211#)[?](http://blog.csdn.net/z69183787/article/details/21961211#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253579/fork)
- /**
-  * Create new Session (but add later).
-  */
- public Session createSession(Event anEvent) throws PushletException {  
- // Trivial
- return Session.create(anEvent);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
/**
      * Create new Session (but add later).
      */
     public Session createSession(Event anEvent) throws PushletException {
         // Trivial
         return Session.create(anEvent);
     }
```
    再修改nl.justobjects.pushlet.core.Session中的create方法，如下： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/21961211#)[copy](http://blog.csdn.net/z69183787/article/details/21961211#)[print](http://blog.csdn.net/z69183787/article/details/21961211#)[?](http://blog.csdn.net/z69183787/article/details/21961211#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253579/fork)
- publicstatic Session create(Event anEvent) throws PushletException {  
-     Session session;  
- try {  
-         session = (Session) Config.getClass(SESSION_CLASS, "nl.justobjects.pushlet.core.Session").newInstance();  
-     } catch (Throwable t) {  
- thrownew PushletException("Cannot instantiate Session from config", t);  
-     }  
- 
- // get clientid
-     session.id = anEvent.getField("clientid");  
-     session.controller = Controller.create(session);  
-     session.subscriber = Subscriber.create(session);  
-     session.event = anEvent;  
- return session;  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public static Session create(Event anEvent) throws PushletException {
         Session session;
         try {
             session = (Session) Config.getClass(SESSION_CLASS, "nl.justobjects.pushlet.core.Session").newInstance();
         } catch (Throwable t) {
             throw new PushletException("Cannot instantiate Session from config", t);
         }
 
         // get clientid
         session.id = anEvent.getField("clientid");
         session.controller = Controller.create(session);
         session.subscriber = Subscriber.create(session);
         session.event = anEvent;
         return session;
     }
```
　　上面就完成了SessionId的转变。 
　　如果我们要完成一个点对点聊天的话客户端和服务器通信当然是必不可少，具体怎么向服务端发消息呢？在这里我使用里Pushlet的js中的p_publish()方法，示例代码如下： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/21961211#)[copy](http://blog.csdn.net/z69183787/article/details/21961211#)[print](http://blog.csdn.net/z69183787/article/details/21961211#)[?](http://blog.csdn.net/z69183787/article/details/21961211#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253579/fork)
- p_publish('answer','msg',encodeURIComponent('你好吗？'),'clientid','ce8954e8557fa9db8c1b2d6774e471a6');  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
```java
p_publish('answer','msg',encodeURIComponent('你好吗？'),'clientid','ce8954e8557fa9db8c1b2d6774e471a6');
```
　　上面方法第一个参数是：执行的操作命令command，后面是都是传给服务器的参数，是以一个参数名+参数值的形式。参数值如果存在中文的话需要使用encodeURIComponent()方法来进行编码再传输。 
　　在服务端我们要接收这一个客户端发过来的消息并进行处理，我需要怎么接收它呢？就需要在nl.justobjects.pushlet.core.Controller这个类中的doCommand()方法中进行处理了，找到这个方法中的eventType.equals(Protocol.E_PUBLISH)判断内，并加入自己的处理代码，代码如下： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/21961211#)[copy](http://blog.csdn.net/z69183787/article/details/21961211#)[print](http://blog.csdn.net/z69183787/article/details/21961211#)[?](http://blog.csdn.net/z69183787/article/details/21961211#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253579/fork)
- publicvoid doCommand(Command aCommand) {  
- try {  
-         ......  
-         } elseif (eventType.equals(Protocol.E_PUBLISH)) {  
- // Publish event
-             doPublish(aCommand);  
- 
- // get command
-             System.out.println(aCommand.reqEvent.getField("p_subject"));  
- // get clientid
-             System.out.println(aCommand.reqEvent.getField("clientid"));  
- // get msg
-             System.out.println(new String(aCommand.reqEvent.getField("msg").getBytes("ISO8859-1"), "UTF-8"));  
-         } elseif (eventType.equals(Protocol.E_LISTEN)) {  
- // Listen to pushed events
-             doListen(aCommand);  
-         }  
- 
-         ......  
-     } catch (Throwable t) {  
-         warn("Exception in doCommand(): " + t);  
-         t.printStackTrace();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public void doCommand(Command aCommand) {
         try {
             ......
             } else if (eventType.equals(Protocol.E_PUBLISH)) {
                 // Publish event
                 doPublish(aCommand);
                 
                 // get command
                 System.out.println(aCommand.reqEvent.getField("p_subject"));
                 // get clientid
                 System.out.println(aCommand.reqEvent.getField("clientid"));
                 // get msg
                 System.out.println(new String(aCommand.reqEvent.getField("msg").getBytes("ISO8859-1"), "UTF-8"));
             } else if (eventType.equals(Protocol.E_LISTEN)) {
                 // Listen to pushed events
                 doListen(aCommand);
             }
 
             ......
         } catch (Throwable t) {
             warn("Exception in doCommand(): " + t);
             t.printStackTrace();
         }
     }
```
　　aCommand.reqEvent.getField("p_subject"))就可以获得客户端传过来的command命令，aCommand.reqEvent.getField("参数名")来获取传过来参数。当然在正常的开发中，所用到的命令会很多，我们可以把每一个命令做成一个个子类去实现，完成不同的需求。至于怎么将消息发送给客户端，将在下面的定点推送消息中讲解。 
2. 定点推送消息 
　　上面已经讲述了每个客户端都会存在一个clientid，这个clientid可以定位每一个客户端，那么既然我们需要向某一个客户端推送消息，那必然要知道这个客户端的clientid，这里我建议开发者自己写一个管理器用于管理连接上来的客户端，用于保存这些客户端的信息。如果需要开发聊天程序的话则需要写一个聊天会话管理器，用于管理一对对的聊天会话Session。 
　　言归正传，我们接着1内容中的最后说，当我们在doCommand()方法中获得了某一个客户端传过来的消息时，我们可能需要做的是将处理的结果返回给它，或将它的消息发给另外一个客户端，至于怎么做呢？我们直接看代码，还是上面的代码加入了些内容如下： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/21961211#)[copy](http://blog.csdn.net/z69183787/article/details/21961211#)[print](http://blog.csdn.net/z69183787/article/details/21961211#)[?](http://blog.csdn.net/z69183787/article/details/21961211#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253579/fork)
- publicvoid doCommand(Command aCommand) {  
- try {  
-         ......  
-         } elseif (eventType.equals(Protocol.E_PUBLISH)) {  
- // Publish event
-             doPublish(aCommand);  
- 
- // get command
-             System.out.println(aCommand.reqEvent.getField("p_subject"));  
- // get clientid
-             System.out.println(aCommand.reqEvent.getField("clientid"));  
- // get msg
-             System.out.println(new String(aCommand.reqEvent.getField("msg").getBytes("ISO8859-1"), "UTF-8"));  
- 
- // 发送给其他客户端
-             Event event = Event.createDataEvent("/conversation"); // 监听的事件
-             event.setField("cmd", "say");  
-             event.setField("code", 100);  
-             event.setField("msg", new String(aCommand.reqEvent.getField("msg").getBytes("ISO8859-1"), "UTF-8"));  
-             Dispatcher.getInstance().unicast(event, aCommand.reqEvent.getField("clientid"));// 指定的clientid
- 
- // 回传给自己
-             event = Event.createDataEvent("/conversation"); // 监听的事件
-             event.setField("cmd", "answerResult");  
-             event.setField("code", 100);  
-             Dispatcher.getInstance().unicast(event, session.getId());// 当前sessionid即为clientid
- 
-         } elseif (eventType.equals(Protocol.E_LISTEN)) {  
- // Listen to pushed events
-             doListen(aCommand);  
-         }  
- 
-         ......  
-     } catch (Throwable t) {  
-         warn("Exception in doCommand(): " + t);  
-         t.printStackTrace();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public void doCommand(Command aCommand) {
         try {
             ......
             } else if (eventType.equals(Protocol.E_PUBLISH)) {
                 // Publish event
                 doPublish(aCommand);
                 
                 // get command
                 System.out.println(aCommand.reqEvent.getField("p_subject"));
                 // get clientid
                 System.out.println(aCommand.reqEvent.getField("clientid"));
                 // get msg
                 System.out.println(new String(aCommand.reqEvent.getField("msg").getBytes("ISO8859-1"), "UTF-8"));
                 
                 // 发送给其他客户端
                 Event event = Event.createDataEvent("/conversation"); // 监听的事件
                 event.setField("cmd", "say");
                 event.setField("code", 100);
                 event.setField("msg", new String(aCommand.reqEvent.getField("msg").getBytes("ISO8859-1"), "UTF-8"));
                 Dispatcher.getInstance().unicast(event, aCommand.reqEvent.getField("clientid"));// 指定的clientid
                 
                 // 回传给自己
                 event = Event.createDataEvent("/conversation"); // 监听的事件
                 event.setField("cmd", "answerResult");
                 event.setField("code", 100);
                 Dispatcher.getInstance().unicast(event, session.getId());// 当前sessionid即为clientid
                 
             } else if (eventType.equals(Protocol.E_LISTEN)) {
                 // Listen to pushed events
                 doListen(aCommand);
             }
 
             ......
         } catch (Throwable t) {
             warn("Exception in doCommand(): " + t);
             t.printStackTrace();
         }
     }
```
　　上面代码就实现了将消息发给指定客户端，以及回传消息给自己。客户端要如何接收服务端传过来的消息，见如下代码： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/21961211#)[copy](http://blog.csdn.net/z69183787/article/details/21961211#)[print](http://blog.csdn.net/z69183787/article/details/21961211#)[?](http://blog.csdn.net/z69183787/article/details/21961211#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253579/fork)
- function onData(event) {  
-      alert(event.get("cmd"));  
-      alert(event.get("code"));  
-      alert(event.get("msg"));  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
function onData(event) {
     alert(event.get("cmd"));
     alert(event.get("code"));
     alert(event.get("msg"));
}
```
　　页面上写上此js就可以接收到服务器传过来的内容，内容全都包含在event当中了。 
总结：到此为止我们对Pushlet中的定点推送消息和与浏览器参数交互有了比较深入的学习，大家可以按照这个方案开发自己想要的东西了。 
