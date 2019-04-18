# 如何基于BS架构开发聊天程序 - z69183787的专栏 - CSDN博客
2014年03月23日 13:09:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4296
最近经常看到关于“如何基于BS架构开发聊天程序”的问题，这里把写的一个简要实例代码贴出来。
那么BS的聊天系统，一般两种技术（各有优劣）：
1、基于Ajax或子页面刷新的拉模型；
2、基于HTTP长连接的推模型。
因为Ajax的“拉模型”，比较常见，这里给出基于HTTP长连接的“推模型”：
由三个页面组成：
chatmain.jsp 框架页面，引入两个Frame页面，并负责创建聊天室；
chatwnd.jsp 聊天信息窗口，负责用服务器推的方式显示所有人的聊天信息；
chatsender.jsp 聊天发送窗口，负责发送自己想说的话。
**注意**：本样例主要是为了说明 “基于长连接的推模型”，在聊天室完备性上完全不具有参考意义。
【chatmain.jsp 框架页面】
**[html]**[view
 plain](http://blog.csdn.net/ldh911/article/details/7268879#)[copy](http://blog.csdn.net/ldh911/article/details/7268879#)
- <%@ page language="java"contentType="text/html; charset=UTF-8"pageEncoding="UTF-8"%>
- <%@ page import="java.util.*"%>
- <%  
-     // 这三句话只是为了防止浏览器缓存  
-     response.setHeader("Cache-Control", "no-cache, no-store"); //HTTP 1.1    
-     response.setHeader("Pragma", "no-cache"); //HTTP 1.0    
-     response.setDateHeader("Expires", 0); //prevents caching at the proxy server  
- 
- 
-     // 检查并按需创建“聊天室”  
-     ArrayList<String>lstMsg = (ArrayList<String>) application.getAttribute("room");  
-     if (lstMsg == null) {  
-         application.setAttribute("room", new ArrayList<String>());  
-     }  
- %>
- <html>
- <framesetrows="80%, 20%">
- <framesrc="chatwnd.jsp"></frame>
- <framesrc="chatsender.jsp"></frame>
- </frameset>
- </html>

【chatwnd.jsp 聊天信息窗口】
**[html]**[view
 plain](http://blog.csdn.net/ldh911/article/details/7268879#)[copy](http://blog.csdn.net/ldh911/article/details/7268879#)
- <%@ page language="java"contentType="text/html; charset=UTF-8"pageEncoding="UTF-8"%>
- <%@ page import="java.util.*"%>
- <%@ page import="java.io.*"%>
- <html>
- <h2>Welcome to the ChatRoom!</h2>
- <%  
-     // 这三句话只是为了防止浏览器缓存  
-     response.setHeader("Cache-Control", "no-cache, no-store"); //HTTP 1.1      
-     response.setHeader("Pragma", "no-cache"); //HTTP 1.0      
-     response.setDateHeader("Expires", 0); //prevents caching at the proxy server    
- 
- 
-     Integer myMsgPos = 0; // 记录最后显示消息的位置  
-     ArrayList<String>lstMsg = (ArrayList<String>) application.getAttribute("room");  
-     out.flush(); // 把之前的HTML信息都刷出去  
-     PrintWriter pw = response.getWriter(); // 用PrintWriter取代JspWriter以便于检查Socket异常  
-     try {    
-         synchronized (lstMsg) {    
-             while (true) { // 死循环，也就意味着本次HTTP请求不会结束  
-                 if (lstMsg.size() > myMsgPos) { // 检查有没有新的消息  
-                     // 循环显示所有新消息  
-                     System.out.println("Got new msg to push: "+(lstMsg.size()-myMsgPos));  
-                     for (int i = myMsgPos; i <lstMsg.size(); i++) {    
-                         String msg = lstMsg.get(i);  
-                         pw.write("<p>"+msg+"</p>");  
-                     }  
-                     if (pw.checkError()) { // 强制将输出缓冲区内容发送出去，并检查异常  
-                         System.out.println("IOException detected, JSP end.");  
-                         break; // 说明已经发生IOException（一般是浏览器页面关掉了）  
-                     }  
- myMsgPos = lstMsg.size(); // 更新最后显示的消息位置  
-                 }  
-                 System.out.println("Waiting new msg..." + out.getRemaining());  
-                 lstMsg.wait(); // 临时释放对lstMsg的独占（同步锁），等待新消息的到来  
-             }  
-         }  
-     } catch (Exception ex) {    
-         System.out.println(ex);    
-     }  
- %>
- </html>
【chatsender.jsp 聊天发送窗口】
**[html]**[view
 plain](http://blog.csdn.net/ldh911/article/details/7268879#)[copy](http://blog.csdn.net/ldh911/article/details/7268879#)
- <%@ page language="java"contentType="text/html; charset=UTF-8"pageEncoding="UTF-8"%>
- <%@ page import="java.util.*" %>
- <%@ page import="java.text.*" %>
- <%  
-     SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");  
-     // 这三句话只是为了防止浏览器缓存  
-     response.setHeader("Cache-Control", "no-cache, no-store"); //HTTP 1.1  
-     response.setHeader("Pragma", "no-cache"); //HTTP 1.0      
-     response.setDateHeader("Expires", 0); //prevents caching at the proxy server  
- 
- 
-     // 获取POST的“发言”      
-     String msg = request.getParameter("msg");    
-     if (msg!=null && msg.length()>0) {    
- msg = "[" + sdf.format(new Date()) + "]" + session.getId() + ": " + msg;     
-         System.out.println(msg); // 调试跟踪用的信息  
-         ArrayList<String>lstMsg = (ArrayList<String>) application.getAttribute("room");    
-         synchronized (lstMsg) {  
-             lstMsg.add(msg); // 消息放入池中（消息太多会溢出的，这里并没有进行处理）  
-             lstMsg.notifyAll(); // 通知chatwnd.jsp们，有新的消息来了，可以推给浏览器了  
-         }  
-     }  
- %>
- <!DOCTYPE unspecified PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
- <html>
- <formmethod="post">
-     My Speak: <inputname="msg"type="text"size="40"/><inputtype="submit"/>
- </form>
- </html>
