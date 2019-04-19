# 【tomcat】配置指定文件夹发布项目 - Big Smile - CSDN博客
2018年05月13日 16:41:39[王啸tr1912](https://me.csdn.net/tr1912)阅读数：5183
        在我们发布项目的时候，通常都是打一个war包，然后上传到tomcat的webapp目录下，然后启动tomcat服务器，这个时候，我们想要访问我们的项目，通常是用“IP:PORT/项目名”的方式来访问的。
        但是由于项目要部署在正式的服务器上，并且我们的访问地址就只能是甲方申请的域名，这个时候要怎么搞呢？
# 一、给tomcat添加context
        先来说一下步骤：
    首先我们需要修改tomcat下面conf文件夹下server.xml文件，在最下方的host标签下面，我们添加一个context标签。
![](https://img-blog.csdn.net/20180513155619435?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```
<Context path="" reloadable="false" docBase="指定的项目文件路径">
	<Logger className="org.apache.catalina.logger.SystemOutLogger" verbosity="4" timestamp="true"/>
</Context>
```
    这个context就是我们需要发布的，不带项目名称就可以访问的项目所需要的配置。
在docBase下面我们可以配置我们项目所在的文件夹的目录，例如：/usr/local/deploy/ymqynewback
配置好这个context之后，我们可以在我们新建的这个发布目录/usr/local/deploy/ymqynewback下面，放入我们所打的war包里面的东西，例如：
![](https://img-blog.csdn.net/20180513162209679?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如图所示，我们打的war包就是我们项目中webroot目录下面的所有的文件，包括生成的classsis文件等等。类比maven项目就和target目录下面的东西差不多，我们可以直接把webroot打成一个zip包，或者target打成一个zip包，然后上传到我们的服务器‘/usr/local/deploy’目录下面解压，然后使用unzip命令解压我们打的zip包。例如：
![](https://img-blog.csdn.net/20180513162626659?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们打一个这样的包，在服务器上面解压缩：
`unzip WebRoot.zip`
然后我们可以把解压之后的WebRoot文件夹重命名为ymqynewback这个文件夹（原来的ymqynewback可以先删除）。
这样就可以了，然后我们去启动tomcat：
`./startup.sh`
这样我们就可以直接通过访问域名或者localhost:8080来访问我们的项目了。
# 二、context解释
 <Context>元素的属性: 
**path:**
指定访问该Web应用的URL入口，即虚拟文件目录。
如果配置path="test"的话，那么我们访问项目的时候需要访问的地址后面需要加上test
浏览器访问地址为："http://IP地址或域名:端口/test"
paht可以为空字符串，为空字符串时表示此项目为Tomcat默认的项目。
**docBase:**
docBase除了可以为本地项目WebRoot绝对路径，也可以是相对Tomcat的webapps目录的成品项目（可以理解为“war文件解压后的项目”）路径。
如第一项里面我们所做的配置就是把‘/usr/local/deploy/ymqynewback’配置到docBase下面，就相当于是项目目录了。
**reloadable:**
如果这个属性设为true，tomcat服务器在运行状态下会监视在WEB-INF/classes和WEB-INF/lib目录下class文件的改动， 如果监测到有class文件被更新的，服务器会自动重新加载Web应用。
另外需要注意的是我们的path为空的context只能指定一个，因为这个相当于是默认的启动项目。
