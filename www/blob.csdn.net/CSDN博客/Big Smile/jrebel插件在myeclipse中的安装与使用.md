# jrebel插件在myeclipse中的安装与使用 - Big Smile - CSDN博客
2017年12月03日 22:50:03[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1510
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
      在公司使用tomcat有一段时间了，因为修改完代码之后总是重启项目才能看到效果，所以果断搜索了一下tomcat的自动热部署的配置，结果发现在配置文件中就可以轻松的实现这个功能，所以果断的试了一下，发现部署确实是部署了，但是每次都是通过重新启动项目来实现的，因为项目比较大，所以有很大的延迟，导致在调试的时候需要等待，所以就用了一个后辈推荐的插件jrebel，然后发现很好的解决了我的问题，在这里和大家分享一下！
# 一、Tomcat实现热部署和热加载
        其实使用tomcat实现热部署和热加载的方法，大家应该都比较了解，这里简单说一下，其实需要注意的有两点，第一就是配置，第二就是生成的发布文件存放的位置。
        首先来看配置：
       热加载：需要在server.xml -> context 属性中 设置 reloadable="true"
`<Context docBase="xxx" path="/xxx" reloadable="true"/>  `        热部署：在server.xml -> context 属性中 设置  autoDeploy="true"
`<Context docBase="xxx" path="/xxx" autoDeploy="true"/>  `       其实也可以检查一下下面的：
```
<Host name="localhost"  appBase="webapps"  
            unpackWARs="true" autoDeploy="true"  
            xmlValidation="false" xmlNamespaceAware="false">  
<Context docBase="xxx" path="/xxx"/>   
</Host>
```
     通过以上简单的设置，就可以实现，保存代码之后，在eclipse里面就可以看到console控制台里面跑日志，这就说明你的热加载和热部署就成功了。
这里有个小区别：
**热加载：**监听class，config等文件，保持局部与整体的统一性，一般会重新加载被修改的类或者配置，不会清除缓存。
**热部署：**监听的是打包完成的文件，如war，ear等，如果发生变化，就会重新部署一次，先释放内存，再加载新的包，会清空session。
# 二、使用jrebel插件
        我们在myeclipse和eclipse中可以很方便的找到这个插件，但是不能直接搜索它的名字，而且还有版本的问题，这里我分步说明。
        首先就是版本的选择，我们现在在网上能找到的破解版的版本停留在了7.0左右，而正式版的更新到了7.3左右，其实还是蛮够用的，网上流传最多的版本是6.5这个版本，一般适用于jdk7和tomcat7.X的版本。而到了jdk8和tomcat8.x的版本的话，就得适用7.0级以上版本，特此说明。
        我们在店中下载的话，一般都是最新版本的，所以到没有什么需要注意的，但是有使用期限，到是一个问题，今天先从最简单的安装开始：
![](https://img-blog.csdn.net/20171203214633193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
首先找到商店的位置，然后切换到eclipse的商店，搜索Optimizer for Eclips，就可以看到我们的插件平台了，点击install：
![](https://img-blog.csdn.net/20171203215111219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后会重启一次，进入Optimizer for Eclips的界面，
     可以的话，这里可以注册一下：
![](https://img-blog.csdn.net/20171203215552666?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后就可以安装jrebel了
![](https://img-blog.csdn.net/20171203215915096?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
直接安装全部插件，最后同意一下协议就等待安装，重启就可以了：
![](https://img-blog.csdn.net/20171203220102851?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
jrebel配置：
![](https://img-blog.csdn.net/20171203224427165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        首先找到jrebel的配置中心，![](https://img-blog.csdn.net/20171203224603314?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在这里设置项目重新部署的时间，
![](https://img-blog.csdn.net/20171203224740477?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在这里设置传统的jrebel模式，然后指定应用的项目：
![](https://img-blog.csdn.net/20171203224847702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样启动项目之后，项目就处于jrebel的控制之下了。
