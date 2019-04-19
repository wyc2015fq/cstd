# Servlet学习 - LC900730的博客 - CSDN博客
2017年07月25日 11:14:33[lc900730](https://me.csdn.net/LC900730)阅读数：134标签：[servlet](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## Servlet编程
#### Servlet生命周期
Servlet生命周期 
构造方法：创建servlet对象，默认情况下第一次访问servlet对象时，只会调用1次。 
init方法(有参)：创建完servlet对象后调用，只调用1次； 
service方法：servlet提供服务的方法，每次发出请求时候调用； 
    注意response、request对象 
destroy：tomcat服务器停止或者web应用重新部署，servlet对象销毁，destroy方法被调用
#### ServletConfig对象
获取servlet初始化参数 
        getinitParameter(“name”); 
        getinitParameterNames();
#### ServletContext对象
1.得到web应用路径 
        context.getContextPath() 
得到web应用参数 
        getinitParameter(“name”); 
        getinitParameterNames();
#### JAVA web路径问题
在java web项目中，写路径，那么全部打上/ 
目标资源：target.html 
角度：
```
给服务器使用；
    给浏览器使用；
```
服务器发出的还是浏览器发出的。
转发是给服务器用的，重定向是给浏览器用到的。区别在于/
- 给服务器使用的：/表示在当前web应用的根目录（webRoot下，直接写资源就可以了）
- 
给浏览器使用的：/表示在webapps的根目录下。此时还没有进入到任何一个web应用中，因此需要这样写response.sendRedirect(“/day11/targethtml”);
- 
给浏览器使用的：/表示在webapps的根目录下。此时还没有进入到任何一个web应用中，因此需要这样写response.sendRedirect(“/day11/targethtml”);
- 
html页面的超链接：
resp.getWriter().write(“[](路径)“);
浏览器发出的超链接，因此需要写上”/day11/target.html”
- 
resp.getWriter().write(““);
如果是form提交的，那么是浏览器发出的请求，因此需要写上/day11/target.html
java项目中： 
.  表示相对目录的当前路径，也就是java命令运行的目录。 
eclipse将我们的目录设置为当前项目的根目录下 
在bin目录下运行就在bin下面，在src下运行就是src目录 
但是启动tomcat是在bin目录下，因此java web中的.代表tomcat/bin目录下
结论：在web项目中，.代表tomcat的bin目录下，因此需要使用web应用下资源加载方式。使用context.getRealPath(“路径”)得到。
```java
this.getServletContext.getRealPath("/");
//返回绝对路径
String path=
this.getServletContext.getRealPath("/WEB-INF/classes/db.properities");
File file=new File(path);
/代表tomcat 的服务器的项目根目录中，最终src/db.properities会编译到/WEB-INF/classes/db.properities下。
```
