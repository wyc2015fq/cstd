# 《转》request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+request.getContextP... - weixin_33985507的博客 - CSDN博客
2013年04月19日 13:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**String path = request.getContextPath();String basePath = request.getScheme()****+"://"****+request.getServerName()+":"+request.getServerPort()+path+"/";    <base href=" <%=basePath%>">**
**这个语句是用来拼装当前网页的相对路径的。<base href="...">是用来表明当前页面的相对路径所使用的根路径的。比如，页面内部有一个连接，完整的路径应该是 http://localhost:80/myblog/authen/login.do其中http://****server/是服务器的基本路径，myblog是当前应用程序的名字，那么，我的根路径应该是那么http://localhost:80/myblog/。有了这个 <base ... >以后，我的页面内容的连接，我不想写全路径，我只要写 authen/login.do就可以了。服务器会自动把 <base ...>指定的路径和页面内的相对路径拼装起来，组成完整路径。如果没有这个 <base...>，那么我页面的连链接就必须写全路径，否则服务器会找不到。request.getSchema()可以返回当前页面使用的协议，就是上面例子中的“http”request.getServerName()可以返回当前页面所在的服务器的名字，就是上面例子中的“localhost"request.getServerPort()可以返回当前页面所在的服务器使用的端口,就是80，request.getContextPath()可以返回当前页面所在的应用的名字，就是上面例子中的myblog这四个拼装起来，就是当前应用的跟路径了**
**最近在自学struts，被里面struts中action 的path与form表单的action属性弄迷糊了。**
**struts-config.xml 文件中，action元素中的path属性表示的是浏览器地址栏中相对于应用程序根目录的请求路径，与form 中提交表单以后有谁处理的action属性指定的根路径一致。(只是一致，千万不要以为是绝对相等~)**
**例如：form表单的提交处理请求是classesAdd.do，其在ie地址栏中的路径如下所示，**
**http://localhost:9000/Struts_study/classesMan/classesAdd.do**
**红色部分表示的根路径，所以，action中的classesAdd.do请求的完整路径是classesMan/classesAdd.do**
**<form name="form1" action="classesAdd.do" method=post>**
**所以 struts 中的action 的path路径是指/classesMan/classesAdd。**
**大家可以看着浏览器的地址栏加以配置。祝大家晚上睡觉快乐。**
**补充一下吧，form中的action的默认路径就是当前路径，**
**而struts中的action 的path属性默认路径为根路径，所以要加上所在的文件夹得路径。**
**转自：[http://blog.csdn.net/ac903919/article/details/5038685](http://blog.csdn.net/ac903919/article/details/5038685)**
