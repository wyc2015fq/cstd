# 手动创建最简单的JSP 文件 		



1、在Tomcat 6.0的安装目录的webapps目录下新建一个目录，起名叫myapp。

2、在myapp目录下新建一个目录WEB-INF，注意，目录名称是区分大小写的。

3、WEB-INF下新建一个文件web.xml，内容如下：

```xml
<?xml version="1.0" encoding="gb2312"?>

<web-app>
    <display-name>My Web Application</display-name>
    <description>
        An application for test.
    </description>
</web-app>
```

4、在myapp下新建一个测试的jsp页面，文件名为index.jsp，文件内容如下：

```jsp
<html>
<body>
    <center>Now time is: <%=new java.util.Date()%></center>
</body>
</html>
```

5、重启Tomcat。

6、打开浏览器，输入http://localhost:8080/myapp/index.jsp 。