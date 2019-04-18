# pushlet 之 无法传送中文解决办法 - z69183787的专栏 - CSDN博客
2014年03月22日 13:38:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1137
好多示例只是示例，没有说这个问题，既然发现了，我就就地解决一下！
我的方法很简单，对要传送的内容进行编码！
Java后台和JS前台都有包装的编解码方法，可以直接使用看效果！
后台编码（解码自己搞）：
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- package com;  
- import java.net.URLEncoder;  
- import java.text.SimpleDateFormat;  
- import java.util.Date;  
- import nl.justobjects.pushlet.core.Event;  
- import nl.justobjects.pushlet.core.EventPullSource;  
- publicclass HelloWorldPlushlet {     
- staticpublicclass HwPlushlet extends EventPullSource {  
- // 休眠五秒
- @Override
- protectedlong getSleepTime() {  
- return2000;  
-         }  
- @SuppressWarnings("deprecation")  
- @Override
- protected Event pullEvent() {  
-             Event event = Event.createDataEvent("/cuige/he");  
- try {  
-                 SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");  
-                 String text = format.format(new Date());                  
-                 String str = "传递中文测试！" + text + "！";  
-                 str = URLEncoder.encode(str,"UTF-8");  
-                 event.setField("cnmess", str);  
-             } catch (Exception e) {  
-                 event.setField("mess", "异常错误！");  
-             }  
- return event;  
-         }  
-     }  
- }  
 URLEncoder.encode有两个方法，一个参数的两个参数的，由于一个参数的已经不建议使用，所以这里使用了两个参数的方法！
至于编码方式，这个由所在系统约定，这里使用UTF-8编码！加上个时间戳看效果！
前台解码（JS也可以编码，自己搞）：
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- <%@ page language="java"import="java.util.*" pageEncoding="UTF-8"%>  
- <html>  
-     <head>  
-         <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />  
-         <meta http-equiv="Pragma" content="no-cache" />  
-         <script type="text/javascript" src="ajax-pushlet-client.js"></script>         
-         <script type="text/javascript">  
-             PL._init();   
-             PL.joinListen('/cuige/he');  
-             function onData(event) {   
-                 document.getElementById("mess").innerHTML=decodeURIComponent(event.get("cnmess"));  
-             }  
-         </script>  
-     </head>  
-     <body>  
-         <center>  
-         <h1>  
-             my first pushlet!  
-         </h1>  
-         <br><br><br>  
-         <div id="mess"></div>  
-         </center>  
-     </body>  
- </html>  
直接使用decodeURIComponent方法进行解码！
当然JS中编解码方式方法很多，不是这里所述重点不再啰嗦！
如果不解码会看到内容：
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- %E4%BC%A0%E9%80%92%E4%B8%AD%E6%96%87%E6%B5%8B%E8%AF%95%EF%BC%812012-10-08+11%3A38%3A02%EF%BC%81
你绝对认识的，至于值的对应关系，参考[http://www.w3school.com.cn/tags/html_ref_urlencode.html](http://www.w3school.com.cn/tags/html_ref_urlencode.html) ！
如果解码后会看到应该看到的汉字：
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- 传递中文测试！2012-10-08+11:39:34！  
如果不没有采取任何方式而直接使用网上的示例进行开发，一旦有中文的话就会出现消息不能推送的问题！
而且观察后台打印会发现，回话一旦传递中文，传递一次后回话就会被移除！
