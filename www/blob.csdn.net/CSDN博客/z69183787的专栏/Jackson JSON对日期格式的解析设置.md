# Jackson JSON对日期格式的解析设置 - z69183787的专栏 - CSDN博客
2015年01月06日 21:55:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：19525
服务器端：采用Spring MVC3 + MyBatis3架构设计，使用Jackson JSON将Java对象转换成JSON格式；
**Spring：3.2.2.RELEASE**
**MyBatis：3.2.1**
**mybatis-spring：1.2.0**
前端：采用jQuery EasyUI框架，使用 jquery-json-2.4.js 将javascript对象转换成JSON格式；
交互方式：服务器端和前端采用json格式进行数据交互。
遇到问题：服务器端对Java对象序列化后，日期格式为一长串数字（如下图），前端接收之后需要转码，很麻烦。于是想到从Jackson JSON框架着手解决。
![](https://img-my.csdn.net/uploads/201303/25/1364214706_7670.png)
解决方案：
设置Jackson JSON的对日期的序列化格式即可，代码如下：
**[java]**[view
 plain](http://blog.csdn.net/zht666/article/details/8718792#)[copy](http://blog.csdn.net/zht666/article/details/8718792#)
- //设置日期格式
- SimpleDateFormat fmt = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");  
- mapper.setDateFormat(fmt);  
- 
- //后续转换
- ...  
- json = mapper.writeValueAsString(obj);  
- ...  
这样设置之后，得到的JSON格式中，日期就变成了“2013-03-25 10:20:00”这样的格式。
前端需要将日期编码成毫秒数，发送到服务器，Jackson JSON才能正确解析，否则提示无法转换成Date类型。
可以使用Javascript的Date函数的getTime()方法即可，例如：
**[javascript]**[view
 plain](http://blog.csdn.net/zht666/article/details/8718792#)[copy](http://blog.csdn.net/zht666/article/details/8718792#)
- var d = new Date($("#userBirthday").datebox('getValue'));  
- //var d = new Date();
- u.userBirthday = d.getTime(); //将日期转换成毫秒数
这样，服务端将日期做成字符串写到JSON中传到前端，前端将日期转换成毫秒数发送到服务器端，这样程序就能正常工作了。
最后上个图看看（自己闲着没事，搭建的这个系统框架，旨在学习Spring、MyBatis、Maven、jQuery、jQuery EasyUI）：
![](https://img-my.csdn.net/uploads/201303/25/1364215595_1457.jpg)最
后附上一个简单的例子：Javascript对象转换为JSON
**[html]**[view
 plain](http://blog.csdn.net/zht666/article/details/8718792#)[copy](http://blog.csdn.net/zht666/article/details/8718792#)
- <html>
- <head>
- <title>用户管理</title>
- <scripttype="text/javascript"src="jquery-1.9.1.js"></script>
- <scripttype="text/javascript"src="jquery.json-2.4.js"></script>
- <script>
-         function jsObjToJson()  
-         {  
-             var o = new Object();  
-             var d = new Date();  
- 
- o.id = "1";  
- o.name = "总公司";  
- o.timestamp = d.getTime(); //将日期转换成毫秒数  
- 
-             var postData = 'reqJson=' + $.toJSON(o); //转换成json格式  
-             alert(postData);  
-         }  
- </script>
- </head>
- <body>
- <h3>Javascript对象转换成JSON</h3>
- <inputtype="button"value="测试"onclick="jsObjToJson()"/>
- </body>
- </html>
