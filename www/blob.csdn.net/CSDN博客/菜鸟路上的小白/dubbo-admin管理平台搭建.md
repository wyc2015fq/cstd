# dubbo-admin管理平台搭建 - 菜鸟路上的小白 - CSDN博客





2016年09月09日 10:24:02[somnus_小凯](https://me.csdn.net/u012486840)阅读数：4045








## 一、前言


dubbo的使用，其实只需要有注册中心，消费者，提供者这三个就可以使用了，但是并不能看到有哪些消费者和提供者，为了更好的调试，发现问题，解决问题，因此引入dubbo-admin。通过dubbo-admin可以对消费者和提供者进行管理。

## 二、下载与配置打包


dubbo-admin的下载，可自行到官网下载：[https://github.com/alibaba/dubbo](https://github.com/alibaba/dubbo)




下载下来是这样的一个目录结构：

![](https://img-blog.csdn.net/20160921153537438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


但是这里我们只关心dubbo-admin这个文件夹。


其实下面所谓配置的目的就是为了得到war包，war包网上也有，但是下载了很多下来都会有问题，原因可能是每个人的电脑jdk版本或其他环境不一样，因此我们自己打包一个war就好。


打包war包，进入dubbo-admin这个文件目录 运行命令：

```java
mvn package -Dmaven.skip.test=true
```

![这里写图片描述](https://img-blog.csdn.net/20160921153626979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如果看到如下结果，说明打包成功了：

![这里写图片描述](https://img-blog.csdn.net/20160921153641143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


打包成功之后，就会发现dubbo-admin下多了个target文件夹，打开target文件夹，发现里面有个war包：

![这里写图片描述](https://img-blog.csdn.net/20160921153656417?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 三、安装dubbo-admin


第二步我们得到dubbo-admin-2.5.4-SNAPSHOT.war，下面我们将这个war包部署到tomcat上。


把dubbo-admin-2.5.4-SNAPSHOT.war放到tomcat的webapps目录下：

![这里写图片描述](https://img-blog.csdn.net/20160921153711683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


然后打开tomcat的conf目录下的server.xml文件，把启动端口改成8090，因为zookeeper默认使用的是8080，以免冲突：

![这里写图片描述](https://img-blog.csdn.net/20160921153724628?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


启动tomcat，让它把war解压了

![这里写图片描述](https://img-blog.csdn.net/20160921153737089?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![这里写图片描述](https://img-blog.csdn.net/20160921153749496?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如果一直提示unable to open the service ‘tomcat*’，那么重新安装一个tomcat（注意一定要使用tomcat安装包，重新安装，不是解压可用那种）

![这里写图片描述](https://img-blog.csdn.net/20160921153802364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


tomcat的webapps里面就会多了这个文件夹：

![这里写图片描述](https://img-blog.csdn.net/20160921153815762?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


然后就可以把tomcat stop了


打开刚刚tomcat解压生成的dubbo.properties，我的在Tomcat 7.0\webapps\dubbo-admin-2.5.4-SNAPSHOT\WEB-INF下：

![这里写图片描述](https://img-blog.csdn.net/20160921153829427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


内容是：

```java
dubbo.registry.address=zookeeper://127.0.0.1:2181
dubbo.admin.root.password=root
dubbo.admin.guest.password=guest
```


知道root的密码：root，guest的密码：guest，待会登录用到

## 四、下面运行

先启动zookeeper，然后在启动tomcat，访问地址：[http://localhost:8090/dubbo-admin-2.5.4-SNAPSHOT/](http://localhost:8090/dubbo-admin-2.5.4-SNAPSHOT/)

![这里写图片描述](https://img-blog.csdn.net/20160921153842521?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


输入刚刚的用户名root和密码root：

![这里写图片描述](https://img-blog.csdn.net/20160921153856090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


完成了，爽歪歪！！！！



