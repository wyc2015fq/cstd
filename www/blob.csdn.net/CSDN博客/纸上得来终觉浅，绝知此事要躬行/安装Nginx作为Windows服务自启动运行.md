# 安装Nginx作为Windows服务自启动运行 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年07月16日 17:15:17[boonya](https://me.csdn.net/boonya)阅读数：18706








如果Nginx每次使用都需要手动启动确实很麻烦，所以最好将其设置为Windows系统服务，开机自启动就行了。

#### 1.下载并修改运行环境支持程序

1）、下载地址：[http://ng-srvinst.attron.org/](http://ng-srvinst.attron.org/)

2）、修改ng_srv.xml文件内容：



```
<service>  
 <id>nginx</id>  
  <name>nginx</name>  
  <description>nginx</description>  
  <executable>d:\www\nginx\nginx.exe</executable>  
  <logpath>d:\www\logs\</logpath>  
  <logmode>roll</logmode>  
  <depend></depend>  
  <startargument>-p d:\www\nginx</startargument>  
  <stopargument>-p d:\www\nginx -s stop</stopargument>  
</service>
```

将上述文件的Nginx目录改为自己本机Nginx所在的目录。我的Nginx版本是1.6.0，修改后的文件如下：



```
<service>  
 <id>nginx</id>  
  <name>nginx</name>  
  <description>nginx</description>  
  <executable>D:\BALANCE_TOMCAT__NGINX\nginx-1.6.0\nginx.exe</executable>  
  <logpath>D:\BALANCE_TOMCAT__NGINX\nginx-1.6.0\logs\</logpath>  
  <logmode>roll</logmode>  
  <depend></depend>  
  <startargument>-p D:\BALANCE_TOMCAT__NGINX\nginx-1.6.0</startargument>  
  <stopargument>-p D:\BALANCE_TOMCAT__NGINX\nginx-1.6.0 -s stop</stopargument>  
</service>
```


注：如果未修改此文件就安装服务，服务无法启动。会出现如下错误：





![](https://img-blog.csdn.net/20140717102424843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 2.安装程序设置为Windows服务操作

在下载文件中有read.me打开之内容如下：


```
NG_SRV

Installation for nginx server like a service.

1. Change in ng_srv.xml path for your nginx path.
2. Launch "ng_srvinst.bat install" for service installation.

3. For uninstall launch "ng_srvinst.bat remove".

Thats all.
```
下载文件的目录结构如下：

![](https://img-blog.csdn.net/20140716171251312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

将ng_srvinst.bat安装为系统服务，如下图：

![](https://img-blog.csdn.net/20140717102814324?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


安装完成后查看是否安装成功“计算机”->"管理"->"服务"，你可以看到Nginx服务已经安装成功，如下图：




![](https://img-blog.csdn.net/20140717103012073?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


参考地址：[http://blog.csdn.net/csfreebird/article/details/8751532](http://blog.csdn.net/csfreebird/article/details/8751532)





