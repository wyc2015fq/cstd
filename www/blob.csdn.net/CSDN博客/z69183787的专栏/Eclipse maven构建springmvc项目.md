# Eclipse maven构建springmvc项目 - z69183787的专栏 - CSDN博客
2014年03月26日 16:21:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：716
个人分类：[项目构建-Maven](https://blog.csdn.net/z69183787/article/category/2265961)
原帖地址：
http://www.cnblogs.com/fangjins/archive/2012/05/06/2485459.html
一.背景介绍
　　对于初学者，用maven构建项目并不是一件容易的事，springmvc并不是MVC中的主流，但是本人觉得springmvc比struts要好用，真正做到了零配置。一次使用，就放不下了。
二.准备工作
　　1.Eclipse 3.7
　　2.maven
　　3.Eclipse 需要安装maven插件。url:maven - [http://download.eclipse.org/technology/m2e/releases](http://download.eclipse.org/technology/m2e/releases) 。安装maven-3.0.4。并且选择本地的maven，如下图：
![](http://pic002.cnblogs.com/images/2012/395525/2012050522453568.png)
三.构建工程
　　1.用maven插件构建项目框架
　　maven具有强大构建功能，使用maven可以构建多种不同类型的工程。这里我们构建maven-archetype-webapp类型的项目。在Eclipse->New中选择other,找到maven Project型。如下图：
![](http://pic002.cnblogs.com/images/2012/395525/2012050522524959.png)
在选完路径之后，我们选择构建类型，如下图：
![](http://pic002.cnblogs.com/images/2012/395525/2012050522543960.png)
接下来，填写工程的Group Id，Artifact Id，如下图：
![](http://pic002.cnblogs.com/images/2012/395525/2012050522571666.png)
这里的Group Id就是大项目的id，Arifact Id就是该项目的Id。就像一个大项目中有许多小项目组成一样。此时，我们的项目已经成型了，样子如下图：
![](http://pic002.cnblogs.com/images/2012/395525/2012050523013856.png)
接下来，我们要完善项目的目录，配置。
　　2.完善项目
　　首先，完善目录，增加重要的source Folder，这个不是简单的Floder,这些文件夹是会参与编译的。增加src/main/java，src/test/resources，src/test/java目录。让目录变成标准的maven结构。如下图：
![](http://pic002.cnblogs.com/images/2012/395525/2012050523075222.png)
接下来，改变一些配置：
让工程的JDK用本地的jdk;
让工程的字符集为UTF-8;
改变工程的目录顺序；
　　这些都完成之后，工程目录应该是如下的样子：
![](http://pic002.cnblogs.com/images/2012/395525/2012050523121772.png)
　　3.将工程变成web工程
　　此时，我们的工程还不是标准的web工程，可以在eclipse中增加web工程的特性，选择工程的Properties,选Project Facets,如下图：
![](http://pic002.cnblogs.com/images/2012/395525/2012050523161645.png)
这里，我们选择Dynamic Web Module,版本选择2.4，这个版本比较通用。如下图：
![](http://pic002.cnblogs.com/images/2012/395525/2012050523183630.png)
此时，我们看到目录中多了一个WebContent目录，由于使用maven构建，web目录是src/main/webapp,所以我们删除WebContent目录。接下来，要配置web项目的发布目录，就是Deployment Assembly，如图：
![](http://pic002.cnblogs.com/images/2012/395525/2012050523222358.png)
test目录不要发布，WebContent目录没有了，所以这三项都删掉。并且增加src/main/webapp目录，和Maven Dependenices,完成之后如下图：
![](http://pic002.cnblogs.com/images/2012/395525/2012050523240143.png)
于是，我们的工程就完全是一个web工程了。
　　4.赋予工程的springmvc特性
　　配置web.xml，使其具有springmvc特性，主要配置两处，一个是ContextLoaderListener，一个是DispatcherServlet。代码如下：　　
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)web.xml
```
　　配置ContextLoaderListener表示，该工程要以spring的方式启动。启动时会默认在/WEB-INF目录下查找applicationContext.xml作为spring容器的配置文件，这里可以初始化一些bean，如DataSource。我们这里什么也不做。代码如下：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)applicationContext.xml
```
　　配置DispatcherServlet表示，该工程将采用springmvc的方式。启动时也会默认在/WEB-INF目录下查找XXX-servlet.xml作为配置文件，XXX就是DispatcherServlet的名字，该文件中将配置两项重要的mvc特性：
HandlerMapping,负责为DispatcherServlet这个前端控制器的请求查找Controller；
ViewResolver,负责为DispatcherServlet查找ModelAndView的视图解析器。
代码如下：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)exam-servlet.xml
```
　　5.让maven自动配置jar包
　　在用maven生成框架时，就生成了pop.xml，这就是maven的配置文件。我们要引入spring-web,servlet等特性的包。代码如下：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)pop.xml
```
　　maven就是这么简单，一旦保存，maven就会自动下载pop.xml的jar包。此时可以看到目录中Maven Dependencies下生成了jar包。
　　更多的jar包可以在maven中心库下载：[http://mvnrepository.com](http://mvnrepository.com/)。
　　6.做个测试
　　说了一大堆，只有运行起来才有意思，下面写个简单的测试。先写Controller。编写两个类，LoginControler.java，LoginForm.java。代码如下：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)LoginController.java
```
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)LoginForm.java
```
　　再增加一些jsp,首页的index.jsp,主要是做跳转，代码如下：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)index.jsp
```
　  还有两个jsp，做些简单的功能，一个表单login.jsp,一个表单提交的返回index.jsp，代码如下：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)login.jsp
```
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)index.jsp
```
　　整个项目的目录结构如下：
![](http://pic002.cnblogs.com/images/2012/395525/2012050600001575.png)
　　在tomcat中运行，即可。不要使用Tomcat插件，可能有问题。就用普通的方式。运行效果如下图：
![](http://pic002.cnblogs.com/images/2012/395525/2012050600083578.png)
