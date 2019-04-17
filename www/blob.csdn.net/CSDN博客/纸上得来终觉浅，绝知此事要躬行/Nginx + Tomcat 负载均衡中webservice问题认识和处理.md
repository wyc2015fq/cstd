# Nginx + Tomcat 负载均衡中webservice问题认识和处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年07月14日 17:17:59[boonya](https://me.csdn.net/boonya)阅读数：8844








  前言：由于我的项目中存在发布webservice的服务，所以这里有必要将我遇到的问题梳理一下。

#### 1、Webservice 不能在同一台计算机上同时启动

![](https://img-blog.csdn.net/20140714171429218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20140714171449170?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**问题描述：**

      因为Webservice在同一台计算机上端口被先启动的Tomcat占用，导致Nginx集群不能同时启动——端口占用。

**解决方法：**

      方 法一、准备两台服务器，分别部署项目到Tomcat应用服务器。

      方 法二、在同一台计算机中时，更改其中一个Tomcat的webservice端口（此法改变了端口不建议采纳）。

#### 2、Webservice打不开[Nginx不做Webservice的代理]

**问题描述：**

      由于1中描述的问题，在本机上测试故将端口分别改为8088和8090；nginx.conf上设置了两个Tomcat的服务器地址，所以通过Nginx便可以很快速的访问到项目里的动态或者静态页面和资源。 但是通过http://localhost:8088/mobilewebservice?wsdl和http://localhost:8090/mobilewebservice?wsdl却怎么也访问不到Webservice。

注：这里的localhost是Nginx本地访问路径而不是通常我们Tomcat访问的localhost，虽然很相似但不是同一个东西，Nginx默认为80端口而Tomcat默认端口为8080。

**解决方法：**

     尝试原始Webservice访问地址——即Tomcat在没有配置负载均衡之前的地址。访问Tomcat的Webservice地址：[http://192.168.1.147:8090/mobilewebservice?wsdl](http://192.168.1.147:8090/mobilewebservice?wsdl)和[http://192.168.1.147:8088/mobilewebservice?wsdl](http://192.168.1.147:8088/mobilewebservice?wsdl)访问成功，说明Nginx的负载均衡对Tomcat原始的Webservice并不构成影响，所以原来怎么访问现在还怎么访问。

注：我的Webservice的访问端口是8088和8090。

![](https://img-blog.csdn.net/20140714171410718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20140714171709165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






