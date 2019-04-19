# eclipse中tomcat能正常启动，但是浏览器访问不了tomcat首页之tomcat配置问题 - 建建的博客 - CSDN博客
2018年01月23日 14:34:21[纪建](https://me.csdn.net/u013898698)阅读数：79
个人分类：[java学习路线](https://blog.csdn.net/u013898698/article/category/7152763)
在eclipse中tomcat启动成功，没抛异常，但是却访问不了问[http://localhost:8080/](http://localhost:8080/)，而在外部浏览器却能访问到，问题原因何在？
    上述现象说明tomcat没问题，而在eclipse中的配置出了问题。
  原因：eclipse将tomcat的项目发布目录（tomcat 目录中的webapp）重定向了,所以你会发现在tomcat安装目录下的webapp目录里面找不到你的项目文件。
        解决办法如下：
1、先清掉在tomcat中发布的项目。（若没有做这一步，则图2中选项为灰色）
在eclipse中的server页面，双击tomcat服务，会看到如图所示的配置页面：
![](https://img-blog.csdn.net/20141104155515694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdsajE2Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                         图1
![](https://img-blog.csdn.net/20141104155618031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdsajE2Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                          图2
配置了这一步那么在Eclipse的workspace里面开发的项目当启动Tomcat服务器时同时也会拷贝到所选择的Deploy Path中。
这样即可。
