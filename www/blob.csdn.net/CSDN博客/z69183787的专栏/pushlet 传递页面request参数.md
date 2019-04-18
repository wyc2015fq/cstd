# pushlet 传递页面request参数 - z69183787的专栏 - CSDN博客
2014年03月23日 19:46:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2586
原帖地址：[http://blog.csdn.net/meikidd/article/details/7446778](http://blog.csdn.net/meikidd/article/details/7446778)
最近项目中有服务器端推送的需求，考察了一下，感觉[pushlet](http://baike.baidu.com/view/2460420.html?fromTaglist)比较适合我们的情况。
用起来比较简单，网上资料也比较多（参考：[开源框架Pushlet入门](http://www.cnblogs.com/linjiqin/archive/2011/12/30/2307788.html)），就不多费笔墨了。
最常见的用法如下：
**[java]**[view
 plain](http://blog.csdn.net/meikidd/article/details/7446778#)[copy](http://blog.csdn.net/meikidd/article/details/7446778#)
- package com.ljq.test;  
- 
- import java.io.Serializable;  
- 
- import nl.justobjects.pushlet.core.Event;  
- import nl.justobjects.pushlet.core.EventPullSource;  
- 
- @SuppressWarnings("serial")  
- publicclass HelloWorldPlushlet extends EventPullSource implements Serializable {  
- 
- /**
-      * 设置休眠时间
-      */
- @Override
- protectedlong getSleepTime() {  
- return1000;  
-     }  
- 
- /**
-      * 创建事件
-      * 
-      * 业务部分写在pullEvent()方法中，这个方法会被定时调用。
-      */
- @Override
- protected Event pullEvent() {  
-         Event event = Event.createDataEvent("/linjiqin/hw");  
-         event.setField("hw", "HelloWorld!!!!");  
- return event;  
-     }  
- 
- }  
在使用的过程中发现，如果要在pullEvent()方法中获取参数比较麻烦，看了半天官方文档也没有找到好的方法（也可能是我没有找对地方，总感觉pushlet不会有这种问题，如果你们知道请一定告诉我）。只好去看源代码，发现在nl.justobjects.pushlet.servlet.Pushlet中已经将request参数传进了Session（注意是nl.justobjects.pushlet.core.Session）。但是在session构造的时候没有用到request。看到这里，就大概知道改怎么改了。代码如下：
1. nl.justobjects.pushlet.core.Session，添加了event域和getEvent()方法，修改了public static Session create(String anId, Event anEvent)方法
**[java]**[view
 plain](http://blog.csdn.net/meikidd/article/details/7446778#)[copy](http://blog.csdn.net/meikidd/article/details/7446778#)
- publicclass Session implements Protocol, ConfigDefs {  
- private Controller controller;  
- private Subscriber subscriber;  
- private Event event;  
- 
-     ...  
- 
- /**
-      * Create instance through factory method.
-      *
-      * @param anId a session id
-      * @return a Session object (or derived)
-      * @throws PushletException exception, usually misconfiguration
-      */
- publicstatic Session create(String anId, Event anEvent) throws PushletException {  
-         Session session;  
- try {  
-             session = (Session) Config.getClass(SESSION_CLASS, "nl.justobjects.pushlet.core.Session").newInstance();  
-         } catch (Throwable t) {  
- thrownew PushletException("Cannot instantiate Session from config", t);  
-         }  
- 
- // Init session
-         session.id = anId;  
-         session.controller = Controller.create(session);  
-         session.subscriber = Subscriber.create(session);  
-         session.event = anEvent;  
- return session;  
-     }  
- 
-     ...  
- 
- /**
-      * Return event.
-      */
- public Event getEvent() {  
- return event;  
-     }  
- 
-     ...  
- 
- }  
2. nl.justobjects.pushlet.core.SessionManager，修改了createSession()方法
**[java]**[view
 plain](http://blog.csdn.net/meikidd/article/details/7446778#)[copy](http://blog.csdn.net/meikidd/article/details/7446778#)
- /**
-  * Create new Session (but add later).
-  */
- public Session createSession(Event anEvent) throws PushletException {  
- // Trivial
- return Session.create(createSessionId(), anEvent);  
- }  
3. ajax-pushlet-client.js，PL添加了parameters属性，修改了_doRequest函数，在函数的最后加了如下一段：
**[javascript]**[view
 plain](http://blog.csdn.net/meikidd/article/details/7446778#)[copy](http://blog.csdn.net/meikidd/article/details/7446778#)
- if(PL.parameters.length > 0) {  
- for (var i = 0; i < PL.parameters.length; i++) {  
- var para = PL.parameters[i];  
-         url += "&" + para.name + "=" + para.value;  
-     }  
- }  
修改后的ajax-pushlet-client.js -_doRequest()函数：
**[javascript]**[view
 plain](http://blog.csdn.net/meikidd/article/details/7446778#)[copy](http://blog.csdn.net/meikidd/article/details/7446778#)
- _doRequest: function(anEvent, aQuery) {  
-         ...  
- 
- // Construct base URL for GET
- var url = PL.pushletURL + '?p_event=' + anEvent;  
- 
- // Optionally attach query string
- if (aQuery) {  
-             url = url + '&' + aQuery;  
-         }  
- 
- // Optionally attach session id
- if (PL.sessionId != null) {  
-             url = url + '&p_id=' + PL.sessionId;  
- if (anEvent == 'p_leave') {  
-                 PL.sessionId = null;  
-             }  
-         }  
- 
- if(PL.parameters.length > 0) {  
- for (var i = 0; i < PL.parameters.length; i++) {  
- var para = PL.parameters[i];  
-                 url += "&" + para.name + "=" + para.value;  
-             }  
-         }  
-         PL.debug('_doRequest', url);  
-         PL._getXML(url, PL._onResponse);  
-     },  
好了，源代码修改完毕，下面是一个如何传递参数的例子
在页面上js代码：
**[javascript]**[view
 plain](http://blog.csdn.net/meikidd/article/details/7446778#)[copy](http://blog.csdn.net/meikidd/article/details/7446778#)
- // pushlet服务器推送，更新实时监控模块
- var initPushlet = function() {  
-     PL.parameters.push({"name":"user-id", "value":"001");  
-     PL._init();  
-     PL.joinListen('/source/event');  
- };  
在HelloWorldPlushlet的pullEvent()方法调用：
**[java]**[view
 plain](http://blog.csdn.net/meikidd/article/details/7446778#)[copy](http://blog.csdn.net/meikidd/article/details/7446778#)
- Session[] sessions = SessionManager.getInstance().getSessions();  
- String userId = sessions[0].getEvent().getField("user-id");  
这样就实现了从页面到pushlet的参数传递
