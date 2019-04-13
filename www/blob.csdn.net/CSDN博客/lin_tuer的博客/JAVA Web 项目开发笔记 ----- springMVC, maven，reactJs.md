
# JAVA Web 项目开发笔记 ----- springMVC, maven，reactJs - lin_tuer的博客 - CSDN博客


2017年07月10日 21:25:39[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：1598


我觉得，不管以后是怎样的，我觉得，我应该记下我曾经做过的努力，这些过程不一定是正确的，但是我要记下我走过的路，不管是弯的还是直达的~
# InteliJ搭建项目环境
首先，我认为，正确的搭建顺序应该是这样子的：
-**新建Maven项目**
-**配置tomcat**
-**搭建springMVC框架**
-**引入前端框架ReactJs**
-**UML序列图和流程图**
---一、Maven环境搭建

---我的开发工具是intelliJ，本来以为要下载配置maven环境，后来发现 intellij 已经集成了maven插件，省去的安装的麻烦。见这篇博客：

---[https://my.oschina.net/henghanan/blog/202344](https://my.oschina.net/henghanan/blog/202344)

---好吧，发现是我太天真，一步步走下来，发现结局是这样得，

![根本没有这些配置，](https://img-blog.csdn.net/20170710084003876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGluX3R1ZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---创建maven web项目

---我还是乖乖得按照这篇blog，一步步按照 配置

---建工程：

---[使用Intellij配置Maven](http://blog.csdn.net/qq_32588349/article/details/51461182)

---根据这篇教程，没有遇到什么错误，现在工程目录就建立好啦

![项目结构](https://img-blog.csdn.net/20170710093428516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGluX3R1ZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---二、配置springMVC

---[this one](http://www.cnblogs.com/qixiaoyizhan/p/5819392.html)

---原来写的一个忘记保存了，，不想再写一遍了，就按这个链接操作即可

---三、安装配置 tomcat

---官网下载

---[tomcat](http://tomcat.apache.org/download-70.cgi)

---说到这里 真的好气啊，原来下了最新版本，运行时 总是一闪而过，后来保守了一下，下载了1.7版本之后，就，，好了


---加压到要安装的地方，其实是免安装的，放在你想放的位置，去配置一下环境变量就可以啦

---- CATALINA_HOME C:\apache-tomcat-7.0.79

---- CATALINA_BASE C:\apache-tomcat-7.0.79

---- 在 Path 中添加 %CATALINA_HOME%\bin;%CATALINA_HOME%\lib;


---在命令行运行一下 startup 弹出一个一堆东西的新框，就可以啦，去访问

---[http://localhost:8080/](http://localhost:8080/)

---就可以看到可爱的小老虎了~


---使用社区版的宝宝们，，我发现，，上面的路子是走不通的，因为社区版intellij的插件中好像没有那个tomcat！！！

---然后，我发现了这个方法

---[社区版intellij tomcat配置](http://blog.csdn.net/u012364631/article/details/47682011)

---然后，就通啦

---真是坎坷，，，

---如果你还遇到了莫名其妙的错误，可能是jar包引起的哦

---四、首次运行项目遇到的问题

---按照这篇文章

---[this one](http://www.cnblogs.com/qixiaoyizhan/p/5819392.html)

---写好测试web文件后，遇到了两个问题：

---注解不识别

---访问项目链接 遇到如下错误：

---警告: No mapping found for HTTP request with URI [/Springmvc/test] in DispatcherServlet with dispatche

---发现是spring-servlet.xml文件没有配置好

---[解决方案](http://blog.csdn.net/u011410529/article/details/44731965)

---这个就是说明你的httpurl没有映射出来，也就是说的你controller没有注入。通常出现这种问题有2种情况，在springmvc-config.xml即spring配置文件中

---加上这样一句话

---<context:component-scan base-package="com.test.controllers" />

---这里包名等需要注意是否写对，我就是包名没有写对，改成自己的controller对应的包名就可以啦


---jsp在intellij中使用遇到错误

---改了上面的东西后，再次启动项目，还是有问题，报错是这样的：

---nested exception is java.lang.NoClassDefFoundError: javax/servlet/jsp/jstl/core/Config] with root cause java.lang.ClassNotFoundException: javax.servlet.jsp.jstl.core.Config

---这个问题是由在intellij中使用jstl标签库引起的，

---[解决方案](http://www.cnblogs.com/junhuawang/p/6953177.html)

---根据这个做下去，问题解决。


---<本节完–期待后续记录>


