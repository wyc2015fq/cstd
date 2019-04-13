
# web项目部署结构 - 李昆鹏的博客 - CSDN博客


2018年07月12日 10:41:39[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：624


---------------------------- web项目部署结构----------------------------
# 1. Web项目
## 1　Web项目部署后目录结构
tomcat\webapps目录是用来存放Java项目的，每个文件夹都是一个项目。默认这个目录下已经有了四个项目，都是tomcat自带的。其中ROOT就是我们测试Tomcat时访问的Tomcat主项目。
我们要创建自己的项目，需要遵循JavaEE目录结构规范，下面是hello项目的目录结构
Hello   (必须出现的)
|------存放web资源。(jsp, xml, HTML  CSS  JS图片...)
|------  WEB-INF     (必须出现的)
|------ web.xml   (必须出现的)
|------classes            Java类的编译路径（Servlet、自定义类 ）
|------lib                   需要引入的第三方的jar包.
hello是项目目录，项目目录下必须有一个名为WEB-INF的目录（必须大写），在WEB-INF目录下必须有一个web.xml文件。
WEB-INF目录是受保护目录，这下面的东西用户是不能直接访问的，但动态页面是可以访问的。通常WEB-INF下还会存在lib和classes，lib下存放项目所需的jar文件，classes目录下存在项目所需的class文件！
如果直接访问WEB-INF就会找不到路径，
![](https://img-blog.csdn.net/20180712103808446?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
web.xml文件中只需要给出XML文档声明，以及根元素。当然，根元素中包含很多与名称空间相关属性，背下来是有难度的，不过我们可以去webapps下的其他项目中去“借”。
![](https://img-blog.csdn.net/20180712103818476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2　发布项目三种方式
Ø**第一种**:就是把项目放到webapps目录下，即localhost主机名对应的项目空间目录中。
访问[http://localhost/hello/hello.html](http://localhost/hello/hello.html)
Ø**第二种**:可以把项目放到任何目录，然后通过添加<Context>元素（Context开头是大写）来指定项目的真实位置。在<Host>元素中添加<Context>元素，然后指定<Context>元素的path和docBase属性，其中path指定的是项目虚拟路径，可以随意给出，例如abc。docBase指定你的项目的真实存放的路径。
例如，项目的真实目录在F:\hello。你找到server.xml中的<Host>元素，给其添加子元素<Context>，内容如下：
![](https://img-blog.csdn.net/20180712103921121?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注意，一旦给出了docBase属性的值后，访问localhost的abc项目时，就不会再去webapps目录下去查找项目，而是通过docBase的值来锁定项目位置，即F:\hello1。
注意，访问项目的URL：http://localhost/abc/index.jsp，而不是http://localhost/hello1/index.jsp，你可能会说，我们的项目目录名叫hello1，但因为指定了<Context>元素的path属性，那么访问项目时就不会再使用项目目录名来作为访问路径了。
Ø  第三种方式与第二种比较相似，第三种是使用外部文件来添加<Context>元素。这需要在conf目录下找到catalina\localhost目录，然后在这个目录下创建一个XML文件，这个名称就是虚拟路径名。例如创建的XML文件名为def.xml，然后在def.xml文件中添加如下内容
<ContextdocBase="F:\hello1"/>
注意，这里没有给出path属性，因为已经不用给出了，path的值由XML文件的名称决定。也就是说path的值为def。访问URL为：http://localhost/def/index.jsp。

