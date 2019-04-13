
# Tomcat安装 - 李昆鹏的博客 - CSDN博客


2018年07月12日 10:35:09[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：33


-------------------------------Tomcat安装------------------------------------
# 1. Tomcat安装
## 1　Tomcat概述
Tomcat服务器由Apache提供，开源免费。开发学习使用最佳选择。安装Tomcat之前需要先安装JDK，其实无论哪一种JavaWeb服务器都需要先安装JDK。我们现在使用的Tomcat版本可以是Tomcat6或Tomcat7。
Tomcat6支持Servlet2.5；
Tomcat7支持Servlet3.0；
## 2　Tomcat都做了些什么？
当用户通过发送过来一个请求后，Tomcat来接收请求，然后给用户返回响应。通常用户都是通常浏览器发出请求，然后浏览器的响应都是html代码。当然，这也不是全部，有时用户可能会通过其他手段向服务器发送请求，而Tomcat返回的响应也可能不是html，也可能是一个文件，一张图片等。
![](https://img-blog.csdn.net/2018071210333511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 3　安装Tomcat（解压版）
解压版Tomcat 只需要解压就能使用，无需安装，这样一台电脑上可以安装多个Tomcat。
启动解压版的Tomcat有点不同，你需要先去配置环境变量：
l  JAVA_HOME：没什么可多说的，如果你这都不会。
环境变量需要配置JAVA_HOME、CATALINA_HOME和path路径
![](https://img-blog.csdn.net/20180712103349627?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018071210340190?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在dos命令窗口输入：startup启动tomcat服务器
在浏览器中输入：[http://localhost:8080/](http://localhost:8080/)，响应服务器

启动：在Tomcat安装目录中的bin目录中找到startup.bat文件，然后双击之。
停止：在Tomcat安装目录中的bin目录中找到shutdown.bat文件，然后双击之。
建议使用解压版！！！
![](https://img-blog.csdn.net/20180712103441898?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 4　测试Tomcat
测试Tomcat需要在浏览器中输入：http://localhost:8080，或者是http://127.0.0.1:8080。127.0.0.1表示本机的IP地址，localhost表示本机的主机名。
在dos命令窗口输入：ipconfig可以查看IP地址
如果其他机器需要访问，那么要把localhost使用本机IP地址替换。
假如我的IP是192.168.1.100，你们要访问我机器上的Tomcat，那么需要在你们的浏览器中访问：http://192.168.1.100:800
![](https://img-blog.csdn.net/20180712103455703?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

