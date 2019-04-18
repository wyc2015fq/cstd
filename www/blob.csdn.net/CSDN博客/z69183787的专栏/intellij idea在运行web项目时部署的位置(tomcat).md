# intellij idea在运行web项目时部署的位置(tomcat) - z69183787的专栏 - CSDN博客
2017年09月19日 15:47:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：18626
[http://m.blog.csdn.net/Victor_Cindy1/article/details/72680553](http://m.blog.csdn.net/Victor_Cindy1/article/details/72680553)
以前一直很好奇，在idea中运行tomcat，把项目部署到其中，运行起来，然后我去tomcat目录下去看，根本找不到我部署的项目 那我的项目是咋运行的啊… - -
后来我就查啊查 ，纠结啊纠结，然后大概知道是咋回事了:
在idea中配置的tomcat，在运行时idea不会把项目放到该路径下，而是复制一份足够的配置文件，到 `${user.home}/.IntelliJIdea/system/tomcat` 目录下：
![这里写图片描述](https://img-blog.csdn.net/20170227113109621?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvam9lbnFj/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
也就是说每个项目都有属于自己的一份tomcat配置，互不干扰。
每个项目的配置文件夹中有一个 `/conf/Catalina/localhost/ROOT.xml` 文件，内容如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Context path="" docBase="D:\work\项目\target\xxx-web" />
```
- 1
- 2
其中，path是指在访问此项目时，是否需要添加额外的路径，如果为空，则直接使用域名或者ip就可以访问到该项目：127.0.0.1。这个值在ieda中的Run/Debug Configurations中可以配置：
![这里写图片描述](https://img-blog.csdn.net/20170227113842554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvam9lbnFj/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
docBase是指要运行的项目的部署位置，`D:\work\项目` 就是我的项目源代码的位置，target是由maven构建后生成的，`D:\work\项目\target\xxx-web` 目录就是maven
 build完成之后生成的项目，结构如下：
![这里写图片描述](https://img-blog.csdn.net/20170227114158669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvam9lbnFj/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而idea启动tomcat的命令为：
![这里写图片描述](https://img-blog.csdn.net/20170227114346759?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvam9lbnFj/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
也就是说，整个项目运行过程为：首先maven build项目，将构建结果写到项目的target目录下，然后idea复制一份tomcat的配置文件到`${user.home}/.IntelliJIdea/system/tomcat` 中，之后启动tomcat安装目录下的catalina.bat文件，tomcat读取配置文件，找到项目位置，然后就运行起来了。
在idea中运行tomcat，把项目部署运行起来，然后我去tomcat目录下去看，找不到我部署的项目 那我的项目是怎么运行起来的？
说明一下：这里是使用tomcat 部署成war exploded 而不是war包。
war exploded模式是直接把文件夹、jsp页面 、classes等等移到Tomcat 部署文件夹里面，进行加载部署。因此这种方式支持热部署，一般在开发的时候也是用这种方式。
在平时开发的时候，使用热部署的话，应该对Tomcat进行相应的设置，这样的话修改的jsp界面什么的东西才可以及时的显示出来。
![](https://img-blog.csdn.net/20170525165233617)
如上，将两处都修改成Update resources即可。
webapps下面文件夹都翻了一遍，都没有发现部署的项目。
![](https://img-blog.csdn.net/20170524171715230)
去work文件夹下也看了一遍，是空的。
![](https://img-blog.csdn.net/20170524171812746)
后来发现，在idea中配置的tomcat，在运行时idea不会把项目放到自己的webapps路径下，而是复制三份文件到 ${user.home}/.IntelliJIdea/system/tomcat 目录下的各自项目。
我们进入该目录，看到如下（名称是通过我们的项目名转化而来）：
![](https://img-blog.csdn.net/20170524172027825)
也就是说每个项目都有属于自己的一份tomcat配置，互不干扰。
我们进入其中一个项目下，看到如下：（在自己的tomcat安装目录下是看不到日志的，日志在这里，还有一些配置文件）
![](https://img-blog.csdn.net/20170524172244279)
每个项目的配置文件夹中有一个 /conf/Catalina/localhost/ROOT.xml 文件，内容如下：
![](https://img-blog.csdn.net/20170524172329450)
其中，path是指在访问此项目时，是否需要添加额外的路径，如果为空，则直接使用域名或者ip就可以访问到该项目：127.0.0.1。这个值在ieda中的Run/Debug Configurations中可以配置：(即Application context)
![](https://img-blog.csdn.net/20170524172417529)
![](https://img-blog.csdn.net/20170524172445405)
如果现在我在Application context加上：/springSecurityDemo
![](https://img-blog.csdn.net/20170524172543344)
Server这边会自动加入：springSecurityDemo/
![](https://img-blog.csdn.net/20170524172608282)
这时候，我们运行tomcat之后，在conf/Catalina/localhost没有发现ROOT.xml，而是springSecurityDemo.xml文件，内容如下：
![](https://img-blog.csdn.net/20170524172707125)
这时候项目的访问路径是：http://localhost:8145/springSecurityDemo/
docBase是指要运行的项目的部署位置，/myProject/springSecurityDemo 就是我的项目源代码的位置，build是由gradle构建后生成的，gradle build完成之后生成的项目，结构
如下：
![](https://img-blog.csdn.net/20170524172933002)
而idea启动tomcat的命令在这里：
![](https://img-blog.csdn.net/20170524172850189)
整个项目运行过程是：首先gradle build项目，将构建结果写到项目的build目录下，然后idea复制一份tomcat的conf、logs和work文件夹到${user.home}/.IntelliJIdea/system/tomcat 中，之后启动tomcat安装目录下的catalina.sh文件，tomcat读取配置文件，找到项目位置，然后就运行起来了。
-------------------------------------------------------------------------------------
如果是使用tomcat 部署成是war包。那么会不会就能在自己安装的tomcat中找到呢？
刚开始我将项目改为war部署的方式，但是运行之后tomcat中始终没有项目的文件。后来发现如果将当前的Application context设置为’/’，那么
tomcat也不会将项目部署到webapps中，因为这个相当于你项目的根路径。后来经过实践，发现这个Application context就是你项目在webapps路径下项目的根目录名。 
![](https://img-blog.csdn.net/20170525171058675)
