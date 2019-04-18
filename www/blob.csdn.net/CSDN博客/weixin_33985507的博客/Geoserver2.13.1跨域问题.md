# Geoserver2.13.1跨域问题 - weixin_33985507的博客 - CSDN博客
2018年10月25日 19:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
### 一、跨域问题报错如下
![11354300-18908c7cb02d0b76.png](https://upload-images.jianshu.io/upload_images/11354300-18908c7cb02d0b76.png)
image.png
### 二、查看自己Geoserver的Jettry版本
![11354300-58fb9bb032acbe0e.png](https://upload-images.jianshu.io/upload_images/11354300-58fb9bb032acbe0e.png)
image.png
### 三、从网址下载和Jetty相应版本的servlets
打开网址[http://central.maven.org/maven2/org/eclipse/jetty/jetty-servlets/](http://central.maven.org/maven2/org/eclipse/jetty/jetty-servlets/)，找到在第二步中和Jetty相同版本的servlets为jetty-servlets-9.2.13.v20150730.jar。
![11354300-a3696496adb7c3ea.png](https://upload-images.jianshu.io/upload_images/11354300-a3696496adb7c3ea.png)
image.png
下载后把jar包放到下面的文件夹下：
![11354300-adcc047beee47d52.png](https://upload-images.jianshu.io/upload_images/11354300-adcc047beee47d52.png)
image.png
### 四、修改web.xml文件
该文件在 D:\GeoServer\GeoServer 2.13.1\webapps\geoserver\WEB-INF 目录下，在此文件中找到下图标红的两段代码，然后取消注释，重启geoserver，跨域问题解决。
![11354300-9103830c283323c7.png](https://upload-images.jianshu.io/upload_images/11354300-9103830c283323c7.png)
image.png
