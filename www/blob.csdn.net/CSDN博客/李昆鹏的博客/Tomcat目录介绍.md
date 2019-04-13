
# Tomcat目录介绍 - 李昆鹏的博客 - CSDN博客


2018年07月12日 10:36:32[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：45标签：[tomcat																](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)个人分类：[Tomcat																](https://blog.csdn.net/weixin_41547486/article/category/7796475)


------------------------------- Tomcat目录介绍---------------------------------
# 1. Tomcat分析
## 1　Tomcat目录结构
我们有必要去了解一下Tomcat目录结构，这有助与我们更好的学习Tomcat。
![](https://img-blog.csdn.net/20180712103543609?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
l**bin**：该目录下存放的是可执行文件，有startup.bat和shutdown.bat文件，startup.bat用来启动Tomcat，但需要先配置JAVA_HOME和shutdawn.bat用来停止Tomcat；
l**conf**：这是一个非常非常重要的目录，这个目录下有三个最为重要的文件：
Øserver.xml：配置整个服务器信息。例如修改端口号，下面会详细介绍这个文件；
Ø  web.xml：部署描述符文件，这个文件中注册了很多MIME类型，即文档类型。这些MIME类型是客户端与服务器之间说明文档类型的，如用户请求一个html网页，那么服务器还会告诉客户端浏览器响应的文档是text/html类型的，这就是一个MIME类型。客户端浏览器通过这个MIME类型就知道如何处理它了。当然是在浏览器中显示这个html文件了。但如果服务器响应的是一个exe文件，那么浏览器就不可能显示它，而是应该弹出下载窗口才对。MIME就是用来说明文档的内容是什么类型的！
l**lib**：Tomcat的类库，里面是一大堆jar文件。如果需要添加Tomcat依赖的jar文件，可以把它放到这个目录中，当然也可以把项目依赖的jar文件放到这个目录中，这个目录中的jar所有项目都可以共享之；
l  logs：这个目录中都是日志文件，记录了Tomcat启动和关闭的信息，如果启动Tomcat时有错误，那么异常也会记录在日志文件中。
l  temp：存放Tomcat的临时文件，这个目录下的东西可以在停止Tomcat后删除！
l**webapps**：存放web项目的目录，其中每个文件夹都是一个项目；如果这个目录下已经存在了目录，那么都是tomcat自带的。项目。其中ROOT是一个特殊的项目，在地址栏中没有给出项目目录时，对应的就是ROOT项目。
lwork：运行时生成的文件，最终运行的文件都在这里。通过webapps中的项目生成的！可以把这个目录下的内容删除，再次运行时会生再次生成work目录。当客户端用户访问一个JSP文件时，Tomcat会通过JSP生成Java文件，然后再编译Java文件生成class文件，生成的java和class文件都会存放到这个目录下。
l  LICENSE：许可证。
l  NOTICE：说明文件。
你应该已经发现，访问ROOT项目时无需给出项目名，其实localhost是一个主机名，每个主机都会对应一个项目空间，localhost对应的就是Tomcat目录下的webapps，每个项目空间都可以有一个名为ROOT项目，这个ROOT项目被访问时无需给出项目名称。
## 2　Tomcat结构之重点
**bin****、conf****、webapps、work。**
bin目录中东西我们只关心启动和停止Tomcat，现在大家也应该掌握了！
webapps目录下存放的是项目，每个文件夹都对应一个项目，而且在访问项目时，其中包含了项目名，而项目名往往都对应项目目录名。这个也应该可以掌握了！还有就是JavaEE Web项目的目录结构，这个必须必须背下来！
work：这个东西有点小难度，现在只需要知道它存放的文件是通过webapps目录下的项目在运行时产生的，最终运行的都是work目录下的文件。这个目录下的东西可以删除，然后再次运行时还会产生！

## 3　修改端口号
打开server.xml文件
![](https://img-blog.csdn.net/20180712103601545?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
http默认端口号为80，也就是说在URL中不给出端口号时就表示使用80端口。当然你也可以修改为其它端口号。修改之后必须重新启动服务器
如果服务器启动后
java.net.BindException: Address already inuse: JVM_Bind<null>:80
![](https://img-blog.csdn.net/20180712103617756?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
就是80端口被占用了
80端口是默认的端口，如果使用80端口访问时就可以省略端口
如果我们改成80端口，访问时就可以不加端口号。http://localhost/

