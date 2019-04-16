# javaweb基础第二十一课：session - PeterBishop - CSDN博客





2019年01月06日 22:17:22[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：17








大纲:      
- session简介
- session工作原理
	- session的创建
- session的使用

- session时效
- session失效





  这节课我们来讲session，大纲已经写好了


- session简介

我们知道cookie是浏览器(客户端)保存在本地的数据

session就是服务端(服务器)保存(不在本地，在服务器里)的数据


- session的工作原理
	- session的创建


- session在第一次调用request.getSession()时创建

- session创建的同时会创建一个name为JSESSIONID(助记: J SESSION ID)的cookie保存在浏览器里

- 每次浏览器访问该项目时都会带着那个cookie
- - session 的使用


- 每次获取session时都会去浏览器里找JSESSIONID

- 如果找到JSESSIONID,则查找有没有对应的session,如果没有，创建新的session，并设置JSESSIONID，如果找到了，直接返回找到的session

- 如果没找到JSESSIONID，创建session，并设置JSESSIONID
- session时效

默认是30分钟，

注意这个过期时间是指距离最后一次使用session的时间，当距离最后一次使用session后一直没使用session超过session时效，则session会失效



可在web.xml里设置session过期时间
<session-config>

    <session-timeout>30</session-timeout>

</session-config>
session-timeout的单位是分钟
- session失效
	- session过期(太久没用，超过时效了)
- 调用session.invalidate()使session强制失效
- 项目卸载
- 服务器宕机


 总结:

session是jsp四大域之一

session对应的java类是HttpSession

session的创建或获取通过request.getSession()

session的创建必然伴随名为JSESSIONID的cookie创建



