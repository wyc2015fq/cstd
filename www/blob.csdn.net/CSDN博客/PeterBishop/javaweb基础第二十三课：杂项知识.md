# javaweb基础第二十三课：杂项知识 - PeterBishop - CSDN博客





2019年01月12日 17:20:38[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：25
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









大纲：
- 资源访问规则
- web.xml里路径写法规则
- ContextPath
- 端口号
- tomcat自己的web.xml
- tomcat的server.xml



  这课我们讲点杂项知识,讲啥看大纲
- 资源访问规则

/WEB-INF下的所有文件都无法直接访问

不在/WEB-INF下的但在/web(idea)或/WebContent(Eclipse)的可以直接访问

直接访问的意思就是通过在浏览器里直接输入文件路径就可以访问的到
- web.xml里路径写法规则
	- /*
- *.jsp
- 不带*


注意：不可以(1)和(2)混合，即/*.jsp是非法的


- ContextPath

![](https://img-blog.csdnimg.cn/20190112172001149.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

红框框里的就是ContextPath，

idea里默认ContextPath是/

Eclipse里默认ContextPath是/项目名

ContextPath就是确定根路径的

可以看到当我把ContextPath改成ABC后运行之后根路径就成了localhost:8080/ABC


- 端口号

端口：port

端口是用于监听各种服务的，一个端口只能监听一种服务，每一个应用程序都会占用一定的端口号，比如tomcat占用的端口号是8080，这也是为什么我们访问web项目url前面一定是localhost:8080



另外，tomcat占用了3个端口，8005,8009,8080(这个还是要看server.xml)
- tomcat自己的web.xml

刚刚可以看到tomcat自己里面有很多的web.xml，其中conf下的web.xml是默认配置，比如conf下的web.xml里有配置welcom-file-list

    <welcome-file-list>

        <welcome-file>index.html</welcome-file>

        <welcome-file>index.htm</welcome-file>

        <welcome-file>index.jsp</welcome-file>

</welcome-file-list>

                这也是为什么我们web项目里如果没有特别定义welcome-file-list，首页默认会是/indedx.html或/index.htm或/index.jsp
- tomcat的server.xml

tomcat的各项配置都在server.xml里，这里只是提一下，大家深入学习tomcat的时候可以去仔细研读

唯一需要了解的一点是，我们可以通过server.xml了解以及更改tomcat占用端口。



